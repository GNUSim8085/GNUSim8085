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

#include "callbacks.h"
#include "config.h"
#include "interface.h"
#include "gui-app.h"
#include "gui-editor.h"
#include "8085-asm.h"
#include "asm-source.h"
#include "gui-view.h"
#include "bridge.h"
#include "asm-listing.h"
#include "file-op.h"
#include "gui-list-memory.h"
#include "gui-list-io.h"

#define DEFAULT_LOAD_ADDR 0x4200

gint start_addr = 0x4200;

GUIEditor *edit = NULL;
GtkWidget *wind = NULL;
GtkWidget *tutorial = NULL;
GtkWidget *filew = NULL;

gboolean
on_window_main_delete_event (GtkWidget * widget,
							 GdkEvent * event, gpointer user_data)
{
  if (gtk_text_buffer_get_modified ((GtkTextBuffer *)app->editor->buffer))
	{
	  if (!file_op_confirm_save())
		return TRUE;
	}
  gui_app_destroy ();
  return FALSE;
}

gint
conv_hex_to_dec (gchar * hex)
{
  return strtoul ((const char *) hex, NULL, 16);
}

GString *
conv_dec_to_hex (gint dec)
{
  GString *hex;
  hex = g_string_new ("");

  if (dec == 0)
	{
	  g_string_append (hex, "0");
	  return hex;
	}

  while (dec)
	{
	  gint lfbits;
	  gchar digit[2] = "X";
	  lfbits = dec & 0xF;

	  if (lfbits < 10)
		digit[0] = '0' + lfbits;
	  else
		digit[0] = 'A' + lfbits - 10;

	  g_string_prepend (hex, digit);

	  dec >>= 4;
	}

  return hex;
}

void
on_new1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  file_op_editor_new ();
}


void
on_open1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  file_op_editor_open ();
}


void
on_save1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  file_op_editor_save ();
}


void
on_save_as1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  file_op_editor_save_as ();
}


void
on_print_activate (GtkAction * menuitem, gpointer user_data)
{
  g_assert (app->editor);
  gui_editor_print (app->editor);
}

void on_font_select_activate (GtkAction * menuitem, gpointer user_data)
{
  GtkWidget *font_selection_dialog = NULL;
  const gchar *font_name = gui_editor_get_font (app->editor);
  gint action = 0;
  g_assert (app->editor);
#if GTK_CHECK_VERSION (3, 2, 0)
  font_selection_dialog = gtk_font_chooser_dialog_new (_("Select font"), NULL);
  gtk_font_chooser_set_font (GTK_FONT_CHOOSER (font_selection_dialog), font_name);
#else
  font_selection_dialog = gtk_font_selection_dialog_new (_("Select font"));
  gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG (font_selection_dialog), font_name);
#endif
  action = gtk_dialog_run (GTK_DIALOG (font_selection_dialog));
  switch (action) 
  {
    case GTK_RESPONSE_OK:
#if GTK_CHECK_VERSION (3, 2, 0)
      font_name = gtk_font_chooser_get_font (GTK_FONT_CHOOSER (font_selection_dialog));
#else
      font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (font_selection_dialog));
#endif
      if (font_name)
      {
        gui_editor_set_font (app->editor, font_name);
      }
      break;
    default:
      break;
  }
  
  gtk_widget_destroy(font_selection_dialog);
}

void
on_quit1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  if (gtk_text_buffer_get_modified ((GtkTextBuffer *)app->editor->buffer))
	{
	  if (!file_op_confirm_save())
		return;
	}
  gtk_main_quit ();
}


void
on_registers1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  eef_reset_reg ();
  gui_view_update_reg_flag (FALSE);
}


void
on_flags1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  eef_reset_flag ();
  gui_view_update_reg_flag (FALSE);
}


void
on_io_ports1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  eef_reset_io ();
  gui_view_update_io_mem ();
  gui_list_io_update ();
}


void
on_main_memory1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  eef_reset_mem ();
  gui_view_update_io_mem ();
  gui_list_memory_update ();
}


void
on_reset_all1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  eef_reset_all ();
  gui_view_update_reg_flag (TRUE);
  gui_view_update_io_mem ();
  gui_list_io_update ();
  gui_list_memory_update ();
}


static void
validate_start_addr (void)
{
  gint la;
  GtkEntry *entry;

  entry = GTK_ENTRY (lookup_widget (app->window_main, "main_entry_sa"));
  g_assert (entry);

  /* get load address */
  if (asm_util_parse_number ((gchar *) gtk_entry_get_text (entry), &la))
	start_addr = la;
  else
	start_addr = DEFAULT_LOAD_ADDR;


}

gboolean asm_error = FALSE;
void
on_assemble1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  /* assemble file */
  gchar *a_text;

  /* la */
  validate_start_addr ();

  /* get text */
  a_text = gui_editor_get_text (app->editor);

  asm_error = b_assemble (a_text, start_addr);
  if (asm_error == FALSE)
	gui_app_show_msg (GTK_MESSAGE_WARNING,
					  _("Program has errors. Check the Message pane."));

  g_free (a_text);

  //disp_list( b_get_src() );
}



void
on_execute1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  if (b_get_state () == B_STATE_IDLE)
	on_assemble1_activate (NULL, NULL);

  if (asm_error == FALSE)
	return;

  if (!b_execute ())
	{
	  gui_app_show_msg (GTK_MESSAGE_ERROR,
						_("Error executing program"));
	}

  /* update */
  gui_view_update_all ();


}


void
on_step_in1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  if (b_get_state () == B_STATE_IDLE)
	on_assemble1_activate (NULL, NULL);

  if (asm_error == FALSE)
	return;
  if (!b_resume_execution (B_TRACE_IN))
	{
	  gui_app_show_msg (GTK_MESSAGE_ERROR, _("Error in step in"));
	}
}


void
on_step_over1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  if (b_get_state () == B_STATE_IDLE)
	on_assemble1_activate (NULL, NULL);

  if (asm_error == FALSE)
	return;
  if (!b_resume_execution (B_TRACE_OVER))
	{
	  gui_app_show_msg (GTK_MESSAGE_ERROR, _("Error in step over"));
	}
}


void
on_step_out1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  if (b_get_state () == B_STATE_IDLE)
	on_assemble1_activate (NULL, NULL);

  if (asm_error == FALSE)
	return;
  if (!b_resume_execution (B_TRACE_OUT))
	{
	  gui_app_show_msg (GTK_MESSAGE_ERROR, _("Error in step out"));
	}
}


void
on_toggle_breakpoint1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  b_toggle_breakpoint ();
}


void
on_clear_all_breakpoints1_activate (GtkMenuItem * menuitem,
									gpointer user_data)
{
  gui_editor_clear_all_marks (app->editor);
}


void
on_stop_execution1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  b_debug_stop ();
}


void
on_help_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  // Show the HTML help.
#ifdef G_OS_WIN32
  gchar * html_file_name = g_strconcat(g_win32_get_package_installation_directory_of_module (NULL), G_DIR_SEPARATOR_S, "help", G_DIR_SEPARATOR_S, PACKAGE, ".htm", NULL);
#else
  gchar * html_file_name = g_strconcat(PACKAGE_HELP_DIR, G_DIR_SEPARATOR_S, PACKAGE, ".htm", NULL);
#endif
  gchar * html_help_uri = g_filename_to_uri(html_file_name, NULL, NULL);
  g_debug ("HTML Help URI: %s\n", html_help_uri);
  gtk_show_uri (gdk_screen_get_default(), html_help_uri, GDK_CURRENT_TIME, NULL);
  g_free (html_file_name);
  g_free (html_help_uri);
}


void
on_assembler_tutorial1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  show_tutorial ();
}


void
on_about1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  /* create about box */
  create_dialog_about ();
}


void
show_hide_side_pane (GtkToggleAction * menuitem, gpointer user_data)
{
  GtkWidget *side_pane;
  side_pane = lookup_widget (app->window_main, "vbox_data");
  if (gtk_toggle_action_get_active (menuitem))
	  gtk_widget_hide (side_pane);
  else
	  gtk_widget_show (side_pane);
}


void
on_main_io_set_clicked (GtkButton * button, gpointer user_data)
{

}

static void
entry_to_mm (gchar * entry_name, gchar * spin_name, guint8 * base)
{
  GtkSpinButton *sb;
  GtkEntry *entry;
  const gchar *s;
  gint val;

  g_assert (spin_name);
  g_assert (base);

  sb = GTK_SPIN_BUTTON (lookup_widget (app->window_main, spin_name));
  g_assert (sb);

  entry = GTK_ENTRY (lookup_widget (app->window_main, entry_name));
  g_assert (entry);

  s = gtk_entry_get_text (entry);

  if (!asm_util_parse_number ((gchar *) s, &val))
	{
	  gui_app_show_msg (GTK_MESSAGE_INFO,
						_("Enter a valid number within range"));
	  return;
	}

  base[(gint) gtk_spin_button_get_value (sb)] = val;
  if (!strcmp (entry_name,"main_mem_entry"))
    gui_list_memory_update_single (gtk_spin_button_get_value_as_int (sb));
  if (!strcmp (entry_name,"main_io_entry"))
    gui_list_io_update_single (gtk_spin_button_get_value_as_int (sb));
}

static void
spin_to_entry (GtkSpinButton * sb, gchar * entry_name, guint8 * base)
{
  GtkEntry *entry;
  gint val, addr;
  gchar s[10] = "";
  g_assert (base);
  g_assert (sb);
  g_assert (entry_name);
  /* set the value to text box */

  entry = GTK_ENTRY (lookup_widget (app->window_main, entry_name));
  g_assert (entry);

  addr = gtk_spin_button_get_value (GTK_SPIN_BUTTON (sb));
  g_assert (0 <= addr && addr < 65536);
  val = base[addr];

  g_ascii_dtostr (s, 10, val);


  gtk_entry_set_text (entry, s);
}

void
on_main_io_spin_changed (GtkEditable * editable, gpointer user_data)
{
  spin_to_entry (GTK_SPIN_BUTTON (editable), "main_io_entry", sys.io);
}


void
on_main_io_update_clicked (GtkButton * button, gpointer user_data)
{
  entry_to_mm ("main_io_entry", "main_io_spin", sys.io);
}


void
on_main_mem_spin_changed (GtkEditable * editable, gpointer user_data)
{
  spin_to_entry (GTK_SPIN_BUTTON (editable), "main_mem_entry", sys.mem);

}


void
on_main_mem_update_clicked (GtkButton * button, gpointer user_data)
{
  entry_to_mm ("main_mem_entry", "main_mem_spin", sys.mem);
}

void
on_show_listing1_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GString *list;
  GtkWidget *cont;

  /* assemble */
  on_assemble1_activate (NULL, NULL);
  if (asm_error == FALSE)
	return;

  list = asm_listing_generate (b_get_src ());

  //gui_editor_set_text (app->editor, list->str);

  /* show */
  wind = create_window_listing ();
  cont = lookup_widget (wind, "listing_vbox");
  g_assert (cont);
  edit = gui_editor_new ();
  g_assert (edit);
  gui_editor_show (edit);
  gui_editor_set_text (edit, list->str);
  gui_editor_set_readonly (edit, TRUE);
  gtk_box_pack_end (GTK_BOX (cont), edit->scroll, TRUE, TRUE, 0);
  gtk_window_maximize (GTK_WINDOW (wind));
  gtk_widget_show_all (wind);
  /* TODO clean up of listing window editor on delete event */

  /* clean up */
  g_string_free (list, TRUE);
}

void
on_listing_save_clicked (GtkButton * button, gpointer user_data)
{
  gchar *ltext;
  //i_save ();
  /* FIXME : the save dialog is getting hung in listing window! */
  g_assert (edit);
  ltext = gui_editor_get_text (edit);
  file_op_listing_save (ltext);
}

void
on_listing_print_clicked (GtkButton * button, gpointer user_data)
{
  g_assert (edit);
  gui_editor_print (edit);
}

void
on_main_but_to_hex_clicked (GtkButton * button, gpointer user_data)
{
  GtkWidget *hex_entry, *dec_entry;
  GString *hex;
  gint dec;

  hex_entry = lookup_widget (app->window_main, "main_entry_hex");
  g_assert (hex_entry);
  dec_entry = lookup_widget (app->window_main, "main_entry_dec");
  g_assert (dec_entry);

  asm_util_parse_number ((gchar *)
						 gtk_entry_get_text (GTK_ENTRY (dec_entry)),
						 &dec);
  hex = conv_dec_to_hex (dec);

  gtk_entry_set_text (GTK_ENTRY (hex_entry), hex->str);
  g_string_free (hex, TRUE);

}


void
on_main_but_to_dec_clicked (GtkButton * button, gpointer user_data)
{
  GtkWidget *hex_entry, *dec_entry;
  gchar *hex;
  gint dec;
  GString *dec_t;

  hex_entry = lookup_widget (app->window_main, "main_entry_hex");
  g_assert (hex_entry);
  dec_entry = lookup_widget (app->window_main, "main_entry_dec");
  g_assert (dec_entry);

  hex = (gchar *) gtk_entry_get_text (GTK_ENTRY (hex_entry));
  dec = conv_hex_to_dec (hex);

  dec_t = g_string_new ("");
  g_string_printf (dec_t, "%d", dec);
  gtk_entry_set_text (GTK_ENTRY (dec_entry), dec_t->str);
  g_string_free (dec_t, TRUE);

}

void
on_main_entry_dec_activate (GtkEntry * entry, gpointer user_data)
{
  on_main_but_to_hex_clicked (NULL, NULL);
}


void
on_main_entry_hex_activate (GtkEntry * entry, gpointer user_data)
{
  on_main_but_to_dec_clicked (NULL, NULL);
}

void
on_start_but_tutorial_clicked (GtkButton * button, gpointer user_data)
{
  show_tutorial ();
  on_start_but_close_clicked (button, NULL);
}


void
on_start_but_open_clicked (GtkButton * button, gpointer user_data)
{
  on_open1_activate (NULL, NULL);
  on_start_but_close_clicked (button, NULL);
}


void
on_start_but_close_clicked (GtkButton * button, gpointer user_data)
{
  GtkWidget *swd;

  swd = lookup_widget (GTK_WIDGET (button), "window_start");
  gtk_widget_destroy (swd);
}

void
on_mem_list_start_clicked (GtkButton * button, gpointer user_data)
{
  GtkWidget *start_entry;
  gint start_addr;
  gchar *text;
	
  start_entry = lookup_widget (app->window_main, "mem_list_start");
  g_assert (start_entry);
	
  if (!asm_util_parse_number (text = (gchar *)gtk_entry_get_text (GTK_ENTRY (start_entry)), &start_addr))
  {
    gui_app_show_msg (GTK_MESSAGE_INFO, _("Enter a valid number within range (0-65535 or 0h-FFFFh)"));
    return;
  }
  gui_list_memory_set_start (start_addr);
  gui_list_memory_update ();
  text[strlen (text)] = 'h';
  gtk_entry_set_text (GTK_ENTRY (start_entry), text);
}

void
on_mem_list_start_changed (GtkEntry *entry, gpointer user_data)
{
  gint start_addr;
  gchar *text;
  g_assert (entry);
	
  if (!asm_util_parse_number (text = (gchar *)gtk_entry_get_text (GTK_ENTRY (entry)),&start_addr))
  {
    gui_app_show_msg (GTK_MESSAGE_INFO, _("Enter a valid number within range (0-65535 or 0h-FFFFh)"));
    return;
  }
  gui_list_memory_set_start (start_addr);
  gui_list_memory_update ();
  text[strlen (text)] = 'h';
  gtk_entry_set_text (GTK_ENTRY (entry), text);
}

void
on_io_list_start_clicked (GtkButton * button, gpointer user_data)
{
  GtkWidget *start_entry;
  gint start_addr;
  gchar *text;

  start_entry = lookup_widget (app->window_main, "io_list_start");
  g_assert (start_entry);

  if (!asm_util_parse_number (text = (gchar *)gtk_entry_get_text (GTK_ENTRY (start_entry)), &start_addr))
  {
    gui_app_show_msg (GTK_MESSAGE_INFO, _("Enter a valid number within range (0-255 / 0h-00FFh)"));
    return;
  }
  gui_list_io_set_start (start_addr);
  gui_list_io_update ();
  text[strlen (text)] = 'h';
  gtk_entry_set_text (GTK_ENTRY (start_entry), text);
}

void
on_io_list_start_changed (GtkEntry *entry, gpointer user_data)
{
  gint start_addr;
  gchar *text;
  g_assert (entry);

  if (!asm_util_parse_number (text = (gchar *)gtk_entry_get_text (GTK_ENTRY (entry)),&start_addr))
  {
    gui_app_show_msg (GTK_MESSAGE_INFO, _("Enter a valid number within range (0-255 / 0h-00FFh)"));
    return;
  }
  gui_list_io_set_start (start_addr);
  gui_list_io_update ();
  text[strlen (text)] = 'h';
  gtk_entry_set_text (GTK_ENTRY (entry), text);
}

void
show_tutorial ()
{
  GString* tutorial_text = read_tutorial ();
  GtkWidget *cont;

  /* show */
  tutorial = create_window_tutorial ();
  cont = lookup_widget (tutorial, "tutorial_vbox");
  g_assert (cont);
  edit = gui_editor_new ();
  g_assert (edit);
  gui_editor_show (edit);
  if (tutorial_text == NULL)
  {
    tutorial_text = g_string_new (_("The tutorial file, asm-guide.txt, was not found. It should be present in directory - "));
    g_string_append (tutorial_text, PACKAGE_DOC_DIR);  
  }
  gui_editor_set_text (edit, tutorial_text->str);
  gui_editor_set_readonly (edit, TRUE);
  gui_editor_set_show_line_numbers (edit, FALSE);
  gtk_box_pack_end (GTK_BOX (cont), edit->scroll, TRUE, TRUE, 0);
  gtk_window_maximize (GTK_WINDOW (tutorial));
  gtk_widget_show_all (tutorial);

  /* clean up */
  g_string_free (tutorial_text, TRUE);
}

void
on_line_mark_activated (GtkSourceView *view, GtkTextIter *iter,
                        GdkEventButton *event, gpointer editor)
{
  if ((event->button == 1) && (event->type == GDK_BUTTON_PRESS))
  {
    gint line_no = gtk_text_iter_get_line (iter);
    gui_editor_toggle_mark_at_line ((GUIEditor *) editor, line_no);
  }
}

