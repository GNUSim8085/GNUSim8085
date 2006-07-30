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

#include "scintilla-interface.h"
#include "asm-id.h"

#define SSM(m, w, l) scintilla_send_message(self, m, w, l)

static gint marker_mark = 2, marker_highlight = 1;

ScintillaObject *
si_editor_new (GtkWidget ** widget)
{
	ScintillaObject *self;
	gchar *op_names;
	gint id_op, id_ps, id_num, id_com, id_other;
	g_assert (widget);

	/* create */
	*widget = scintilla_new ();
	g_assert (*widget);
	op_names = asm_id_return_all_opcode_names ();
	g_assert (op_names);

	self = SCINTILLA (*widget);
	scintilla_set_id (self, 0);
	gtk_widget_set_usize (*widget, 50, 100);

	id_op = SCE_C_STRING;
	id_ps = SCE_C_OPERATOR;
	id_com = SCE_C_COMMENT;
	id_num = SCE_C_COMMENTLINE;
	id_other = SCE_C_WORD;

	/* prepare */
	SSM (SCI_STYLECLEARALL, 0, 0);	/* first clear */
	SSM (SCI_SETLEXER, SCLEX_ASM, 0);	/* set suitable lexer */

	SSM (SCI_SETKEYWORDS, 4, (sptr_t) "db ds equ");	/* set keyword - pseudo */
	SSM (SCI_SETKEYWORDS, 0, (sptr_t) op_names);	/* set keyword - opcode */
	SSM (SCI_SETMARGINWIDTHN, 0, 32);	/* set line number margin width */

	/* set styles */
	SSM (SCI_STYLESETFORE, id_com, 0x008000);
	SSM (SCI_STYLESETFORE, id_num, 0xF000F0);
	//SSM (SCI_STYLESETFORE, SCE_C_NUMBER, 0x0000FF);
	SSM (SCI_STYLESETFORE, id_other, 0x000000);
	SSM (SCI_STYLESETFORE, id_ps, 0x000080);
	SSM (SCI_STYLESETBACK, id_ps, 0xF0F0F0);
	SSM (SCI_STYLESETFORE, id_op, 0x800000);
	SSM (SCI_STYLESETBOLD, id_ps, 0);
	SSM (SCI_STYLESETBOLD, id_op, 0);

	/* set size */
	SSM (SCI_STYLESETSIZE, id_op, 11);
	SSM (SCI_STYLESETSIZE, id_ps, 11);
	SSM (SCI_STYLESETSIZE, id_com, 11);
	SSM (SCI_STYLESETSIZE, id_num, 11);
	SSM (SCI_STYLESETSIZE, id_other, 11);

	/* Set markers */
	SSM (SCI_MARKERDEFINE, marker_highlight, SC_MARK_BACKGROUND);
	SSM (SCI_MARKERSETBACK, marker_highlight, 0xffFF00);
	SSM (SCI_MARKERDEFINE, marker_mark, SC_MARK_ROUNDRECT);
	SSM (SCI_MARKERSETFORE, marker_mark, 5);

	//SSM (SCI_ZOOMOUT, 0, 0);
	//SSM (SCI_ZOOMIN,0,0);

	//SSM (SCI_INSERTTEXT, 0, (sptr_t)"");

	g_free (op_names);

	return self;
}

static gint
si_editor_get_length (ScintillaObject * self)
{
	return SSM (SCI_GETLENGTH, 0, 0);
}

gchar *
si_editor_get_text (ScintillaObject * self)
{
	gint len;
	gchar *text;
	g_assert (self);

	len = si_editor_get_length (self);
	/* debug */
	text = g_new (gchar, len + 1);
	text[len] = '\0';

	SSM (SCI_GETTEXT, len + 1, (long int) text);	/* FIXME: ignoring + 1 means last char is not saved */

	return text;
}

void
si_editor_set_text (ScintillaObject * self, const gchar * text)
{
	g_assert (self);

	SSM (SCI_SETTEXT, 0, (long int) text);
}

void
si_editor_set_mark (ScintillaObject * self, guint line_no, gboolean set)
{
	if (set)
		SSM (SCI_MARKERADD, line_no, marker_mark);
	else
		SSM (SCI_MARKERDELETE, line_no, marker_mark);
}

gboolean
si_editor_get_mark (ScintillaObject * self, guint line_no)
{
	gint i = 0;

	i = SSM (SCI_MARKERGET, line_no, 0);

	//return (i == (1 << (marker_mark)));
	return (i & (1 << marker_mark));
}

gboolean
si_editor_get_highlight (ScintillaObject * self, guint line_no)
{
	guint32 i;

	i = SSM (SCI_MARKERGET, line_no, 0);

	return (i & (1 << marker_highlight));
}

void
si_editor_set_highlight (ScintillaObject * self, guint line_no, gboolean set)
{
	if (set)
		SSM (SCI_MARKERADD, line_no, marker_highlight);
	else
		SSM (SCI_MARKERDELETE, line_no, marker_highlight);
}

gint
si_editor_get_current_line_no (ScintillaObject * self)
{
	CharacterRange crange;
	int selStart;
	crange.cpMin = SSM (SCI_GETSELECTIONSTART, 0, 0);
	crange.cpMax = SSM (SCI_GETSELECTIONEND, 0, 0);
	selStart = crange.cpMin;
	return SSM (SCI_LINEFROMPOSITION, selStart, 0);
}

void
si_editor_set_current_line_no (ScintillaObject * self, gint ln)
{
	CharacterRange crange;

	crange.cpMin = SSM (SCI_POSITIONFROMLINE, ln, 0);
	crange.cpMax = crange.cpMin;

	SSM (SCI_SETSELECTIONSTART, crange.cpMin, 0);
	SSM (SCI_SETSELECTIONEND, crange.cpMax, 0);

	/* FIXME : is this correct way to move the view? */
	SSM (SCI_REPLACESEL, 0, (long int) "");

}

void
si_editor_clear_all_highlights (ScintillaObject * self)
{
	SSM (SCI_MARKERDELETEALL, marker_highlight, 0);
}

void
si_editor_clear_all_marks (ScintillaObject * self)
{
	SSM (SCI_MARKERDELETEALL, marker_mark, 0);
}

void
si_editor_set_readonly (ScintillaObject * self, gboolean val)
{
	SSM (SCI_SETREADONLY, val, 0);
}

void
si_editor_insert_text (ScintillaObject *self, gchar * text)
{
	g_assert (text);
	SSM (SCI_INSERTTEXT, (long unsigned int)-1, (long int)text);
}
