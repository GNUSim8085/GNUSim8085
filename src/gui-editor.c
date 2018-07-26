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

#include "gui-editor.h"

GUIEditor *
gui_editor_new (void)
{
  GUIEditor *self;
  GPtrArray *dirs;
  const gchar * const *current_search_path;
  gchar **lang_spec_search_path;

  self = g_malloc0 (sizeof (GUIEditor));

  self->buffer = gtk_source_buffer_new (NULL);

  self->widget = gtk_source_view_new_with_buffer (self->buffer);

  self->scroll = gtk_scrolled_window_new (NULL, NULL);

  gtk_container_add (GTK_CONTAINER (self->scroll), self->widget);

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (self->scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gui_editor_set_font (self, DEFAULT_EDITOR_FONT);

  self->hltag = gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(self->buffer),
											HIGHLIGHT_TAG,
											"background", COLOUR_BG_HL,
											NULL);
	
  self->lang_manager = gtk_source_language_manager_new ();

  self->style_scheme_manager = gtk_source_style_scheme_manager_get_default ();

  dirs = g_ptr_array_new();

  current_search_path = gtk_source_language_manager_get_search_path(self->lang_manager);

  for (; current_search_path != NULL && *current_search_path != NULL; ++current_search_path)
    g_ptr_array_add(dirs, g_strdup(*current_search_path));

  // look for spec file in our own directory
  g_ptr_array_add(dirs, g_strdup(PACKAGE_DATA_DIR));
  // look for spec file in data directory when running from svn
  g_ptr_array_add(dirs, g_strdup("data"));
  // look for spec file in current directory, when running on windows
  g_ptr_array_add(dirs, g_strdup("."));
  g_ptr_array_add(dirs, g_strdup(NULL));

  lang_spec_search_path = (gchar **)g_ptr_array_free(dirs, FALSE);

  gtk_source_language_manager_set_search_path (self->lang_manager, lang_spec_search_path);
  
  gtk_source_style_scheme_manager_append_search_path (self->style_scheme_manager, "data");
  gtk_source_style_scheme_manager_append_search_path (self->style_scheme_manager, ".");

  GtkSourceMarkAttributes *mark_attributes = gtk_source_mark_attributes_new ();
  gtk_source_mark_attributes_set_icon_name (mark_attributes, IMG_STOCK_MEDIA_RECORD);
  gtk_source_view_set_mark_attributes (GTK_SOURCE_VIEW (self->widget), MARKER_BREAKPOINT, mark_attributes, 0);

  return self;
}

void
gui_editor_show (GUIEditor * self)
{
  gtk_widget_show (GTK_WIDGET (self->widget));
  gtk_widget_show (GTK_WIDGET (self->scroll));
  gtk_source_view_set_show_line_numbers (GTK_SOURCE_VIEW(self->widget), TRUE);
  self->language = gtk_source_language_manager_get_language(self->lang_manager,"8085_asm");
  if (self->language != NULL){
	gtk_source_buffer_set_language (self->buffer, self->language);
	gtk_source_buffer_set_style_scheme (self->buffer, gtk_source_style_scheme_manager_get_scheme(self->style_scheme_manager,"classic"));
	gtk_source_buffer_set_highlight_syntax (self->buffer, TRUE);
  }
  self->mark = gtk_text_buffer_get_insert (GTK_TEXT_BUFFER(self->buffer));
  gtk_source_view_set_show_line_marks (GTK_SOURCE_VIEW(self->widget), TRUE);
}

void
gui_editor_destroy (GUIEditor * self)
{
  gtk_widget_destroy (self->widget);
  g_free (self);
}

/* return new string and clones the string passed to it */
gchar *
gui_editor_get_text (GUIEditor * self)
{
  g_assert (self);
  GtkTextIter *start = g_malloc0 (sizeof (GtkTextIter));
  GtkTextIter *end = g_malloc0 (sizeof (GtkTextIter));
  gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER(self->buffer), start, end);

  return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(self->buffer),start,end,FALSE);
}

void
gui_editor_set_text (GUIEditor * self, const gchar * text)
{
  g_assert (self);
  gtk_text_buffer_set_text(GTK_TEXT_BUFFER(self->buffer),text,-1);
}


void
gui_editor_set_mark (GUIEditor * self, guint line_no, gboolean set)
{
  gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER(self->buffer), &(self->iter), line_no);
  gtk_source_buffer_create_source_mark (self->buffer, NULL, MARKER_BREAKPOINT, &(self->iter));
  g_assert (self);
}

void
gui_editor_set_highlight (GUIEditor * self, guint line_no, gboolean set)
{
  g_assert (self);
  GtkTextIter line_start, line_end;

  /* get line bounds */
  gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (self->buffer), &line_start, (line_no -1));
  line_end = line_start;
  gtk_text_iter_forward_to_line_end (&line_end);

  if (set)
	gtk_text_buffer_apply_tag (GTK_TEXT_BUFFER(self->buffer), self->hltag, &line_start, &line_end);
  else
	gtk_text_buffer_remove_tag (GTK_TEXT_BUFFER(self->buffer), self->hltag, &line_start, &line_end);
}

void
gui_editor_toggle_mark (GUIEditor * self)
{
  g_assert (self);

  gint y_buf;
  GtkTextIter line_start, line_end;
  GtkTextMark *insert;
  GSList *marker_list;
  GtkSourceMark *marker;

  insert = gtk_text_buffer_get_insert (GTK_TEXT_BUFFER(self->buffer));
  gtk_text_buffer_get_iter_at_mark (GTK_TEXT_BUFFER(self->buffer), &(self->iter), insert);
  y_buf = gtk_text_iter_get_line(&(self->iter));

  /* get line bounds */
  gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (self->buffer), &line_start, y_buf);
  line_end = line_start;
  gtk_text_iter_forward_to_line_end (&line_end);

  /* get the breakpoint markers already in the line */
  marker_list = gtk_source_buffer_get_source_marks_at_line (self->buffer, y_buf, MARKER_BREAKPOINT);

  if (marker_list != NULL && g_slist_length(marker_list)!=0)
	{
	  /* markers were found, so delete them */
	  gtk_source_buffer_remove_source_marks (self->buffer,  &line_start, &line_end, MARKER_BREAKPOINT);
	}
  else
	{
	  /* no marker found -> create one */
	  marker = gtk_source_buffer_create_source_mark (self->buffer, NULL, MARKER_BREAKPOINT, &(self->iter));
	}
  g_slist_free (marker_list);
	
}

void
gui_editor_toggle_mark_at_line (GUIEditor * self, gint line_no)
{
  g_assert (self);

  GtkTextIter line_start, line_end;
  GSList *marker_list;
  GtkSourceMark *marker;

  /* get line bounds */
  gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (self->buffer), &line_start, line_no);
  line_end = line_start;
  gtk_text_iter_forward_to_line_end (&line_end);

  /* get the breakpoint markers already in the line */
  marker_list = gtk_source_buffer_get_source_marks_at_line (self->buffer, line_no, MARKER_BREAKPOINT);

  if (marker_list != NULL && g_slist_length(marker_list)!=0)
	{
	  /* markers were found, so delete them */
	  gtk_source_buffer_remove_source_marks (self->buffer,  &line_start, &line_end, MARKER_BREAKPOINT);
	}
  else
	{
	  /* no marker found -> create one */
	  marker = gtk_source_buffer_create_source_mark (self->buffer, NULL, MARKER_BREAKPOINT, &(line_start));
	}
  g_slist_free (marker_list);
	
}

void
gui_editor_set_margin_toggle_mark (GUIEditor * self)
{
  g_signal_connect ((gpointer) self->widget, "line-mark-activated",
                    G_CALLBACK (on_line_mark_activated),
                    self);
}

void
gui_editor_clear_all_highlights (GUIEditor * self)
{
  GtkTextIter buffer_start, buffer_end;

  gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER(self->buffer), &buffer_start, &buffer_end);
  gtk_text_buffer_remove_tag (GTK_TEXT_BUFFER(self->buffer), self->hltag, &buffer_start, &buffer_end);
}

gboolean
gui_editor_is_marked (GUIEditor * self, gint ln)
{
  GtkTextIter line_start, line_end;
  GSList *list_iter;

  /* get line bounds */
  gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (self->buffer), &line_start, ln);
  line_end = line_start;
  gtk_text_iter_forward_to_line_end (&line_end);

  /* get the markers already in the line */
  list_iter = gtk_source_buffer_get_source_marks_at_iter (self->buffer, &line_start, NULL);

  /* search for the breakpoint marker */
  while (list_iter != NULL)
	{
	  GtkSourceMark *tmp = list_iter->data;
	  const gchar *tmp_type = gtk_source_mark_get_category (tmp);
	  if (tmp_type && !strcmp (tmp_type, MARKER_BREAKPOINT))
		{
		  return TRUE;
		}
	  list_iter = g_slist_next (list_iter);
	}

  return FALSE;
}

void
gui_editor_clear_all_marks (GUIEditor * self)
{
  GtkTextIter buffer_start, buffer_end;

  /* get buffer bounds */
  gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER(self->buffer), &buffer_start, &buffer_end);

  /* delete all breakpoint mark */
  gtk_source_buffer_remove_source_marks (self->buffer, &buffer_start, &buffer_end, MARKER_BREAKPOINT);

}

void
gui_editor_goto_line (GUIEditor * self, gint ln)
{
  gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER(self->buffer), &(self->iter), (ln -1));

  gtk_text_buffer_place_cursor (GTK_TEXT_BUFFER(self->buffer), &(self->iter));

  gtk_text_buffer_move_mark (GTK_TEXT_BUFFER(self->buffer), self->mark, &(self->iter));

  gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(self->widget), self->mark);
}

gint
gui_editor_get_line (GUIEditor * self)
{
  gtk_text_buffer_get_iter_at_mark (GTK_TEXT_BUFFER(self->buffer), &(self->iter), self->mark);

  return gtk_text_iter_get_line (&(self->iter));
}

void
gui_editor_insert (GUIEditor * self, gchar * text)
{
  gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(self->buffer), text, -1);
}

void
gui_editor_set_readonly (GUIEditor * self, gboolean val)
{
  gtk_text_view_set_editable (GTK_TEXT_VIEW(self->widget), !val);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(self->widget), !val);
}

void
gui_editor_set_show_line_numbers (GUIEditor * self, gboolean val)
{
  gtk_source_view_set_show_line_numbers (GTK_SOURCE_VIEW(self->widget), val);
}

void
gui_editor_grab_focus (GUIEditor * self)
{
  gtk_widget_grab_focus (self->widget);
}

const gchar *
gui_editor_get_font (GUIEditor *self)
{
  return self->current_font;
}

void
gui_editor_set_font (GUIEditor *self, const gchar *font_name)
{

  PangoFontDescription *font_desc = NULL;

  g_return_if_fail (font_name != NULL);

  font_desc = pango_font_description_from_string (font_name);
  g_return_if_fail (font_desc != NULL);

  gtk_widget_modify_font (GTK_WIDGET (self->widget), font_desc);

  self->current_font = font_name;

  pango_font_description_free (font_desc);
}

static void
gui_editor_begin_print (GtkPrintOperation *operation, GtkPrintContext *context, GUIEditor *editor)
{
  g_assert (editor->buffer);

  /* Create a print compositor from the buffer */
  editor->print_compositor = gtk_source_print_compositor_new (editor->buffer);

  /* Set some formatting options for pages */
  gtk_source_print_compositor_set_print_header (editor->print_compositor, TRUE);
  gtk_source_print_compositor_set_print_footer (editor->print_compositor, TRUE);
  gtk_source_print_compositor_set_header_format (editor->print_compositor, TRUE, NULL, "%N/%Q", NULL);
  gtk_source_print_compositor_set_footer_format (editor->print_compositor, TRUE, NULL, PACKAGE_URL, NULL);
  gtk_source_print_compositor_set_left_margin (editor->print_compositor, 15.0, GTK_UNIT_MM);
  gtk_source_print_compositor_set_right_margin (editor->print_compositor, 15.0, GTK_UNIT_MM);
  gtk_source_print_compositor_set_bottom_margin (editor->print_compositor, 15.0, GTK_UNIT_MM);

  /* Pagination */
  while (!gtk_source_print_compositor_paginate (editor->print_compositor, context));
  gtk_print_operation_set_n_pages (operation, gtk_source_print_compositor_get_n_pages (editor->print_compositor));
}

static void
gui_editor_draw_page (GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, GUIEditor *editor)
{
  gtk_source_print_compositor_draw_page (editor->print_compositor, context, page_nr);
}

void
gui_editor_print (GUIEditor *editor)
{
  editor->print_operation = gtk_print_operation_new ();
#ifdef G_OS_WIN32
  gtk_print_operation_set_unit (editor->print_operation, GTK_UNIT_POINTS);
#endif
  GtkPrintOperationResult res;
  GError *error;
  GtkWidget *error_dialog;

  g_signal_connect (editor->print_operation, "begin-print", G_CALLBACK (gui_editor_begin_print), editor);
  g_signal_connect (editor->print_operation, "draw-page", G_CALLBACK (gui_editor_draw_page), editor);
  gtk_print_operation_set_show_progress (editor->print_operation, TRUE);
  res = gtk_print_operation_run (editor->print_operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, NULL, &error);
  if (res == GTK_PRINT_OPERATION_RESULT_ERROR )
  {
    error_dialog = gtk_message_dialog_new (GTK_WINDOW (editor->scroll),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_CLOSE,
						"Error printing file:\n%s",
						error->message);
    gtk_widget_show (error_dialog);
    g_error_free (error);
  }

  g_object_unref (editor->print_operation);
}

GdkPixbuf *
gui_editor_get_stock_icon (GtkWidget *widget, const gchar *stock_id, GtkIconSize size)
{
  GdkScreen *screen = gtk_widget_get_screen (widget);
  GtkIconTheme *theme = gtk_icon_theme_get_for_screen (screen);
  gint marker_size;
  gtk_icon_size_lookup (size,
		  NULL,
		  &marker_size);


  GdkPixbuf *pixbuf;
  pixbuf = gtk_icon_theme_load_icon (theme, stock_id, marker_size, 0, NULL);

  return pixbuf;
}

