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
#include <config.h>
#endif

#include <libintl.h>
#include "interface.h"
#include "gui-app.h"
#include "gui-list-message.h"
#include "asm-ds-symtable.h"
#include "gui-list-data.h"
#include "gui-list-stack.h"
#include "gui-keypad.h"
#include "bridge.h"
#include "callbacks.h"
#include "file-op.h"

int
main (int argc, char *argv[])
{
  GtkWidget *statusbar;

  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  gtk_init (&argc, &argv);

  /* create app */
  gui_app_new ();

  asm_sym_init ();

  /* add extra */
  gui_list_message_attach_me ();
  gui_list_data_attach_me ();
  gui_list_stack_attach_me ();
  gui_keypad_attach_me ();
  b_init ();

  /* Start with NEW file */
  on_new1_activate (NULL, NULL);

  /* show start with dialog */
  gui_app_show ();

  statusbar = lookup_widget (app->window_main, "main_statusbar");
  g_assert (statusbar);

  gtk_statusbar_push (GTK_STATUSBAR (statusbar), gtk_statusbar_get_context_id (GTK_STATUSBAR(statusbar), "Simulator"), _("Simulator: Idle"));

  /* hook up url handler */
  gtk_about_dialog_set_url_hook (activate_url, NULL, NULL);

  /* open file specified in command line if any */
  if (argc > 1)
	ori_open (argv[1], TRUE);
  else
	gtk_widget_show (create_window_start ());

  gtk_main ();

  return 0;
}
