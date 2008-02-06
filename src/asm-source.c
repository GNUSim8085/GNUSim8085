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

#include "asm-source.h"
#include "asm-err-comm.h"
#include "asm-token.h"
#include "asm-id.h"
#include "asm-ds-symtable.h"

void
disp_list (AsmSource * self)
{
  gint cnt = 0;

  /* for each line */
  while (cnt < self->listing_buffer_total)
	{
	  /* listing */
	  cnt++;
	}
}

static void
asm_source_create_listing_buffer (AsmSource * self, const gchar * text)
{
  gchar **lines;
  gint cnt = 0;

  g_assert (text);

  lines = g_strsplit (text, "\n", -1);
  g_assert (lines);

  /* for each line */
  while (lines[cnt])
	{
	  /* listing */
	  self->listing_buffer[cnt] = g_string_new (lines[cnt]);
	  self->listing_buffer_total = cnt + 1;

	  cnt++;
	}

  g_strfreev (lines);
}

static void
asm_source_add_to_table (AsmSource * self, AsmSourceEntry * entry)
{
  self->entries[self->entries_total++] = entry;
}

static void
util_remove_comment (gchar * str)
{
  gchar *p = str;
  g_assert (str);
  while (*p && *p != ';')
	p++;
  *p = '\0';
}

static gboolean
asm_source_parse_from_listing (AsmSource * self)
{
  int i;
  gchar *line = NULL;
  AsmSourceEntry *sl;

  for (i = 0; i < self->listing_buffer_total; i++)
	{
	  line = g_strdup (self->listing_buffer[i]->str);

	  /* remove unwanted stuffs */
	  g_strstrip (line);
	  g_strdelimit (line, " \t", ' ');
	  util_remove_comment (line);

	  /* skip if not source code */
	  if (!line[0] || line[0] == ';')
		{
		  g_free (line);
		  continue;
		}

	  sl = asm_source_entry_new (line, i);
	  if (!sl)
		{
		  asm_err_comm_send (0, _("Assembling Aborted"),
							 ASM_ERR_MESSAGE);
		  return FALSE;
		}

	  asm_source_add_to_table (self, sl);

	  g_free (line);

	}

  return TRUE;
}

/* str is stripped to maximum. only spaces and commas */
AsmSourceEntry *
asm_source_entry_new (gchar * str, gint line_no)
{
  AsmSourceEntry *entry = NULL;
  AsmTokenizer *tokenizer;
  gchar *nstr = NULL;
  IdOpcode *id_opcode;
  IdPseudo *id_pseudo;
  gboolean need_operand = FALSE;

#define SEND_ERR(errstr)									\
  {															\
	asm_err_comm_send (line_no + 1, errstr, ASM_ERR_ERROR);	\
	entry = NULL;											\
	goto CLEAN_UP;											\
  }

#define GET_TK(del, asrt)						\
  g_free (nstr);								\
  nstr = asm_tokenizer_next (tokenizer, del);	\
  if ( asrt )									\
	g_assert (nstr);							\
												\
  /* assert */
  g_assert (str);
  g_assert (line_no >= 0);

  /* create */
  entry = g_malloc (sizeof (AsmSourceEntry));
  g_assert (entry);
  entry->listing_buffer_line_no = line_no;
  entry->s_id[0] = '\0';
  entry->s_opnd[0] = '\0';
  entry->s_op_id_opcode = NULL;
  entry->s_op_id_pseudo = NULL;

  tokenizer = asm_tokenizer_new (str);
  g_assert (tokenizer);

  /* ready */
  asm_tokenizer_ready (tokenizer);

  GET_TK (' ', 1);

  /* if label */
  if (nstr[strlen (nstr) - 1] == ':')
	{
	  nstr[strlen (nstr) - 1] = '\0';
	  g_stpcpy (entry->s_id, nstr);
	  GET_TK (' ', 0);

	  if (nstr == NULL)
		{
		  SEND_ERR (_("Label should be given to a code line"));
		}

	}

  /* this must be a op */

  id_pseudo = asm_id_pseudo_lookup (nstr);
  if (id_pseudo)
	{
	  /* pseudo */
	  entry->s_op = id_pseudo->op_num;
	  entry->s_op_id_pseudo = id_pseudo;
	  entry->s_opnd_size = id_pseudo->user_args;

	  if (id_pseudo->user_args)
		need_operand = TRUE;
	}
  else if (id_opcode =
		   asm_id_opcode_lookup (nstr, NULL, NULL), id_opcode)
	{
	  /* opcode */
	  if (id_opcode->num_args)
		{
		  /* need args for complete op info */
		  gchar *arg1, *arg2 = NULL;

		  /* get first before comma */
		  arg1 = asm_tokenizer_next (tokenizer, ',');
		  if (!arg1)
			SEND_ERR (_("Incomplete opcode"));


		  if (id_opcode->num_args == 2)
			{
			  arg2 = asm_tokenizer_next (tokenizer, ' ');	/* last stuff */
			  if (!arg2)
				SEND_ERR (_("Incomplete opcode"));

			}

		  id_opcode = asm_id_opcode_lookup (nstr, arg1, arg2);
		  if (!id_opcode)
			{
			  SEND_ERR (_("Incomplete opcode"));
			}
		}

	  entry->s_op = id_opcode->op_num;
	  entry->s_op_id_opcode = id_opcode;
	  entry->s_opnd_size = id_opcode->user_args;
	  entry->b_op = id_opcode->op_num;

	  if (id_opcode->user_args)
		need_operand = TRUE;
	}
  else
	{
	  /* debug */
	  g_print (_("\nNeither Op not PsOp: [%s]"), nstr);
	  SEND_ERR (_("Invalid Opcode or Pseudo op"));
	}

  /* assign remaining as operand */
  if (need_operand)
	{
	  GET_TK (' ', 0);
	  if (nstr == NULL)
		{
		  SEND_ERR (_("Opcode needs an user argument"));
		}
	  g_stpcpy (entry->s_opnd, nstr);
	}

  /* if any extra str */
  GET_TK (' ', 0);
  if (nstr)
	SEND_ERR (_("Extra characters in line"));

 CLEAN_UP:
  g_free (nstr);
  asm_tokenizer_destroy (tokenizer);
  return entry;
}

/* returns the ds */
AsmSource *
asm_source_new (const gchar * text)
{
  AsmSource *as;
  g_assert (text);

  as = g_malloc (sizeof (AsmSource));
  g_assert (as);

  /* init data */
  as->listing_buffer_total = 0;
  as->entries_total = 0;

  /* create listing */
  asm_source_create_listing_buffer (as, text);

  /* parse */
  if (!asm_source_parse_from_listing (as))
	return FALSE;

  return as;
}

void
asm_source_entry_destroy (AsmSourceEntry * entry)
{
  g_return_if_fail (entry);

  //g_free ( entry->s_id );
  //g_free ( entry->s_opnd );

  g_free (entry);
}

void
asm_source_destroy (AsmSource * src)
{
  int i = 0;

  g_return_if_fail (src);

  for (i = 0; i < src->listing_buffer_total; i++)
	{
	  g_string_free (src->listing_buffer[i], TRUE);
	}

  for (i = 0; i < src->entries_total; i++)
	{
	  asm_source_entry_destroy (src->entries[i]);
	}

  g_free (src);
}

static gboolean
asm_util_str_to_int (gchar * str, gint * value, int base)
{
  gchar *err = "";

  *value = strtol (str, &err, base);


  if (strlen (err))
	return FALSE;

  return TRUE;
}

gboolean
asm_util_parse_number (gchar * str, gint * value)
{

  g_assert (str);
  if (!g_ascii_isdigit (str[0]))
	return FALSE;

  if (str[strlen (str) - 1] == 'h' || str[strlen (str) - 1] == 'H')
	{
	  str[strlen (str) - 1] = '\0';

	  return asm_util_str_to_int (str, value, 16);
	}

  return asm_util_str_to_int (str, value, 10);
}

static gboolean
util_symbol_validate_and_get_num_after_sign (gchar * str, gint * val)
{
  gchar *p;
  g_assert (str);
  g_assert (*str);
  g_assert (val);

  *val = 0;
  p = str;
  while (*p && !(*p == '+' || *p == '-'))
	p++;
  if (*p == '\0')
	return TRUE;

  if (!asm_util_parse_number (p + 1, val))
	return FALSE;
  if (*p == '-')
	{
	  *val *= -1;
	}
  *p = '\0';

  return TRUE;
}

gboolean
asm_source_entry_parse_not_operand_but_this (AsmSourceEntry * entry,
											 gint * value, gchar * symbol)
{
  //gchar *symbol;

  //g_assert (entry);
  //g_return_val_if_fail (entry->s_opnd, FALSE);
  g_assert (symbol);
  g_assert (value);
  //symbol = entry->s_opnd;
  g_assert (symbol[0]);

  if (g_ascii_isdigit (symbol[0]))
	{
	  /* absolute number in some base */
	  return asm_util_parse_number (symbol, value);
	}
  else
	{
	  /* should be a symbol */
	  AsmSymEntry *sym;
	  gchar *good_sym;
	  gint val;

	  good_sym = g_strdup (symbol);
	  if (!util_symbol_validate_and_get_num_after_sign
		  (good_sym, &val))
		{
		  if (entry)
			{
			  asm_err_comm_send (entry->
								 listing_buffer_line_no,
								 _("Expression error in symbol"),
								 ASM_ERR_ERROR);
			}
		  return FALSE;
		}

	  sym = asm_sym_query (good_sym);

	  if (!sym)
		{
		  if (entry)
			{
			  asm_err_comm_send (entry->
								 listing_buffer_line_no,
								 _("Undefined symbol"),
								 ASM_ERR_ERROR);
			}
		  return FALSE;
		}

	  // This code is disabled to fix the bug #1683342
	  // See http://lustymonk.livejournal.com/1997.html
	  /* if (sym->data == NULL)
		 return FALSE;*/

	  *value = GPOINTER_TO_INT (sym->data) + val;
	  return TRUE;
	}

  g_assert_not_reached ();
  return FALSE;
}

gboolean
asm_source_entry_parse_operand (AsmSourceEntry * entry, gint * value)
{
  g_assert (entry);
  return asm_source_entry_parse_not_operand_but_this (entry, value,
													  entry->s_opnd);
}

static gint
asm_util_no_of_occr (gchar * str, gchar c)
{
  gchar *p = str;
  gint cnt = 0;

  g_assert (str);

  while (*p)
	{
	  if (*p == c)
		cnt++;
	  p++;
	}

  return cnt;
}

gboolean
asm_source_entry_get_size (AsmSourceEntry * entry, gint * size)
{
  g_assert (entry);
  g_assert (size);

  *size = 0;

  if (entry->s_op < 256)	/* opcode */
	{
	  *size = entry->s_opnd_size + 1;
	  return TRUE;
	}
  else			/* pseudo op */
	{
	  if (entry->s_op == ID_PSEUDO_DB)
		{
		  if (entry->s_opnd == NULL)
			*size = 0;
		  else
			*size = asm_util_no_of_occr (entry->s_opnd,
										 ',') + 1;
		}
	  else if (entry->s_op == ID_PSEUDO_DS)
		{
		  if (entry->s_opnd == NULL)
			*size = 0;
		  else
			{
			  //asm_util_parse_number ( entry->s_opnd, size);
			  asm_source_entry_parse_operand (entry, size);
			}
		}
	  return TRUE;
	}

  g_assert_not_reached ();
  return FALSE;
}
