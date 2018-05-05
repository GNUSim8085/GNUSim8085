/*
  Copyright (C) 2010  Debajit Biswas <http://launchpad.net/~debjitbis08>
	
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

#include "config.h"
#include "gui-list-memory.h"

static GtkTreeStore *store = NULL;
static GtkTreeView *view = NULL;
static gboolean child_position = FALSE;
static GtkTreeIter last_iter;
static GtkTreeIter mark_iter;
static eef_addr_t start = 0;

enum
{
  C_ADDR_HEX,
  C_ADDR,
  C_DATA,
  N_COLS
};

void
on_mem_list_data_edited (GtkCellRendererText *renderer, gchar *path,
                         gchar *new_text, gpointer user_data)
{
  GtkTreeIter iter;
  gtk_tree_model_get_iter_from_string (GTK_TREE_MODEL (store), &iter, path);
	
  gint value;
  gint addr;
  if(!asm_util_parse_number (new_text, &value))
    value = 0;

  gtk_tree_store_set (store, &iter, C_DATA, value, -1);
  gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, C_ADDR, &addr, -1);
  sys.mem[addr] = value;
}

static void
_add_column (GtkTreeView * view, gint id, gchar * title)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  g_assert (view);

  renderer = gtk_cell_renderer_text_new ();
  
  if ( !strcmp(title, _("Data")) )
  {
    g_object_set ((gpointer) renderer, "editable", TRUE, NULL);
    g_signal_connect ((gpointer) renderer, "edited",
                    G_CALLBACK (on_mem_list_data_edited),
                    NULL);
  }
  	
  column = gtk_tree_view_column_new_with_attributes (title, renderer,
                                                     "text", id, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);
}

static void
create_me (void)
{
  /* create store */
  store = gtk_tree_store_new (N_COLS,
                              G_TYPE_STRING,
                              G_TYPE_INT,
                              G_TYPE_INT);
  g_assert (store);

  /* create view */
  view = GTK_TREE_VIEW (gtk_tree_view_new_with_model (GTK_TREE_MODEL (store)));
  g_assert (view);

  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);
  gtk_widget_show (GTK_WIDGET (view));

  /* add column */
  _add_column (view, C_ADDR_HEX, g_strconcat(_("Address"), " (", _("Hex"), ")", NULL));
  _add_column (view, C_ADDR, _("Address"));
  _add_column (view, C_DATA, _("Data"));

}

void
gui_list_memory_attach_me (void)
{
  GtkWidget *cont;

  cont = lookup_widget (app->window_main, "main_memory_scroll");
  g_assert (cont);

  create_me ();

  gtk_container_add (GTK_CONTAINER (cont), GTK_WIDGET (view));

}

void
gui_list_memory_clear (void)
{
  gtk_tree_store_clear (store);
}

void
gui_list_memory_child_state (gboolean state)
{
  child_position = state;
  if (state)
    mark_iter = last_iter;
}

void
gui_list_memory_set_start (gint st)
{
  if ( st >= 0 && st < 65536 )
  	start = st;
}

static void
gui_list_memory_add (eef_addr_t addr, eef_data_t value)
{
  GtkTreeIter iter;
  gchar addr_str[5] = "XXXX";
  guint8 s1, s2;
  g_assert (store);
	
  gtk_tree_store_append (store, &iter, NULL);
	
  /* address */
  eef_split16 (addr, &s1, &s2);
  gui_util_gen_hex (s1, addr_str, addr_str+1);
  gui_util_gen_hex (s2, addr_str+2, addr_str+3);

  gtk_tree_store_set (store, &iter,
                      C_ADDR_HEX, addr_str,
                      C_ADDR, addr,
                      C_DATA, value,
                      -1);

  last_iter = iter;
}

void
gui_list_memory_initialize (void)
{
  eef_addr_t i = 0;
  for( i = 0; i < MAX_LIST_LEN && i+start <= EEF_ADDR_T_MAX ; i++)
  {
    gui_list_memory_add (i+start, sys.mem[i + start]);
  }
}

void
gui_list_memory_update (void)
{
  gui_list_memory_clear();
  gui_list_memory_initialize();
}

void
gui_list_memory_update_single (eef_addr_t addr)
{
  if (addr >= start && addr < MAX(start + MAX_LIST_LEN, EEF_ADDR_T_MAX))
  {
    GtkTreeIter iter;
    g_assert(gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter));
		 
    gint pos = addr -start;
    gint n = 0;
    while ( n++ < pos )
      gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter);

    gtk_tree_store_set (store, &iter, C_DATA, sys.mem[addr], -1);
  }
}
