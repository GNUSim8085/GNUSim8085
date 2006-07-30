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

#include "gui-app.h"

GUIApp *app = NULL;

GtkWidget *container = NULL;

void
gui_app_new (void)
{
	GtkPaned *p_msg, *p_data;
	/*GdkScreen *screen;*/
	g_assert (app == NULL);
	app = g_malloc0 (sizeof (GUIApp));

	app->window_main = create_window_main ();
	gtk_window_maximize (GTK_WINDOW (app->window_main));

	app->editor = gui_editor_new ();
	gui_editor_show (app->editor);

	/* add editor */
	container = lookup_widget (app->window_main, "main_vbox_center");
	g_assert (container);

	gtk_box_pack_start_defaults (GTK_BOX (container),
				     app->editor->widget);

	/* dim */
	//gtk_window_get_size(app->window_main, &w, &h);

	p_msg = GTK_PANED (lookup_widget
			   (app->window_main, "main_vpaned_msg"));
	g_assert (p_msg);
	p_data = GTK_PANED (lookup_widget
			    (app->window_main, "main_hpaned_data"));
	g_assert (p_data);

	/* get screen 
	screen = gdk_screen_get_default ();
	w = gdk_screen_get_width (screen);
	h = gdk_screen_get_height (screen);

	g_message ("screen w = %d", w);
	g_message ("screen h = %d", h);*/

	/* maximize window FIXME - not working in other wms say IceWM */
	gtk_window_maximize (GTK_WINDOW (app->window_main));

	//FIXME properly allocate spaces to widgets?
	//gtk_paned_set_position (p_data, w);
	//gtk_paned_set_position (p_msg, h/2);
}

void
gui_app_show (void)
{
	gtk_widget_show (app->window_main);
}

void
gui_app_destroy (void)
{
	/* first hide */
	gtk_widget_hide (app->window_main);

	/* destroy */
	gtk_widget_destroy (app->window_main);

	/* quit main */
	gtk_main_quit ();
}

void
gui_app_show_msg (GtkMessageType type, gchar *msg)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new (GTK_WINDOW(app->window_main),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 type, 
					 GTK_BUTTONS_OK,
					 msg);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}
