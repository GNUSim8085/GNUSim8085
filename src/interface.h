/*
  Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
        
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget* create_window_main (void);
void create_dialog_about (void);
GtkWidget* create_window_listing (void);
GtkWidget* create_window_start (GtkWindow *);
GtkWidget* create_window_tutorial (void);
GtkWidget* create_dialog_isymbol (void);
GtkWidget* create_dialog_ireg (void);

#if GTK_CHECK_VERSION (3, 0, 0)
#define HBOX(spacing) \
gtk_box_new (GTK_ORIENTATION_HORIZONTAL, spacing)
#define VBOX(spacing) \
gtk_box_new (GTK_ORIENTATION_VERTICAL, spacing)
#else
#define HBOX(spacing) \
gtk_hbox_new (FALSE, spacing)
#define VBOX(spacing) \
gtk_vbox_new (FALSE, spacing)
#endif
