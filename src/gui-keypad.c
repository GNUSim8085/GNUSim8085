/*
  Copyright (C) 2003  Sridhar Ratnakumar <sridhar.ratna@gmail.com>
	
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
  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "gui-keypad.h"
#include "asm-id.h"
#include "gui-app.h"
#include "gui-input-symbol.h"
#include "bridge.h"

static GtkWidget *table;
#define TABLE_ROWS 21
#define TABLE_COLS 4

IdOpcode *p_opcode;
IdPseudo *p_pseudo;

/* Due to the overall unknown bug, I need this 
   Call this of you want to use the globals */
static void
validate_globals()
{
  p_opcode = asm_id_get_opcode (); /* make sure */
  p_pseudo = asm_id_get_pseudo ();
}

/* helper: return string containing registers that is used by opcode */
gchar *
get_register (IdOpcode * opcode)
{
  gchar *ret;
  gint tot;
  gint i = 0;
  gint index;
  g_assert(opcode);
	
  validate_globals();
  if (!g_ascii_strcasecmp ("MOV", opcode->op_str))
	{
	  ret = g_strdup ("ABCDEHLM");
	  return ret;
	}

  ret = g_malloc (15);
  for (i = 0; i < 15; i++)
	ret[i] = 0;

  tot = asm_id_total_opcodes ();
  index = -1;
  for (i = 0; i < tot; i++)
	{
	  if (opcode == &p_opcode[i])
		{
		  index = i;
		  break;
		}
	}
  g_assert (index != -1);

  i = 0;
  while (!g_ascii_strcasecmp (opcode->op_str, p_opcode[index].op_str))
	{
	  ret[i++] = p_opcode[index].arg1[0];
	  index--;
	}
  ret[i] = '\0';

  return ret;
}

/* callback for Opcode Button click */
static void
cb_clicked (GtkButton * button, gpointer user_data)
{
  if (b_get_state() != B_STATE_DEBUG) {
  IdOpcode *opdata = (IdOpcode *) user_data;
  GString *line_buf;
  gchar *re, *re2;
  gchar *newop;

  g_assert (opdata);
	
  line_buf = g_string_new ("");
  newop = g_ascii_strdown (opdata->op_str, -1);
  g_string_append_printf (line_buf, "%s ", newop);
  g_free (newop);

  if (opdata->num_args > 0)
	{
	  re = get_register (opdata);
	  re2 = gui_input_reg (re);
	  if ( !re2 ) goto done;
	  newop = g_ascii_strdown (re2, -1);
	  g_string_append_printf (line_buf, " %s", newop);
	  g_free (newop);
	  g_free (re2);
	  if (opdata->num_args > 1)
		{
		  re2 = gui_input_reg (re);
		  if ( !re2 ) goto done;
		  newop = g_ascii_strdown (re2, -1);
		  g_string_append_printf (line_buf, ", %s", newop);
		  g_free (newop);
		  g_free (re2);
		}
	  g_free (re);
	}
  if (opdata->user_args)
	{
	  re = gui_input_symbol ();
	  if ( !re ) goto done;
	  if (opdata->num_args > 0)
		g_string_append_printf (line_buf, ", %s", re);
	  else
		g_string_append_printf (line_buf, " %s", re);
	  g_free (re);
	}

  g_string_append (line_buf, "\n");

  /* Insert into editor */
  {
	gint old_no = gui_editor_get_line (app->editor);
	gui_editor_insert (app->editor, line_buf->str);
	gui_editor_goto_line (app->editor, old_no + 1);
  }

 done:
  /* set focus */
  gui_editor_grab_focus (app->editor);
  }
}

/* a generator for distinct opcodes, die after end */
IdOpcode *
get_next_opcode (void)
{
  static int valid = 1;
  static int next = 0;
  gchar save[5];
  IdOpcode *opcode, *prev = NULL;
  g_assert (valid);

  if (next == asm_id_total_opcodes ())
	{
	  valid = 0;
	  return NULL;
	}
  opcode = &p_opcode[next++];
  g_stpcpy (save, opcode->op_str);
  while (next < asm_id_total_opcodes () + 1
		 && !g_ascii_strcasecmp (save, opcode->op_str))
	{
	  prev = opcode;
	  opcode = &p_opcode[next++];
	}
  next--;

  return prev;
}


static void
cb_focus ()
{
  /* set focus */
  gui_editor_grab_focus (app->editor);
}


static void
create_me (void)
{
  int i, j;
  IdOpcode *opcode;
  opcode = asm_id_get_opcode ();

  /* Construct a table of cells */
  table = gtk_table_new (TABLE_ROWS, TABLE_COLS, TRUE);
	
  g_signal_connect (table, "focus", (GCallback) cb_focus, NULL);
	
  p_opcode = asm_id_get_opcode ();
  p_pseudo = asm_id_get_pseudo ();
  for (i = 0; i < TABLE_ROWS; i++)
	{
	  for (j = 0; j < TABLE_COLS; j++)
		{
		  GtkWidget *button;
		  IdOpcode *op;

		  /* get opcode name */
		  op = get_next_opcode ();
		  if (!op)
			goto breakout;

		  button = gtk_button_new_with_label (op->op_str);
		  gtk_widget_set_tooltip_text (button, op->op_desc);
		  g_signal_connect (button, "clicked",
							(GCallback) cb_clicked, op);
		  gtk_table_attach_defaults (GTK_TABLE (table), button,
									 j, j + 1, i, i + 1);
		}
	}
 breakout:
  return;

}

void
gui_keypad_attach_me (void)
{
  GtkWidget *cont;

  cont = lookup_widget (app->window_main, "main_keypad_scroll");
  g_assert (cont);

  create_me ();

  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (cont),
										 GTK_WIDGET (table));

  gtk_widget_show_all (GTK_WIDGET (cont));
}
