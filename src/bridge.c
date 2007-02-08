/*
	Copyright (C) 2003  Sridhar Ratna <srid@nearfar.org>
	
	This file is part of GNUSim8085.

	GNUSim8085 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	GNUSim8085 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with GNUSim8085; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* bridge.c is really an uglier code! sorry ;-| */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "bridge.h"

#include "8085-asm.h"
#include "8085.h"
#include "gui-app.h"
#include "8085-link.h"
#include "gui-view.h"
#include "gui-list-stack.h"
#include <gnome.h>

#define SIM_PROG_RUNNING _("Simulator: Program running")

/* DATA */
static BState state = B_STATE_IDLE;	/* state of the program */
static gboolean saved_and_assembled_successfully = FALSE;
static gboolean execution_single_stepped = FALSE;

/* if above is TRUE , then associate with these */

static BTraceMode ess_trace_mode = B_TRACE_IN;	/* supplementry vars below */
static gboolean ess_trace_mode_over_inside_func = FALSE;
static eef_addr_t ess_trace_mode_over_stack_cont = 0xFF;
static gint ess_trace_mode_out_callret = 0;


static guint16 executed_bytes = 0;
static guint16 last_loaded_address = 0;

/* for break point and trace */
static guint16 about_to_execute_from = 0;

static AsmSource *ds_source = NULL;
static EefMemBlock *ds_memblock = NULL;

/* appbar id */
GtkWidget *appbar = NULL;

/* Methods */
BState
b_get_state (void)
{
	return state;
}

static void
_end_of_exec (void)
{
	gnome_appbar_pop (GNOME_APPBAR(appbar));
	//gui_list_stack_reset ();
}

static void
_update_gui_state (void)
{
	gboolean bdebug = FALSE;
	GtkWidget *widget;
	if (state == B_STATE_DEBUG)
		bdebug = TRUE;

#define DF(wstr, issen) \
	widget = lookup_widget (app->window_main, wstr); \
	g_assert (widget); \
	gtk_widget_set_sensitive (widget, issen);



#undef DF
	
	GtkAction *action_widget;
#define DF_ACTION(wstr, issen) \
	action_widget = lookup_action_widget (app->window_main, wstr); \
	g_assert (action_widget); \
	gtk_action_set_sensitive (action_widget, issen);

	DF_ACTION ("newfile", !bdebug);
	DF_ACTION ("openfile", !bdebug);
	DF_ACTION ("savefile", !bdebug);
	DF_ACTION ("savefileas", !bdebug);
	DF_ACTION ("resetregisters", !bdebug);
	DF_ACTION ("resetflags", !bdebug);
	DF_ACTION ("resetports", !bdebug);
	DF_ACTION ("resetmemory", !bdebug);
	DF_ACTION ("resetall", !bdebug);
	DF_ACTION ("assemble", !bdebug);
	DF_ACTION ("listing", !bdebug);


#undef DF_ACTION
}

static void
ds_clean_up (void)
{
	if (ds_memblock)
		eef_mem_block_delete (ds_memblock, TRUE);
	ds_memblock = NULL;

	if (ds_source)
		asm_source_destroy (ds_source);
	ds_source = NULL;
}

/* CALLBACK  */


static void
hightlight_line (gint ln)
{
	gui_editor_clear_all_highlights (app->editor);

	if (ln != -1)
	{
		gui_editor_goto_line (app->editor, ln);
		gui_editor_set_highlight (app->editor, TRUE);
	}
}

static void
bridge_debug_this_line (eef_addr_t addr, gint ln)
{
	if (ln != -1)
	{
		about_to_execute_from = addr;
		state = B_STATE_DEBUG;
	}

	gui_view_update_all ();
	_update_gui_state ();

	hightlight_line (ln);
}

/* If returned TRUE 8085 will proceed with executing
 * instruction at "addr" otherwise it will return to caller. */
gboolean prev_is_call_instr = FALSE;
gboolean
_bridge_8085_cb (eef_addr_t addr, eef_addr_t prev_addr, gboolean finished)
{
	gint ln;
	gboolean break_pt = FALSE;
	eef_data_t opcode;

	if (finished)
	{
		state = B_STATE_IDLE;
		prev_is_call_instr = FALSE;
		execution_single_stepped = FALSE;
		saved_and_assembled_successfully = FALSE;
		bridge_debug_this_line (0, -1);
		gui_editor_set_readonly (app->editor, FALSE);
		gui_editor_set_highlight (app->editor, FALSE);
		//gnome_appbar_pop (appbar);
		_end_of_exec ();
		return TRUE;	/* caller is going to return */
	}

	/* get opcode */
	opcode = eef_get_op_at_addr (addr);
	if (eef_is_call_instr (eef_get_op_at_addr (prev_addr)))
		prev_is_call_instr = TRUE;
	else
		prev_is_call_instr = FALSE;

	/* get line no */
	ln = eef_link_get_line_no (addr);
	if (ln == -1)
	{
		GString *err_str;

		err_str = g_string_new ("");
		g_string_printf (err_str,
				 _("Execution branched to invalid memory location <%xH>. Execution will be stopped!\n\n\
Check whether you've included the \"hlt\" instruction at the end of your program and your source program\
 is not empty.  Also check your program logic. If you're very sure, there might be a bug in GNUSim8085.\
If so you're advised to send a copy of your source progam."), addr);
		/* fatal error */
		gui_app_show_msg (GTK_MESSAGE_WARNING, err_str->str);
		g_string_free (err_str, TRUE);

		b_debug_stop ();
		//g_return_val_if_fail (ln != -1, FALSE);
		return FALSE;
	}

	break_pt = gui_editor_is_marked (app->editor, ln);
	if (!execution_single_stepped && break_pt)
	{
		/* break */
		bridge_debug_this_line (addr, ln);
		return FALSE;
	}
	else if (execution_single_stepped)
	{
		/* find the mode - ess_trace_mode */
		if (ess_trace_mode == B_TRACE_OVER)
		{
			/* normal trace */
			if (ess_trace_mode_over_inside_func == FALSE)
			{
				if (prev_is_call_instr)
				{
					ess_trace_mode_over_inside_func =
						TRUE;
					ess_trace_mode_over_stack_cont =
						eef_regpair_get ('S') + 2;
					prev_is_call_instr = FALSE;
					return TRUE;
				}

				bridge_debug_this_line (addr, ln);
				return FALSE;

			}
			/* inside call */
			if (ess_trace_mode_over_stack_cont ==
			    eef_regpair_get ('S'))
			{
				ess_trace_mode_over_inside_func = FALSE;
				bridge_debug_this_line (addr, ln);
				return FALSE;
			}
			return TRUE;	/* finish the func */
		}
		else if (ess_trace_mode == B_TRACE_OUT)
		{
			if (eef_is_call_instr
			    (eef_get_op_at_addr (prev_addr)))
				ess_trace_mode_out_callret++;
			if (eef_is_ret_instr (eef_get_op_at_addr (prev_addr)))
				ess_trace_mode_out_callret--;
			if (ess_trace_mode_out_callret < 0)
			{
				bridge_debug_this_line (addr, ln);
				return FALSE;
			}
			return TRUE;
		}
		else if (ess_trace_mode == B_TRACE_IN)
		{
			bridge_debug_this_line (addr, ln);
			return FALSE;
		}
	}


	return TRUE;
}

void
b_init (void)
{
	/* set callback */
	eef_set_trace_callback (_bridge_8085_cb);

	/* set appbar id */
	appbar = lookup_widget (app->window_main, "main_appbar");
	g_assert (appbar);
}

gboolean
b_assemble (char *text, guint16 start_addr)
{
	g_assert (state == B_STATE_IDLE);

	/* clean up previous */
	ds_clean_up ();

	/* restart asm */
	eef_asm_init ();

	sys.reg.sph = 0xff;
	sys.reg.spl = 0xff;
	gnome_appbar_push (GNOME_APPBAR(appbar), _("Assembler: running"));
	saved_and_assembled_successfully =
		eef_asm_assemble (text, start_addr, &ds_source, &ds_memblock);
	gnome_appbar_pop (GNOME_APPBAR(appbar));

	if (saved_and_assembled_successfully)
		last_loaded_address = start_addr;
	else
	{
		last_loaded_address = 0;
		ds_clean_up ();
		return FALSE;
	}

	return TRUE;
}

gboolean
b_execute (void)
{
	//g_assert (state == B_STATE_IDLE);

	if (!saved_and_assembled_successfully)
		return FALSE;

	if (state == B_STATE_DEBUG)
	{
		/* resuming execution */
		state = B_STATE_IDLE;
		execution_single_stepped = FALSE;
		gui_editor_set_readonly (app->editor, TRUE);
		gui_editor_set_highlight (app->editor, TRUE);
		return eef_execute_from (about_to_execute_from,
					 &executed_bytes, -1);
	}
	else
	{
		/* init */
		gui_list_stack_reset ();
	}
	if (execution_single_stepped)	/* tracing */
		state = B_STATE_DEBUG;

	gui_editor_set_readonly (app->editor, TRUE);
	gnome_appbar_push (GNOME_APPBAR(appbar), SIM_PROG_RUNNING);
	return eef_execute_from (last_loaded_address, &executed_bytes, -1);
	gnome_appbar_pop (GNOME_APPBAR(appbar));
}

gboolean
b_resume_execution (BTraceMode tmode)
{
	gboolean r;

	ess_trace_mode_out_callret = 0;
	if (state != B_STATE_DEBUG)
	{

		/* start it */
		if (!saved_and_assembled_successfully)
			return FALSE;

		gui_list_stack_reset ();

		execution_single_stepped = TRUE;
		state = B_STATE_DEBUG;
		about_to_execute_from = last_loaded_address;

		ess_trace_mode = tmode;

		gui_editor_set_readonly (app->editor, TRUE);
		gui_editor_set_highlight (app->editor, TRUE);
		bridge_debug_this_line (about_to_execute_from,
					eef_link_get_line_no
					(about_to_execute_from));

		gnome_appbar_push (GNOME_APPBAR(appbar), SIM_PROG_RUNNING);
		return TRUE;
	}

	execution_single_stepped = TRUE;
	ess_trace_mode = tmode;

	gui_editor_set_readonly (app->editor, TRUE);
	gnome_appbar_push (GNOME_APPBAR(appbar), SIM_PROG_RUNNING);
	r = eef_execute_from (about_to_execute_from, &executed_bytes, -1);
	//gnome_appbar_pop (appbar);
	_end_of_exec ();

	return r;
}

/* after this mode is set, execution is traced 
 * after program halt of stop it will automatically switch back
 * to normal mode */
void
b_enter_debug_mode (void)
{
	g_assert (execution_single_stepped == FALSE);

	execution_single_stepped = TRUE;

	_update_gui_state ();

}

/* stop the current debugging */
void
b_debug_stop (void)
{
	execution_single_stepped = FALSE;
	state = B_STATE_IDLE;
	prev_is_call_instr = FALSE;
	ess_trace_mode_out_callret = 0;

	bridge_debug_this_line (0, -1);	/* clear display */
	_update_gui_state ();

	//gnome_appbar_pop (appbar);
	_end_of_exec ();
	gui_editor_set_readonly (app->editor, FALSE);
	gui_editor_set_highlight (app->editor, FALSE);
}

/* toggles at current line number */
void
b_toggle_breakpoint (void)
{
	gui_editor_toggle_mark (app->editor);
}

AsmSource *
b_get_src (void)
{
	return ds_source;
}
