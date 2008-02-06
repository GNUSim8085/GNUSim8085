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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gui-list-stack.h"
#include "gui-app.h"
#include "gui-view.h"
#include "8085.h"
#include "asm-ds-limits.h"
#include "asm-ds-symtable.h"

static GtkTreeStore *store = NULL;
static GtkTreeView *view = NULL;
static gboolean child_position = FALSE;
static GtkTreeIter last_iter;
static GtkTreeIter mark_iter;

enum
  {
	C_ADDR,
	C_NAME,
	C_VAL,
	C_VAL_HEX,
	N_COLS
  };

static void
_add_column (GtkTreeView * view, gint id, gchar * title)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  g_assert (view);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes (title,
													 renderer,
													 "text", id, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);
}

void stack_modified (eef_addr_t sp, gboolean pushed, gchar what);

static void
create_me (void)
{
  /* create store */
  store = gtk_tree_store_new (N_COLS, G_TYPE_STRING, G_TYPE_STRING,
							  G_TYPE_INT, G_TYPE_STRING);
  g_assert (store);

  /* create view */
  view = GTK_TREE_VIEW (gtk_tree_view_new_with_model
						(GTK_TREE_MODEL (store)));
  g_assert (view);

  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);
  gtk_widget_show (GTK_WIDGET (view));

  /* add column */
  _add_column (view, C_ADDR, _("Stack Loc"));
  _add_column (view, C_NAME, _("Proc/Reg"));
  _add_column (view, C_VAL_HEX, _("Value"));
  _add_column (view, C_VAL, _("Value(Decimal)"));
}

void
gui_list_stack_attach_me (void)
{
  GtkWidget *cont;

  cont = lookup_widget (app->window_main, "main_stack_scroll");
  g_assert (cont);

  create_me ();

  gtk_container_add (GTK_CONTAINER (cont), GTK_WIDGET (view));

  /* setup callback */
  eef_set_stack_callback (stack_modified);
}

static eef_addr_t stack_addr[65536];	/* addr of pushed data locations */
static gchar stack_type[65536];	/* stack_type[i] contains type of [i] */
static int stack_addr_top = -1;

void
stack_modified (eef_addr_t sp, gboolean pushed, gchar what)
{
  if (pushed)
	{
	  /* check if stack size exceeds */
	  if (stack_addr_top == 65536 - 1)
		{
		  gui_app_show_msg (GTK_MESSAGE_WARNING,
							_
							("Stack size exceeded.  Stop the execution"));
		  return;
		}
	  /* push to stack */
	  stack_addr[++stack_addr_top] = sp;
	  stack_type[stack_addr[stack_addr_top]] = what;
	}
  else
	{
	  /* check of illegal pop */
	  if (stack_addr_top == -1)
		{
		  gui_app_show_msg (GTK_MESSAGE_WARNING,
							_
							("More number of POP are executed than PUSH. Stop the execution and check the logic of your program"));
		  return;
		}
	  /* pop */
	  stack_addr_top--;
	}
}

void
gui_list_stack_clear (void)
{
  /* clear view */
  gtk_tree_store_clear (store);

  child_position = FALSE;
}

void
gui_list_stack_reset (void)
{
  /* reset stack */
  stack_addr_top = -1;
}

void
gui_list_stack_add (guint16 addr, const char *sym_name, guint16 val)
{
  GtkTreeIter iter;
  gchar str[] = "XXXXh";
  gchar add_str[5] = "XXXX";
  guint8 s1, s2;
  g_assert (store);
  g_assert (sym_name);

  gtk_tree_store_append (store, &iter,
						 (child_position) ? &mark_iter : NULL);

  /* address */
  eef_split16 (addr, &s1, &s2);
  gui_util_gen_hex (s1, add_str, add_str + 1);
  gui_util_gen_hex (s2, add_str + 2, add_str + 3);

  /* value */
  eef_split16 (val, &s1, &s2);
  gui_util_gen_hex (s1, str, str + 1);
  gui_util_gen_hex (s2, str + 2, str + 3);

  gtk_tree_store_set (store, &iter,
					  C_ADDR, add_str,
					  C_NAME, sym_name, C_VAL, val, C_VAL_HEX, str, -1);

  last_iter = iter;

}

void
gui_list_stack_child_state (gboolean yes)
{
  child_position = yes;
  if (yes)
	mark_iter = last_iter;
}

void
gui_list_stack_reload (void)
{
  int i;
  gboolean prev_is_p = FALSE;

  /* for each stack content add it to list */
  for (i = 0; i <= stack_addr_top; ++i)
	{
	  gchar symbol[ASM_DS_MAX_IDENTIFIER_LENGTH] = "";
	  eef_addr_t sp = stack_addr[i];
	  gchar what = stack_type[sp];
	  eef_addr_t mem16 = eef_mem16_get (sp);

	  /* setup symbol name */
	  if (what == 'P')
		{
		  /* procedure address! */
		  eef_addr_t called_addr = eef_mem16_get (mem16 - 2);	/* assuming call instr. */

		  /* find proc name */

		  g_snprintf (symbol, ASM_DS_MAX_IDENTIFIER_LENGTH - 1,
					  "%s", asm_sym_symbol_at (called_addr,
											   ASM_SYM_LABEL));

		  gui_list_stack_child_state (FALSE);
		  prev_is_p = TRUE;
		}
	  else
		{
		  /* Register Pair contents */
		  g_snprintf (symbol, ASM_DS_MAX_IDENTIFIER_LENGTH - 1,
					  "[%c%c]", what,
					  eef_regpair_get_another (what));

		  if (prev_is_p)
			gui_list_stack_child_state (TRUE);
		  prev_is_p = FALSE;
		}

	  /* create children instead parent if approriate */
	  /*if (what != 'P')
	   * gui_list_stack_child_state (TRUE);
	   * else if (child_position)
	   * gui_list_stack_child_state (FALSE); */

	  /* add to view */
	  gui_list_stack_add (sp, symbol, mem16);
	}
}
