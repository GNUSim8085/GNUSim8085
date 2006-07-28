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

#include "gui-editor.h"

GUIEditor *
gui_editor_new (void)
{
	GUIEditor *self;

	self = g_malloc0 (sizeof (GUIEditor));

	self->sci = si_editor_new (&self->widget);

	return self;
}

void
gui_editor_show (GUIEditor * self)
{
	gtk_widget_show (GTK_WIDGET (self->widget));
}

void
gui_editor_destroy (GUIEditor * self)
{
	gtk_widget_destroy (self->widget);
	//FIXME : destroy scintilla object
	g_free (self);
}

/* return new string and clones the string passed to it */
gchar *
gui_editor_get_text (GUIEditor * self)
{
	g_assert (self);

	return si_editor_get_text (self->sci);
}

void
gui_editor_set_text (GUIEditor * self, const gchar * text)
{
	g_assert (self);
	si_editor_set_text (self->sci, text);
}


void
gui_editor_set_mark (GUIEditor * self, guint line_no, gboolean set)
{
	g_assert (self);
	si_editor_set_mark (self->sci, line_no, set);
}

void
gui_editor_set_highlight (GUIEditor * self, guint line_no, gboolean set)
{
	g_assert (self);
	si_editor_set_highlight (self->sci, line_no, set);
}

void
gui_editor_toggle_mark (GUIEditor * self)
{
	gint ln;
	g_assert (self);

	ln = si_editor_get_current_line_no (self->sci);

	if (si_editor_get_mark (self->sci, ln))
		si_editor_set_mark (self->sci, ln, FALSE);
	else
		si_editor_set_mark (self->sci, ln, TRUE);
}

void
gui_editor_clear_all_highlights (GUIEditor * self)
{
	si_editor_clear_all_highlights (self->sci);
}

gboolean
gui_editor_is_marked (GUIEditor * self, gint ln)
{
	return si_editor_get_mark (self->sci, ln);
}

void
gui_editor_clear_all_marks (GUIEditor * self)
{
	si_editor_clear_all_marks (self->sci);
}

void
gui_editor_goto_line (GUIEditor * self, gint ln)
{
	si_editor_set_current_line_no (self->sci, ln);
}

gint
gui_editor_get_line (GUIEditor * self)
{
	return si_editor_get_current_line_no (self->sci);
}

void
gui_editor_insert (GUIEditor * self, gchar * text)
{
	si_editor_insert_text (self->sci, text);
}

void
gui_editor_set_readonly (GUIEditor * self, gboolean val)
{
	si_editor_set_readonly (self->sci, val);
}

void
gui_editor_grab_focus (GUIEditor * self)
{
	gtk_widget_grab_focus (self->widget);
}
