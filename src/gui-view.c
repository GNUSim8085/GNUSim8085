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

#include "gui-view.h"
#include "gui-app.h"
#include "asm-source.h"
#include "gui-list-data.h"
#include "gui-list-stack.h"
#include "asm-ds-symtable.h"

typedef struct
{
	gpointer data;
	gchar *glade;
	gpointer extra;

} GUIViewTable;

/* reg, glade */
static GUIViewTable reg_flag[] = {
	{&sys.reg.a, "main_reg_a", NULL},
	{&sys.reg.b, "main_reg_b", NULL},
	{&sys.reg.c, "main_reg_c", NULL},
	{&sys.reg.d, "main_reg_d", NULL},
	{&sys.reg.e, "main_reg_e", NULL},
	{&sys.reg.h, "main_reg_h", NULL},
	{&sys.reg.l, "main_reg_l", NULL},
	{&sys.reg.pswh, "main_reg_pswh", NULL},
	{&sys.reg.pswl, "main_reg_pswl", NULL},
	{&sys.reg.pch, "main_reg_pch", NULL},
	{&sys.reg.pcl, "main_reg_pcl", NULL},
	{&sys.reg.sph, "main_reg_sph", NULL},
	{&sys.reg.spl, "main_reg_spl", NULL},
	{&sys.reg.int_reg, "main_reg_int_reg", NULL},
	{NULL, NULL, NULL},
	
	/* flags */
	{ (gpointer)'S', "main_flag_s", NULL },
	{ (gpointer)'Z', "main_flag_z", NULL },
	{ (gpointer)'A', "main_flag_ac", NULL },
	{ (gpointer)'P', "main_flag_p", NULL },
	{ (gpointer)'C', "main_flag_c", NULL },
	{NULL, NULL, NULL}
};

static gboolean
_get_flag (gchar c)
{
	switch (c)
	{
		case 'S':
			return sys.flag.s;
		case 'Z':
			return sys.flag.z;
		case 'A':
			return sys.flag.ac;
		case 'P':
			return sys.flag.p;
		case 'C':
			return sys.flag.c;
	}
	g_assert_not_reached();
	return FALSE;
}

/* spin, glade, areamem */
static GUIViewTable io_mem[] = {
	{"main_io_spin", "main_io_entry", sys.io},
	{"main_mem_spin", "main_mem_entry", sys.mem},
	{NULL, NULL, NULL}
};

static gchar
gui_util_hex_char (guint8 val)
{
	
	/**/
	
	if (val < 10)
		return val + 48;

	else
		return 'A' + (val - 10);
}

void
gui_util_gen_hex (guint8 val, gchar * a, gchar * b)
{
	guint8 x = 0, y = 0;
	g_assert (a);
	g_assert (b);

	x = val;
	x >>= 4;

	y = val;
	y <<= 4;
	y >>= 4;

	*a = gui_util_hex_char (x);
	*b = gui_util_hex_char (y);
}

void
gui_view_update_io_mem (void)
{
	GUIViewTable *ptr;
	GtkSpinButton *spin;
	GtkEntry *entry;
	gint val;
	gchar str[3] = "XX";
	
	ptr = io_mem;
	while (ptr->data)
	{
		spin = GTK_SPIN_BUTTON(lookup_widget (app->window_main, ptr->data));
		g_assert (spin);
		entry = GTK_ENTRY(lookup_widget (app->window_main, ptr->glade));
		g_assert (entry);
		val = gtk_spin_button_get_value_as_int (spin);
		
		gui_util_gen_hex (*( (eef_data_t *)(ptr->extra) + val), str, str+1);
		
		gtk_entry_set_text (entry, str);
		ptr++;
	}
	
}

void
gui_add_me_to_data_list(guint16 addr, gchar *name, gint cnt)
{
	guint8 val;
	gchar *t;
	
	if ( cnt != -1 )
		t = g_strdup_printf ("%s + %d", name, cnt);
	else 
		t = g_strdup (name);
	
	
	if ( cnt != -1 )
		addr += cnt;
	
	val = sys.mem[addr];
	gui_list_data_add (addr, t, val);
	
	g_free (t);
}

void
gui_for_each_in_hash(gchar *name, AsmSymEntry *entry, gpointer data)
{
	/* add */
	gint cnt = -1;
	
	if ( entry->type != ASM_SYM_VARIABLE )
		return ; /* no thanx */
	
	
	if ( entry->no_of_data == 1 )
		gui_add_me_to_data_list (GPOINTER_TO_UINT(entry->data), entry->name, -1);
	else
	{
		gui_add_me_to_data_list (GPOINTER_TO_UINT(entry->data), entry->name, -1);
		gui_list_data_child_state (TRUE);
		for ( cnt=0; cnt < entry->no_of_data; cnt++ )
			gui_add_me_to_data_list (GPOINTER_TO_UINT(entry->data), entry->name, cnt);
		gui_list_data_child_state (FALSE);
	}
	
}

static void
gui_view_update_data(void)
{
	gui_list_data_clear();
	
	/* add */
	asm_sym_scan ( (GHFunc)gui_for_each_in_hash, NULL);
}

static void
gui_view_update_stack(void)
{
	gui_list_stack_clear();
	
	/* add */
	gui_list_stack_reload ();
}

void gui_view_update_reg_flag (void)
{
	GUIViewTable *ptr;
	/* update reg_flag */
	ptr = &reg_flag[0];
	while (ptr->data)
	{
		GtkLabel *label;
		gchar str[3] = "XX";

		label = GTK_LABEL(lookup_widget (app->window_main, ptr->glade));
		g_assert (label);

		gui_util_gen_hex (*((eef_data_t *) (ptr->data)), str,
				  str + 1);

		/* put in label */
		gtk_label_set_text (label, str);

		ptr++;
	}
	/* update flag */
	ptr++;
	while (ptr->data)
	{
		GtkLabel *label;
		gchar str[2] = "0";

		label = GTK_LABEL(lookup_widget (app->window_main, ptr->glade));
		g_assert (label);

		if ( _get_flag (GPOINTER_TO_INT(ptr->data)) )
			str[0] = '1';
		
		/* put in label */
		gtk_label_set_text (label, str);

		ptr++;
	}
}

void
gui_view_update_all (void)
{
	/* update reg, flag labels */
	gui_view_update_reg_flag();

	/* update io_mem text entries */
	gui_view_update_io_mem ();
	
	/* update data, stack lists */
	gui_view_update_data ();
	gui_view_update_stack ();
}
