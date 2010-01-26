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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "file-op.h"
#include "gui-editor.h"
#include "gui-app.h"
#include "interface.h"

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

void
ori_open (gchar * fn, gboolean replace)
{

  GString *gstr;
  GFile *fp;
  GFileInputStream *file_in;
  char *nullstr = "NULLSTRING";
  gssize bytes = 1;

  if (!fn)
	fn = nullstr;

  fp = g_file_new_for_uri (fn);

  if (fp == NULL)
	{
	  gchar errmsg[MAX_ERR_MSG_SIZE + 1];
	  g_snprintf (errmsg, MAX_ERR_MSG_SIZE, _("Failed to open <%s>"),
				  fn);
	  gui_app_show_msg (GTK_MESSAGE_ERROR, errmsg);
	  return;
	}

  gstr = g_string_new ("");
  file_in = g_file_read (fp, NULL, NULL);
  
  while (bytes != 0)
	{
	  gchar buf[100] = { 0 };
	  bytes = g_input_stream_read (G_INPUT_STREAM (file_in), buf, 100, NULL, NULL);
	  g_string_append_len (gstr, buf, bytes);
	}
  g_input_stream_close (G_INPUT_STREAM (file_in), NULL, NULL);

  gui_editor_set_text (app->editor, gstr->str);
  gtk_text_buffer_set_modified ((GtkTextBuffer *)app->editor->buffer, FALSE);

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

static gboolean
ori_save (gchar * fn, gboolean replace)
{
  gchar *text;
  GFile *fp;
  GFileOutputStream *file_out;
  GError *error = NULL;
  char *nullstr = "NULLSTRING";
  gssize bytes;

  if (!fn)
	fn = nullstr;

  fp = g_file_new_for_uri (fn);

  file_out = g_file_create (fp, G_FILE_CREATE_NONE, NULL, &error);
  if ((file_out == NULL) && (error->code == G_IO_ERROR_EXISTS))
	{
	  if (replace)
		{
		  g_error_free (error);
		/* replace file */
		  file_out = g_file_replace (fp, NULL, TRUE, G_FILE_CREATE_NONE, NULL, &error);
		}
	}
  text = gui_editor_get_text (app->editor);
  if (file_out == NULL)
	{
	  gchar errmsg[MAX_ERR_MSG_SIZE + 1];
	  g_snprintf (errmsg, MAX_ERR_MSG_SIZE, _("Failed to save <%s>"),
				  fn);
	  gui_app_show_msg (GTK_MESSAGE_ERROR, errmsg);
	  return TRUE;
	}
  bytes = g_output_stream_write (G_OUTPUT_STREAM (file_out), text, strlen (text), NULL, NULL);
  gtk_text_buffer_set_modified ((GtkTextBuffer *)app->editor->buffer, FALSE);
  
  /* debug */
  g_output_stream_close (G_OUTPUT_STREAM (file_out), NULL, NULL);

  g_free (text);
  if (replace)
	_set_file_name (fn);

  /*g_error_free (error);*/
  return TRUE;
}

void
file_op_editor_new (void)
{
  gchar template[] = "\
\n\
;<Program title>\n\
\n\
jmp start\n\
\n\
;data\n\
\n\
\n\
;code\n\
start: nop\n\
\n\
\n\
hlt";

  if (gtk_text_buffer_get_modified ((GtkTextBuffer *)app->editor->buffer))
    {
      if (!file_op_confirm_save())
          return;
    }
  
  if (G_UNLIKELY (file_name))
	{
	  g_string_free (file_name, TRUE);
	  file_name = NULL;
	}

  /* Set template text */
  gui_editor_set_text (app->editor, template);
  gui_editor_goto_line (app->editor, 11);
  gtk_text_buffer_set_modified ((GtkTextBuffer *)app->editor->buffer, FALSE);
}

/* funcs related to main editor */
gboolean
file_op_editor_save (void)
{
  if (file_name)
	return ori_save (file_name->str, TRUE);

  gchar *selected_file;
  GtkWidget *file_selector;
  gboolean is_saved = FALSE;

  file_selector = create_file_dialog
	("Save File", GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_SAVE);

  while (!is_saved)
	{
	  if (gtk_dialog_run (GTK_DIALOG (file_selector)) == GTK_RESPONSE_ACCEPT)
		{
		  selected_file = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (file_selector));
		  is_saved = ori_save(selected_file, TRUE);
		  g_free (selected_file);
		}
	  else
		break;
	}

  gtk_widget_destroy (file_selector);
  return is_saved;
}

void
file_op_editor_save_as (void)
{
  gchar *selected_file;
  GtkWidget *file_selector;
  gboolean is_saved = FALSE;

  file_selector = create_file_dialog
	("Save File As", GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_SAVE);

  while (!is_saved)
	{
	  if (gtk_dialog_run (GTK_DIALOG (file_selector)) == GTK_RESPONSE_ACCEPT)
		{
		  selected_file = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (file_selector));
		  is_saved = ori_save(selected_file, TRUE);
		  g_free (selected_file);
		}
	  else
		break;
	}

  gtk_widget_destroy (file_selector);
}

void
file_op_editor_open (void)
{
  if (gtk_text_buffer_get_modified ((GtkTextBuffer *)app->editor->buffer))
	{
	  if (!file_op_confirm_save())
		return;
	}

  gchar *selected_file;
  GtkWidget *file_selector;

  file_selector = create_file_dialog
	("Open File", GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OPEN);

  if (gtk_dialog_run (GTK_DIALOG (file_selector)) == GTK_RESPONSE_ACCEPT)
	{

	  selected_file = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (file_selector));
	  ori_open(selected_file, TRUE);
	  g_free (selected_file);
	}

  gtk_widget_destroy (file_selector);
}

/* -- related to listing window */
void
file_op_listing_save (gchar * text)
{
  gchar *selected_filename;
  GtkWidget *file_selector;
  int ret;
  FILE *fp;

  file_selector = create_file_dialog
	("Save Op Listing", GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_SAVE);

  if (gtk_dialog_run (GTK_DIALOG (file_selector)) == GTK_RESPONSE_ACCEPT)
	{

	  selected_filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_selector));

	  fp = fopen (selected_filename, "w");
	  if (fp == NULL)
		{
		  gui_app_show_msg (GTK_MESSAGE_ERROR, _("Failed to save listing file"));
		  return;
		}
	  ret = fwrite (text, 1, strlen (text), fp);
	  fclose (fp);

	  g_free (selected_filename);
	}

  gtk_widget_destroy (file_selector);
}


GtkWidget *
create_file_dialog(const gchar *title,
				   GtkFileChooserAction action,
				   const gchar *stock)
				   
{
  GtkWidget *file_selector;
  GtkFileFilter *file_filter;
  
  file_selector = gtk_file_chooser_dialog_new
	(title, NULL,
	 action,
	 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	 stock, GTK_RESPONSE_ACCEPT,
	 NULL);

  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER(file_selector), TRUE);
  gtk_file_chooser_set_local_only (GTK_FILE_CHOOSER(file_selector), FALSE);
  
  file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name (file_filter, "ASM Files");
  gtk_file_filter_add_pattern(file_filter, "*.asm");
  gtk_file_chooser_add_filter
	(GTK_FILE_CHOOSER(file_selector),
	 GTK_FILE_FILTER(file_filter));

  file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name (file_filter, "All Files");
  gtk_file_filter_add_pattern(file_filter, "*");
  gtk_file_chooser_add_filter
	(GTK_FILE_CHOOSER(file_selector),
	 GTK_FILE_FILTER(file_filter));
  
  return file_selector;
}

/* to confirm saving the file */
gboolean
file_op_confirm_save (void)
{
  GtkWidget *dialog;
  GtkResponseType response;
  gchar *primary_msg;
  gchar *secondary_msg;
  primary_msg = g_strdup_printf ("Close the file without saving?");
  secondary_msg = g_strdup_printf ("All the changes made will be lost if unsaved." );

  dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,
								   GTK_BUTTONS_NONE, "%s", primary_msg);
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), "%s", secondary_msg);
  g_free (primary_msg);
  g_free (secondary_msg);
  gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
  gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_DISCARD, GTK_RESPONSE_CLOSE);
  gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
  gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_SAVE, GTK_RESPONSE_OK);
  gtk_dialog_set_default_response	(GTK_DIALOG (dialog), GTK_RESPONSE_CANCEL);

  response = gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);

  if (response == GTK_RESPONSE_CLOSE)
	return TRUE;
  if (response == GTK_RESPONSE_OK)
	return file_op_editor_save ();

  return FALSE;
}
