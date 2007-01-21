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
	
	self->lang_manager = gtk_source_languages_manager_new ();

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
	g_assert (self);
//	si_editor_set_mark (self->sci, line_no, set);
}

void
gui_editor_set_highlight (GUIEditor * self, guint line_no, gboolean set)
{
	g_assert (self);
//	si_editor_set_highlight (self->sci, line_no, set);
}

void
gui_editor_toggle_mark (GUIEditor * self)
{
//	gint ln;
	g_assert (self);

	//ln = si_editor_get_current_line_no (self->sci);

/*	if (si_editor_get_mark (self->sci, ln))
		si_editor_set_mark (self->sci, ln, FALSE);
	else
		si_editor_set_mark (self->sci, ln, TRUE);
*/
}

void
gui_editor_clear_all_highlights (GUIEditor * self)
{
//	si_editor_clear_all_highlights (self->sci);
}

gboolean
gui_editor_is_marked (GUIEditor * self, gint ln)
{
//	return si_editor_get_mark (self->sci, ln);
    return FALSE;
}

void
gui_editor_clear_all_marks (GUIEditor * self)
{
//	si_editor_clear_all_marks (self->sci);
}

void
gui_editor_goto_line (GUIEditor * self, gint ln)
{
//	si_editor_set_current_line_no (self->sci, ln);
}

gint
gui_editor_get_line (GUIEditor * self)
{
//	return si_editor_get_current_line_no (self->sci);
    return 0;
}

void
gui_editor_insert (GUIEditor * self, gchar * text)
{
    gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(self->buffer), text, -1);
}

void
gui_editor_set_readonly (GUIEditor * self, gboolean val)
{
    gtk_text_view_set_editable (GTK_TEXT_VIEW(self->widget), val);
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

