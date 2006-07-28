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

/*
 * GUI - Main application window and stuffs
 *
 * R. Sridhar
 */

#ifndef __GUI_APP_H__
#define __GUI_APP_H__

#include <gtk/gtk.h>
#include "interface.h"

/* widgets */
#include "gui-editor.h"
#include "support-common.h"

G_BEGIN_DECLS typedef struct
{
	GtkWidget *window_main;
	GUIEditor *editor;

} GUIApp;

void gui_app_new (void);
void gui_app_destroy (void);
void gui_app_show (void);

extern GUIApp *app;

void gui_app_show_msg (GtkMessageType type, gchar * msg);

G_END_DECLS
#endif /* __GUI_APP_H__ */
