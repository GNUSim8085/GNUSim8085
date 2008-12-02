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

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>


gboolean
on_window_main_delete_event (GtkWidget * widget,
							 GdkEvent * event, gpointer user_data);

G_MODULE_EXPORT void on_new1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_open1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_save1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_save_as1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_quit1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_registers1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_flags1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_io_ports1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_main_memory1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_reset_all1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_assemble1_activate (GtkMenuItem * menuitem, gpointer user_data);


G_MODULE_EXPORT void on_execute1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_step_in1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_step_over1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_step_out1_activate (GtkMenuItem * menuitem, gpointer user_data);


G_MODULE_EXPORT void
on_toggle_breakpoint1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void
on_clear_all_breakpoints1_activate (GtkMenuItem * menuitem,
									gpointer user_data);

G_MODULE_EXPORT void on_stop_execution1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_help_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_8085_manual1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void
on_assembler_reference1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_about1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_main_io_monitor_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_io_set_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_io_spin_changed (GtkEditable * editable, gpointer user_data);

G_MODULE_EXPORT void on_main_io_update_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_mem_spin_changed (GtkEditable * editable, gpointer user_data);

G_MODULE_EXPORT void on_main_mem_update_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_io_spin_changed (GtkEditable * editable, gpointer user_data);

G_MODULE_EXPORT void on_main_io_update_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_mem_spin_changed (GtkEditable * editable, gpointer user_data);

G_MODULE_EXPORT void on_main_mem_update_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_show_listing1_activate (GtkMenuItem * menuitem, gpointer user_data);

G_MODULE_EXPORT void on_listing_save_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_but_to_hex_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_but_to_dec_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_main_entry_dec_activate (GtkEntry * entry, gpointer user_data);

G_MODULE_EXPORT void on_main_entry_hex_activate (GtkEntry * entry, gpointer user_data);

G_MODULE_EXPORT void on_start_but_tutorial_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_start_but_open_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_start_but_close_clicked (GtkButton * button, gpointer user_data);

G_MODULE_EXPORT void on_listing_save_clicked (GtkButton * button, gpointer user_data);
