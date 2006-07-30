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

/*
 * Scintilla interface (C inteface to C++ code)
 *
 * R. Sridhar
 */

#ifndef __SCINTILLA_INTERFACE_H__
#define __SCINTILLA_INTERFACE_H__

#include <gtk/gtk.h>
#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>

G_BEGIN_DECLS
/* get new editor */
	ScintillaObject * si_editor_new (GtkWidget ** widget);

gchar *si_editor_get_text (ScintillaObject * self);
void si_editor_set_text (ScintillaObject * self, const gchar * text);

void si_editor_set_mark (ScintillaObject * self, guint line_no, gboolean set);
gboolean si_editor_get_mark (ScintillaObject * self, guint line_no);
void si_editor_set_highlight (ScintillaObject * self, guint line_no,
			      gboolean set);

gint si_editor_get_current_line_no (ScintillaObject * self);

void si_editor_clear_all_highlights (ScintillaObject * self);

void si_editor_clear_all_marks (ScintillaObject * self);

void si_editor_set_current_line_no (ScintillaObject * self, gint ln);

void
si_editor_set_readonly (ScintillaObject * self, gboolean val);

void si_editor_insert_text (ScintillaObject *self,gchar *text);

G_END_DECLS
#endif				/* __SCINTILLA_INTERFACE_H__ */
