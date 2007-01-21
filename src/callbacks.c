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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>

#include "callbacks.h"
#include "interface.h"
#include "support-common.h"
#include "gui-app.h"
#include "gui-editor.h"
#include "8085-asm.h"
#include "asm-source.h"
#include "gui-view.h"
#include "bridge.h"
#include "asm-listing.h"
#include "file-op.h"
#include <stdio.h>
#include <stdlib.h>

gint start_addr = 0x4200;

GUIEditor *edit = NULL;
GtkWidget *wind = NULL;
GtkWidget *filew = NULL;

gboolean
on_window_main_delete_event (GtkWidget * widget,
			     GdkEvent * event, gpointer user_data)
{
	gui_app_destroy ();

	return FALSE;
}

gint
conv_hex_to_dec (gchar * hex)
{
	return strtoul ((const char *) hex, NULL, 16);
}

GString *
conv_dec_to_hex (gint dec)
{
	GString *hex;
	hex = g_string_new ("");

	if (dec == 0)
	{
		g_string_append (hex, "0");
		return hex;
	}

	while (dec)
	{
		gint lfbits;
		gchar digit[2] = "X";
		lfbits = dec & 0xF;

		if (lfbits < 10)
			digit[0] = '0' + lfbits;
		else
			digit[0] = 'A' + lfbits - 10;

		g_string_prepend (hex, digit);

		dec >>= 4;
	}

	return hex;
}

void
on_new1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	file_op_editor_new ();
}


void
on_open1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	file_op_editor_open ();
}


void
on_save1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	file_op_editor_save ();
}


void
on_save_as1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	file_op_editor_save_as ();
}


void
on_quit1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_main_quit ();
}


void
on_registers1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	eef_reset_reg ();
	gui_view_update_reg_flag ();
}


void
on_flags1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	eef_reset_flag ();
	gui_view_update_reg_flag ();
}


void
on_io_ports1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	eef_reset_io ();
	gui_view_update_io_mem ();
}


void
on_main_memory1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	eef_reset_mem ();
	gui_view_update_io_mem ();
}


void
on_reset_all1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	eef_reset_all ();
	gui_view_update_reg_flag ();
	gui_view_update_io_mem ();
}


static void
validate_start_addr (void)
{
	gint la;
	GtkEntry *entry;

	entry = GTK_ENTRY (lookup_widget (app->window_main, "main_entry_sa"));
	g_assert (entry);

	/* get load address */
	asm_util_parse_number ((gchar *) gtk_entry_get_text (entry), &la);

	start_addr = la;


}

gboolean asm_error = FALSE;
void
on_assemble1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	/* assemble file */
	gchar *a_text;

	/* la */
	validate_start_addr ();

	/* get text */
	a_text = gui_editor_get_text (app->editor);

	asm_error = b_assemble (a_text, start_addr);
	if (asm_error == FALSE)
		gui_app_show_msg (GTK_MESSAGE_WARNING,
				  _("Program has errors. Check the Message pane."));

	g_free (a_text);

	//disp_list( b_get_src() );
}



void
on_execute1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	if (b_get_state () == B_STATE_IDLE)
		on_assemble1_activate (NULL, NULL);

	if (asm_error == FALSE)
		return;

	if (!b_execute ())
	{
		gui_app_show_msg (GTK_MESSAGE_ERROR,
				  _("Error executing program"));
	}

	/* update */
	gui_view_update_all ();


}


void
on_step_in1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	if (b_get_state () == B_STATE_IDLE)
		on_assemble1_activate (NULL, NULL);

	if (asm_error == FALSE)
		return;
	if (!b_resume_execution (B_TRACE_IN))
	{
		gui_app_show_msg (GTK_MESSAGE_ERROR, _("Error in step in"));
	}
}


void
on_step_over1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	if (b_get_state () == B_STATE_IDLE)
		on_assemble1_activate (NULL, NULL);

	if (asm_error == FALSE)
		return;
	if (!b_resume_execution (B_TRACE_OVER))
	{
		gui_app_show_msg (GTK_MESSAGE_ERROR, _("Error in step over"));
	}
}


void
on_step_out1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	if (b_get_state () == B_STATE_IDLE)
		on_assemble1_activate (NULL, NULL);

	if (asm_error == FALSE)
		return;
	if (!b_resume_execution (B_TRACE_OUT))
	{
		gui_app_show_msg (GTK_MESSAGE_ERROR, _("Error in step out"));
	}
}


void
on_toggle_breakpoint1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	b_toggle_breakpoint ();
}


void
on_clear_all_breakpoints1_activate (GtkMenuItem * menuitem,
				    gpointer user_data)
{
	gui_editor_clear_all_marks (app->editor);
}


void
on_stop_execution1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	b_debug_stop ();
}


void
on_help_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	//TODO: gnome-help
	gtk_widget_show (create_window_start ());
}


void
on_8085_manual1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show (create_window_start ());
}


void
on_assembler_reference1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show (create_window_start ());
}


void
on_about1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	/* create about box */
	create_dialog_about ();
}



void
on_main_io_set_clicked (GtkButton * button, gpointer user_data)
{

}

static void
entry_to_mm (gchar * entry_name, gchar * spin_name, guint8 * base)
{
	GtkSpinButton *sb;
	GtkEntry *entry;
	const gchar *s;
	gint val;

	g_assert (spin_name);
	g_assert (base);

	sb = GTK_SPIN_BUTTON (lookup_widget (app->window_main, spin_name));
	g_assert (sb);

	entry = GTK_ENTRY (lookup_widget (app->window_main, entry_name));
	g_assert (entry);

	s = gtk_entry_get_text (entry);

	if (!asm_util_parse_number ((gchar *) s, &val))
	{
		gui_app_show_msg (GTK_MESSAGE_INFO,
				  _("Enter a valid number within range"));
		return;
	}

	base[(gint) gtk_spin_button_get_value (sb)] = val;
}

static void
spin_to_entry (GtkSpinButton * sb, gchar * entry_name, guint8 * base)
{
	GtkEntry *entry;
	gint val, addr;
	gchar s[10] = "";
	g_assert (base);
	g_assert (sb);
	g_assert (entry_name);
	/* set the value to text box */

	entry = GTK_ENTRY (lookup_widget (app->window_main, entry_name));
	g_assert (entry);

	addr = gtk_spin_button_get_value (GTK_SPIN_BUTTON (sb));
	g_assert (0 <= addr && addr < 65536);
	val = base[addr];

	g_ascii_dtostr (s, 10, val);


	gtk_entry_set_text (entry, s);
}

void
on_main_io_spin_changed (GtkEditable * editable, gpointer user_data)
{
	spin_to_entry (GTK_SPIN_BUTTON (editable), "main_io_entry", sys.io);
}


void
on_main_io_update_clicked (GtkButton * button, gpointer user_data)
{
	entry_to_mm ("main_io_entry", "main_io_spin", sys.io);
}


void
on_main_mem_spin_changed (GtkEditable * editable, gpointer user_data)
{
	spin_to_entry (GTK_SPIN_BUTTON (editable), "main_mem_entry", sys.mem);

}


void
on_main_mem_update_clicked (GtkButton * button, gpointer user_data)
{
	entry_to_mm ("main_mem_entry", "main_mem_spin", sys.mem);
}

void
on_show_listing1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	GString *list;
	GtkWidget *cont;

	/* assemble */
	on_assemble1_activate (NULL, NULL);
	if (asm_error == FALSE)
		return;

	list = asm_listing_generate (b_get_src ());

	//gui_editor_set_text (app->editor, list->str);

	/* show */
	wind = create_window_listing ();
	cont = lookup_widget (wind, "listing_vbox");
	g_assert (cont);
	edit = gui_editor_new ();
	g_assert (edit);
	gui_editor_set_text (edit, list->str);
	gtk_box_pack_end_defaults (GTK_BOX (cont), edit->scroll);
	gtk_window_maximize (GTK_WINDOW (wind));
	gtk_widget_show_all (wind);
	/* TODO clean up of listing window editor on delete event */

	/* clean up */
	g_string_free (list, TRUE);
}

void
on_listing_save_clicked (GtkButton * button, gpointer user_data)
{
	gchar *ltext;
	//i_save ();
	/* FIXME : the save dialog is getting hung in listing window! */
	g_assert (edit);
	ltext = gui_editor_get_text (edit);
	file_op_listing_save (ltext);
}

void
on_main_but_to_hex_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *hex_entry, *dec_entry;
	GString *hex;
	gint dec;

	hex_entry = lookup_widget (app->window_main, "main_entry_hex");
	g_assert (hex_entry);
	dec_entry = lookup_widget (app->window_main, "main_entry_dec");
	g_assert (dec_entry);

	asm_util_parse_number ((gchar *)
			       gtk_entry_get_text (GTK_ENTRY (dec_entry)),
			       &dec);
	hex = conv_dec_to_hex (dec);

	gtk_entry_set_text (GTK_ENTRY (hex_entry), hex->str);
	g_string_free (hex, TRUE);

}


void
on_main_but_to_dec_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *hex_entry, *dec_entry;
	gchar *hex;
	gint dec;
	GString *dec_t;

	hex_entry = lookup_widget (app->window_main, "main_entry_hex");
	g_assert (hex_entry);
	dec_entry = lookup_widget (app->window_main, "main_entry_dec");
	g_assert (dec_entry);

	hex = (gchar *) gtk_entry_get_text (GTK_ENTRY (hex_entry));
	dec = conv_hex_to_dec (hex);

	dec_t = g_string_new ("");
	g_string_printf (dec_t, "%d", dec);
	gtk_entry_set_text (GTK_ENTRY (dec_entry), dec_t->str);
	g_string_free (dec_t, TRUE);

}

void
on_main_entry_dec_activate (GtkEntry * entry, gpointer user_data)
{
	on_main_but_to_hex_clicked (NULL, NULL);
}


void
on_main_entry_hex_activate (GtkEntry * entry, gpointer user_data)
{
	on_main_but_to_dec_clicked (NULL, NULL);
}

void
on_start_but_tutorial_clicked (GtkButton * button, gpointer user_data)
{
	gchar *msg =
		_("Do the following\n\n\
1. Read \"asm_reference.txt\" for assembler tutorial\n\
2. Open \"examples/nqueens.asm\" and simulate it.\n\n\
Usually these files will be found in your system document directory,\n\
which can be one of the following\n\n\
/usr/local/doc/GNUSim8085\n\
/usr/local/share/doc/GNUSim8085\n\
/usr/share/doc/GNUSim8085\n\n\
These files are also available in \"doc\" directory\n\
of GNUSim8085 source tarball (tar.gz file)\n");
	
	gui_app_show_msg (GTK_MESSAGE_INFO, msg);
	on_start_but_close_clicked (button, NULL);
}


void
on_start_but_open_clicked (GtkButton * button, gpointer user_data)
{
	on_open1_activate (NULL, NULL);
	on_start_but_close_clicked (button, NULL);
}


void
on_start_but_close_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *swd;

	swd = lookup_widget (GTK_WIDGET (button), "window_start");
	gtk_widget_destroy (swd);
}
