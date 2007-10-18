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

#include "gui-editor.h"

GUIEditor *
gui_editor_new (void)
{
	GUIEditor *self;

	self = g_malloc0 (sizeof (GUIEditor));

	self->buffer = GTK_SOURCE_BUFFER (gtk_source_buffer_new (NULL));

	self->widget = gtk_source_view_new_with_buffer (GTK_SOURCE_BUFFER(self->buffer));

	self->scroll = gtk_scrolled_window_new (NULL, NULL);

	gtk_container_add (GTK_CONTAINER (self->scroll), self->widget);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (self->scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gui_editor_set_font (self, DEFAULT_EDITOR_FONT);

	self->hltag = gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(self->buffer),
							HIGHLIGHT_TAG,
							"background", COLOUR_BG_HL,
							NULL);
	
	self->lang_manager = gtk_source_languages_manager_new ();

	GdkPixbuf *pixbuf;
	pixbuf = gui_editor_get_stock_icon (GTK_WIDGET(self->widget), GTK_STOCK_NO, GTK_ICON_SIZE_MENU);
	if (pixbuf)
	{
        	gtk_source_view_set_marker_pixbuf (GTK_SOURCE_VIEW (self->widget), MARKER_BREAKPOINT, pixbuf);
		g_object_unref (pixbuf);
	}

	return self;
}

void
gui_editor_show (GUIEditor * self)
{
	gtk_widget_show (GTK_WIDGET (self->widget));
	gtk_widget_show (GTK_WIDGET (self->scroll));
	gtk_source_view_set_show_line_numbers (GTK_SOURCE_VIEW(self->widget), TRUE);
	self->language = gui_editor_languages_manager_get_language_from_id(self->lang_manager,"8085@32@Assembly");
	if (self->language != NULL){
		gtk_source_buffer_set_language (GTK_SOURCE_BUFFER(self->buffer), GTK_SOURCE_LANGUAGE(self->language));
		gtk_source_buffer_set_highlight (GTK_SOURCE_BUFFER(self->buffer), TRUE);
	}
	self->mark = gtk_text_buffer_get_insert (GTK_TEXT_BUFFER(self->buffer));
	gtk_source_view_set_show_line_markers (GTK_SOURCE_VIEW(self->widget), TRUE);
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
	gtk_source_buffer_create_marker (GTK_SOURCE_BUFFER(self->buffer), NULL, MARKER_BREAKPOINT, &(self->iter));
	g_assert (self);
}

void
gui_editor_set_highlight (GUIEditor * self, guint line_no, gboolean set)
{
	g_assert (self);
	GtkTextIter line_start, line_end;

	/* get line bounds */
	gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (self->buffer), &line_start, line_no);
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
	GSList *marker_list, *list_iter;
	GtkSourceMarker *marker;

	insert = gtk_text_buffer_get_insert (GTK_TEXT_BUFFER(self->buffer));
	gtk_text_buffer_get_iter_at_mark (GTK_TEXT_BUFFER(self->buffer), &(self->iter), insert);
	y_buf = gtk_text_iter_get_line(&(self->iter));

	/* get line bounds */
	gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (self->buffer), &line_start, y_buf);
	line_end = line_start;
	gtk_text_iter_forward_to_line_end (&line_end);

	/* get the markers already in the line */
	marker_list = gtk_source_buffer_get_markers_in_region (self->buffer, &line_start, &line_end);

	/* search for the breakpoint marker */
	marker = NULL;
	for (list_iter = marker_list; list_iter && !marker; list_iter = g_slist_next (list_iter))
	{
		GtkSourceMarker *tmp = list_iter->data;
		gchar *tmp_type = gtk_source_marker_get_marker_type (tmp);
		if (tmp_type && !strcmp (tmp_type, MARKER_BREAKPOINT))
		{
			marker = tmp;
		}
                g_free (tmp_type);
	}

	g_slist_free (marker_list);

	if (marker)
	{
		/* a marker was found, so delete it */
		gtk_source_buffer_delete_marker (self->buffer, marker);
	}
	else
	{
		/* no marker found -> create one */
		marker = gtk_source_buffer_create_marker (self->buffer, NULL, MARKER_BREAKPOINT, &(self->iter));
	}
	
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
        list_iter = gtk_source_buffer_get_markers_in_region (self->buffer, &line_start, &line_end);

        /* search for the breakpoint marker */
	while (list_iter != NULL)
	{
                GtkSourceMarker *tmp = list_iter->data;
                gchar *tmp_type = gtk_source_marker_get_marker_type (tmp);
                if (tmp_type && !strcmp (tmp_type, MARKER_BREAKPOINT))
                {
                        return TRUE;
                }
                g_free (tmp_type);

		list_iter = g_slist_next (list_iter);
	}

	return FALSE;
}

void
gui_editor_clear_all_marks (GUIEditor * self)
{
        GtkTextIter buffer_start, buffer_end;
        GSList *list_iter;

        /* get buffer bounds */
	gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER(self->buffer), &buffer_start, &buffer_end);

        /* get the markers already in the line */
        list_iter = gtk_source_buffer_get_markers_in_region (self->buffer, &buffer_start, &buffer_end);

        /* search for the breakpoint marker */
        while (list_iter != NULL)
        {
                GtkSourceMarker *tmp = list_iter->data;
                gchar *tmp_type = gtk_source_marker_get_marker_type (tmp);
                if (tmp_type && !strcmp (tmp_type, MARKER_BREAKPOINT))
                {
                	/* a marker was found, so delete it */
                	gtk_source_buffer_delete_marker (self->buffer, tmp);
                }
                g_free (tmp_type);

                list_iter = g_slist_next (list_iter);
        }

}

void
gui_editor_goto_line (GUIEditor * self, gint ln)
{
	gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER(self->buffer), &(self->iter), ln);

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
gui_editor_grab_focus (GUIEditor * self)
{
	gtk_widget_grab_focus (self->widget);
}

void
gui_editor_set_font (GUIEditor *self, const gchar *font_name)
{

        PangoFontDescription *font_desc = NULL;

        g_return_if_fail (font_name != NULL);

        font_desc = pango_font_description_from_string (font_name);
        g_return_if_fail (font_desc != NULL);

        gtk_widget_modify_font (GTK_WIDGET (self->widget), font_desc);

        pango_font_description_free (font_desc);
}

GdkPixbuf *
gui_editor_get_stock_icon (GtkWidget *widget, const gchar *stock_id, GtkIconSize size)
{
        GdkScreen *screen = gtk_widget_get_screen (widget);
        GtkIconTheme *theme = gtk_icon_theme_get_for_screen (screen);
        gint marker_size;
        gtk_icon_size_lookup_for_settings (gtk_widget_get_settings (widget),
                                           size,
                                           NULL,
                                           &marker_size);


        GdkPixbuf *pixbuf;
        pixbuf = gtk_icon_theme_load_icon (theme, stock_id, marker_size, 0, NULL);

        return pixbuf;
}

GtkSourceLanguage *
gui_editor_languages_manager_get_language_from_id (GtkSourceLanguagesManager *lm,
							const gchar *lang_id)
{
        const GSList *languages;

        g_return_val_if_fail (lang_id != NULL, NULL);

        languages = gtk_source_languages_manager_get_available_languages (lm);

        while (languages != NULL)
        {
                gchar *name;

                GtkSourceLanguage *lang = GTK_SOURCE_LANGUAGE (languages->data);

                name = gtk_source_language_get_id (lang);

                if (strcmp (name, lang_id) == 0)
                {
                        g_free (name);
                        return lang;
                }

                g_free (name);
                languages = g_slist_next (languages);
        }

        return NULL;
}

