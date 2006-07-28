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

#include "gui-list-message.h"
#include "gui-app.h"
#include "support-common.h"
#include <gnome.h>

static GtkListStore *store = NULL;
static GtkTreeView *view = NULL;

enum
{
	C_NO,
	C_MESG,
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
cb_clicked (GtkTreeView * listview, gpointer user_data)
{
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	gint ln;

	g_assert (view);

	/* get selected */
	selection = gtk_tree_view_get_selection (view);
	g_assert (selection);
	g_assert (gtk_tree_selection_get_selected
		  (selection, (GtkTreeModel **)&store, &iter));

	/* get */ 
	gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, C_NO, &ln, -1);
	if (ln == 0)
		return;

	/* FIXME line number is not correct some times at which ln-1 will work */
	gui_editor_goto_line (app->editor, ln /* -1 ? */);
	gtk_widget_grab_focus (app->editor->widget);
}

static void
create_me (void)
{
	/* create store */
	store = gtk_list_store_new (N_COLS, G_TYPE_INT, G_TYPE_STRING);
	g_assert (store);

	/* create view */
	view = GTK_TREE_VIEW (gtk_tree_view_new_with_model
			      (GTK_TREE_MODEL (store)));
	g_assert (view);

	gtk_widget_show (GTK_WIDGET (view));

	/* add column */
	_add_column (view, C_NO, _("Line No"));
	_add_column (view, C_MESG, _("Assembler Message"));

	/* connect signals */
	g_signal_connect (view, "cursor-changed", (GCallback) cb_clicked,
			  NULL);
}

void
gui_list_message_attach_me (void)
{
	GtkWidget *cont;

	cont = lookup_widget (app->window_main, "main_vpaned_msg");
	g_assert (cont);

	create_me ();

	gtk_paned_add2 (GTK_PANED (cont), GTK_WIDGET (view));

}

void
gui_list_message_clear (void)
{
	gtk_list_store_clear (store);
}

void
gui_list_message_add (const char *msg, gint ln, gint attr)
{
	GtkTreeIter iter;
	g_assert (store);
	g_assert (msg);

	gtk_list_store_append (store, &iter);

	gtk_list_store_set (store, &iter, C_NO, ln, C_MESG, msg, -1);

}
