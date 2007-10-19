/*
  Copyright (C) 2003  Sridhar Ratnakumar <srid@nearfar.org>
	
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

#include "gui-input-symbol.h"
#include "support-common.h"
#include "interface.h"
#include "asm-ds-symtable.h"

/* DON't directly use these variables. see the 2 funcs below */
static GtkWidget *combo_macros = NULL, *combo_variables =
  NULL, *combo_labels = NULL;
static GList *list_macros = NULL, *list_variables = NULL, *list_labels = NULL;

static GtkWidget *cdialog = NULL;

static void
combo_vars_validate (GtkWidget * dig)
{
  /* validate widget vars */
  combo_macros = lookup_widget (GTK_WIDGET (dig), "isymbol_macros");
  g_assert (combo_macros);
  combo_variables =
	lookup_widget (GTK_WIDGET (dig), "isymbol_variables");
  g_assert (combo_macros);
  combo_labels = lookup_widget (GTK_WIDGET (dig), "isymbol_labels");
  g_assert (combo_macros);

  /* validate lists */
  list_macros = list_variables = list_labels = NULL;
}

static void
combo_vars_invalidate (gboolean only_combo)
{
  /* widget */
  combo_macros = combo_variables = combo_labels = NULL;

  if (only_combo)
	return;

  /* lists */
  if (list_macros)
	g_list_free (list_macros);
  if (list_variables)
	g_list_free (list_variables);
  if (list_labels)
	g_list_free (list_labels);

}

static void
combo_vars_flush (void)
{
  if (list_macros)
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_macros),
								   list_macros);
  if (list_variables)
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_variables),
								   list_variables);
  if (list_labels)
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_labels),
								   list_labels);
}

static void
for_each_symbol (gchar * name, AsmSymEntry * entry, gpointer dialog)
{
  GtkWidget *dig;
  GList **lst = NULL;

  g_assert (entry);
  g_assert (dialog);

  dig = GTK_WIDGET (dialog);

  /* select combo to insert */
  switch (entry->type)
	{
	case ASM_SYM_VARIABLE:
	  lst = &list_variables;
	  break;
	case ASM_SYM_LABEL:
	  lst = &list_labels;
	  break;
	case ASM_SYM_MACRO:
	  lst = &list_macros;
	  break;
	default:
	  return;
	}

  *lst = g_list_insert (*lst, (gpointer) entry->name, -1);
}

/* fills dialog with symbols from current module */
static void
_fill_symbols (GtkWidget * dig)
{
  combo_vars_validate (dig);
  asm_sym_scan ((GHFunc) for_each_symbol, (gpointer) dig);
  combo_vars_flush ();
  combo_vars_invalidate (FALSE);
}

static void
_connect_cb_callback (GtkWidget * list, GtkEntry * centry)
{
  /* set entry text */
  GtkWidget *entry;

  entry = lookup_widget (cdialog, "entry1");
  g_assert (entry);

  gtk_entry_set_text (GTK_ENTRY (entry),
					  gtk_entry_get_text (GTK_ENTRY (centry)));
}

static void
_connect_cb (GtkWidget * dig)
{
  g_assert (dig);
  combo_vars_validate (dig);

  /* connect signal */
#define CBCON(comb) g_signal_connect ((gpointer) (GTK_COMBO (comb)->list), \
									  "selection-changed",				\
									  (GCallback) _connect_cb_callback, \
									  (gpointer) (GTK_COMBO (comb)->entry));

  CBCON (combo_macros);
  CBCON (combo_labels);
  CBCON (combo_variables);

#undef CBCON

  combo_vars_invalidate (TRUE);
}

gchar *
gui_input_symbol (void)
{
  GtkWidget *dig;
  gchar *symbol_name = NULL;

  cdialog = dig = create_dialog_isymbol ();
  g_assert (dig);

  _fill_symbols (dig);

  _connect_cb (dig);

  if (gtk_dialog_run (GTK_DIALOG (dig)) == GTK_RESPONSE_OK)
	{
	  GtkWidget *entry;

	  entry = lookup_widget (dig, "entry1");
	  g_assert (entry);

	  symbol_name =
		g_strdup (gtk_entry_get_text (GTK_ENTRY (entry)));

	}

  gtk_widget_destroy (GTK_WIDGET (dig));

  cdialog = NULL;
  return symbol_name;
}

/* Now Input Register widget --- */
gchar *
gui_input_reg (gchar * set)
{
  gint i = 0;
  gchar psw[] = "PSW", sp[] = "SP", a[] = "A";	/* yes. this is uglier (C) */
  gchar b[] = "B", c[] = "C", d[] = "D", e[] = "E";
  gchar h[] = "H", l[] = "L", m[] = "M";
  gchar d1[] = "1", d2[] = "2", d3[] = "3", d4[] = "4";
  gchar d5[] = "5", d6[] = "6", d7[] = "7", d0[] = "0";
  GtkWidget *combo_reg;
  GtkWidget *dig;
  GList *items = NULL;
  gchar *symbol_name = NULL;

  g_assert (set);

  dig = create_dialog_ireg ();
  g_assert (dig);
  combo_reg = lookup_widget (GTK_WIDGET (dig), "ireg");
  g_assert (combo_reg);
	
  while (set[i])
	{
	  gchar *to = NULL;
	  if (set[i] == 'P')
		to = psw;
	  else if (set[i] == 'S')
		to = sp;
	  else
		{
		  switch (set[i])
			{
			case 'A': to = a; break;
			case 'B': to = b; break;
			case 'C': to = c; break;
			case 'D': to = d; break;
			case 'E': to = e; break;
			case 'H': to = h; break;
			case 'L': to = l; break;
			case 'M': to = m; break;
			case '0': to = d0; break;
			case '1': to = d1; break;
			case '2': to = d2; break;
			case '3': to = d3; break;
			case '4': to = d4; break;
			case '5': to = d5; break;
			case '6': to = d6; break;
			case '7': to = d7; break;
			default: g_assert_not_reached();
			}
		}
	  i++;

	  items = g_list_append (items, to);
		
	}
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_reg), items);
	
  if (gtk_dialog_run (GTK_DIALOG (dig)) == GTK_RESPONSE_OK)
	{
	  GtkWidget *entry;

	  entry = lookup_widget (dig, "ireg_entry");
	  g_assert (entry);

	  symbol_name =
		g_strdup (gtk_entry_get_text (GTK_ENTRY (entry)));

	}

  gtk_widget_destroy (GTK_WIDGET (dig));
  g_list_free (items);
	
  return symbol_name;
}
