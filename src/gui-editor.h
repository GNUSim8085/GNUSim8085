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

/*
 * GUI - Editor for ASM code
 * using gtksourceview
 *
 * R. Sridhar 
 */

#ifndef __GUI_EDITOR_H__
#define __GUI_EDITOR_H__

#include <config.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include "callbacks.h"
#include "interface.h"

#define DEFAULT_EDITOR_FONT   (const gchar*) "Monospace 12"
#define MARKER_BREAKPOINT   (const gchar*) "breakpoint"
#define HIGHLIGHT_TAG   (const gchar*) "hl_tag"
#define COLOUR_BG_HL   (const gchar*) "#00FFFF"

G_BEGIN_DECLS typedef struct
{
  /* use this id for use with GTK+ */
  GtkWidget *widget;

  GtkWidget *scroll;

  const gchar* current_font;
	
  GtkSourceBuffer *buffer;

  GtkTextIter iter;

  GtkTextMark *mark;

  GtkTextTag *hltag;

  GtkSourceLanguageManager *lang_manager;
  GtkSourceStyleSchemeManager *style_scheme_manager;

  GtkSourceLanguage *language;

  GtkPrintOperation *print_operation;
  GtkSourcePrintCompositor *print_compositor;

} GUIEditor;

GUIEditor *gui_editor_new (void);
void gui_editor_show (GUIEditor * self);
void gui_editor_destroy (GUIEditor * self);

/* return new string and clones the string passed to it */
gchar *gui_editor_get_text (GUIEditor * self);
void gui_editor_set_text (GUIEditor * self, const gchar * text);

void gui_editor_set_mark (GUIEditor * self, guint line_no, gboolean set);
void gui_editor_set_highlight (GUIEditor * self, guint line_no, gboolean set);

void gui_editor_toggle_mark (GUIEditor * self);

void gui_editor_toggle_mark_at_line (GUIEditor * self, gint line_no);

void gui_editor_set_margin_toggle_mark (GUIEditor * self);

void gui_editor_clear_all_highlights(GUIEditor *self);

gboolean gui_editor_is_marked (GUIEditor *self, gint ln);

void gui_editor_clear_all_marks(GUIEditor *self);

void gui_editor_goto_line (GUIEditor *self, gint ln);

gint gui_editor_get_line (GUIEditor *self);

void gui_editor_set_readonly (GUIEditor *self, gboolean val);

void gui_editor_set_show_line_numbers (GUIEditor *self, gboolean val);

void gui_editor_insert (GUIEditor *self, gchar *text);

void gui_editor_grab_focus (GUIEditor *self);

const gchar *gui_editor_get_font (GUIEditor *self);

void gui_editor_set_font (GUIEditor *self, const gchar *font_name);

void gui_editor_print (GUIEditor *self);

GdkPixbuf *
gui_editor_get_stock_icon (GtkWidget *widget, const gchar *stock_id, GtkIconSize size);

G_END_DECLS
#endif /* __GUI_EDITOR_H__ */
