/*
	Copyright (C) 2006  Sridhar Ratna <srid@nearfar.org>
	
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

#include "gui-list-data.h"
#include "gui-app.h"
#include "support-common.h"
#include "8085.h"
#include "gui-view.h"
#include <gnome.h>

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

static void
create_me (void)
{
	/* create store */
	store = gtk_tree_store_new (N_COLS, G_TYPE_STRING, G_TYPE_STRING,
				    G_TYPE_INT, G_TYPE_STRING);
	g_assert (store);

	/* create view */
	view = GTK_TREE_VIEW(gtk_tree_view_new_with_model (GTK_TREE_MODEL (store)) );
	g_assert (view);

	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW(view), TRUE);
	gtk_widget_show (GTK_WIDGET (view));

	/* add column */
	_add_column (view, C_ADDR, _("Address"));
	_add_column (view, C_NAME, _("Variable"));
	_add_column (view, C_VAL_HEX, _("Value"));
	_add_column (view, C_VAL, _("Value(Decimal)"));
}

void
gui_list_data_attach_me (void)
{
	GtkWidget *cont;

	cont = lookup_widget (app->window_main, "main_data_scroll");
	g_assert (cont);

	create_me ();

	gtk_container_add (GTK_CONTAINER (cont), GTK_WIDGET (view));

}

void
gui_list_data_clear (void)
{
	gtk_tree_store_clear (store);
}

void
gui_list_data_add (guint16 addr, const char *sym_name, guint8 val)
{
	GtkTreeIter iter;
	gchar str[4] = "XXh";
	gchar add_str[5] = "XXXX";
	guint8 s1, s2;
	g_assert (store);
	g_assert (sym_name);

	gtk_tree_store_append (store, &iter, (child_position)?&mark_iter:NULL);

	/* address */
	eef_split16 (addr, &s1, &s2);
	gui_util_gen_hex (s1, add_str, add_str+1);
	gui_util_gen_hex (s2, add_str+2, add_str+3);
	
	/* value */
	gui_util_gen_hex (val, str, str+1);
	
	gtk_tree_store_set (store, &iter, 
		C_ADDR, add_str, 
		C_NAME, sym_name,
		C_VAL, val, 
		C_VAL_HEX, str, -1);

	last_iter = iter;
}

void gui_list_data_child_state (gboolean yes)
{
	child_position = yes;
	if ( yes )
		mark_iter = last_iter;
}
