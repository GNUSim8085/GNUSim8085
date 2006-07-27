/*
	Copyright (C) 2003  Sridhar <r_sridhar@users.sf.net>
	
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "file-op.h"
#include "gui-editor.h"
#include "gui-app.h"
#include "interface.h"
#include <glib.h>
#include <stdio.h>		//TODO: remove those non-glib funcs
#include <string.h>
#include <gnome.h>

#define MAX_ERR_MSG_SIZE 512

static GString *file_name = NULL;
//static GtkFileSelection *file_selection = NULL;

static void
_set_file_name (gchar * name)
{
	if (file_name == NULL)
		file_name = g_string_new (name);
	else
		g_string_assign (file_name, name);
}

static void
_fs_show (GCallback cb, gpointer udata, gchar * title)
{
	GtkWidget *file_selector;

	/* Create the selector */

	file_selector = gtk_file_selection_new (title);

	udata = udata ? udata : file_selector;
	g_signal_connect (GTK_OBJECT
			  (GTK_FILE_SELECTION (file_selector)->ok_button),
			  "clicked", G_CALLBACK (cb), (gpointer) udata);

	/* Ensure that the dialog box is destroyed when the user clicks a button. */

	g_signal_connect_swapped (GTK_OBJECT
				  (GTK_FILE_SELECTION (file_selector)->
				   ok_button), "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  (gpointer) file_selector);

	g_signal_connect_swapped (GTK_OBJECT
				  (GTK_FILE_SELECTION (file_selector)->
				   cancel_button), "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  (gpointer) file_selector);

	gtk_window_set_modal (GTK_WINDOW (file_selector), TRUE);
	gtk_widget_show (file_selector);
	//gtk_dialog_run (GTK_DIALOG(file_selector));
}

void
ori_open (gchar * fn, gboolean replace)
{

	GString *gstr;
	FILE *fp;
	char *nullstr = "NULLSTRING";
	if (!fn)
		fn = nullstr;

	fp = fopen (fn, "r");

	if (fp == NULL)
	{
		gchar errmsg[MAX_ERR_MSG_SIZE + 1];
		g_snprintf (errmsg, MAX_ERR_MSG_SIZE, _("Failed to open <%s>"),
			    fn);
		gui_app_show_msg (GTK_MESSAGE_ERROR, errmsg);
		return;
	}

	gstr = g_string_new ("");

	while (!feof (fp))
	{
		gchar buf[300] = { 0 };
		fgets (buf, 100, fp);
		g_string_append (gstr, buf);
	}

	gui_editor_set_text (app->editor, gstr->str);

	/* Set breakpoints as instructed in the source */
	{
		gchar *str = gstr->str;
		gint ln = 0;

		gchar **lines = NULL;

		g_assert (str);

		lines = g_strsplit (str, "\n", -1);
		g_assert (lines);

		/* for each line */
		while (lines[ln])
		{
			/* check for ;@ */
			if (strlen (lines[ln]) > 1)
			{
				if (lines[ln][0] == ';'
				    && lines[ln][1] == '@')
				{
					/* add breakpoint */
					gui_editor_set_mark (app->editor,
							     ln + 1, TRUE);
					//exit(99);
				}
			}
			ln++;
		}

		g_strfreev (lines);
	}

	g_string_free (gstr, TRUE);
	if (replace)
		_set_file_name (fn);
}

static void
ori_save (gchar * fn, gboolean replace)
{
	gchar *text;
	FILE *fp;
	char *nullstr = "NULLSTRING";
	if (!fn)
		fn = nullstr;

	text = gui_editor_get_text (app->editor);
	fp = fopen (fn, "w");
	if (fp == NULL)
	{
		gchar errmsg[MAX_ERR_MSG_SIZE + 1];
		g_snprintf (errmsg, MAX_ERR_MSG_SIZE, _("Failed to save <%s>"),
			    fn);
		gui_app_show_msg (GTK_MESSAGE_ERROR, errmsg);
		return;
	}
	fwrite (text, 1, strlen (text), fp);
	/* debug */
	fclose (fp);

	g_free (text);
	if (replace)
		_set_file_name (fn);
}

void
file_op_editor_new (void)
{
	gchar template[] = "\
\n\
;<Program title>\n\
\n\
\t\tjmp start\n\
\n\
;data\n\
\n\
\n\
;code\n\
start:\t\tnop\n\
\n\
\n\
\t\thlt\n\
";

	if (G_UNLIKELY (file_name))
	{
		g_string_free (file_name, TRUE);
		file_name = NULL;
	}

	/* Set template text */
	gui_editor_set_text (app->editor, template);
	gui_editor_goto_line (app->editor, 11);
}

void
fs_cb_open (GtkButton * button, gpointer user_data)
{
	gchar *selected_filename;

	selected_filename =
		(gchar *) gtk_file_selection_get_filename (GTK_FILE_SELECTION
							   (user_data));

	ori_open (selected_filename, TRUE);
}

void
fs_cb_save_as (GtkButton * button, gpointer user_data)
{
	gchar *selected_filename;

	selected_filename =
		(gchar *) gtk_file_selection_get_filename (GTK_FILE_SELECTION
							   (user_data));


	ori_save (selected_filename, TRUE);
}

/* funcs related to main editor */
void
file_op_editor_save (void)
{
	if (file_name)
	{
		ori_save (file_name->str, FALSE);
		return;
	}

	_fs_show (G_CALLBACK (fs_cb_save_as), NULL, _("Save this unnamed file"));
}

void
file_op_editor_save_as (void)
{

	_fs_show (G_CALLBACK (fs_cb_save_as), NULL,
		  _("Save this file under different name"));
}

void
file_op_editor_open (void)
{

	_fs_show (G_CALLBACK (fs_cb_open), NULL, _("Open an existing file"));
}

void
fs_cb_save_listing (GtkButton * button, gpointer user_data)
{
	gchar *selected_filename;
	gchar *text = (gchar *) user_data;
	FILE *fp;

	selected_filename =
		(gchar *) gtk_file_selection_get_filename (GTK_FILE_SELECTION
							   (gtk_widget_get_ancestor
							    (GTK_WIDGET(button),
							    GTK_TYPE_FILE_SELECTION)) );

	fp = fopen (selected_filename, "w");
	if (fp == NULL)
	{
		gui_app_show_msg (GTK_MESSAGE_ERROR, _("Failed to save listing file"));
		return;
	}
	fwrite (text, 1, strlen (text), fp);
	fclose (fp);
}

/* -- related to listing window */
void
file_op_listing_save (gchar * text)
{
	_fs_show (G_CALLBACK (fs_cb_save_listing), (gpointer) text,
		  _("Save assembler listing"));
}
