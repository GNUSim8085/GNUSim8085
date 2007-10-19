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

#include "asm-token.h"

/* allocation */
AsmTokenizer *
asm_tokenizer_new (gchar * str)
{
  AsmTokenizer *self;
  g_assert (str);

  self = g_malloc (sizeof (AsmTokenizer));
  g_assert (self);

  self->full_string = g_strdup (str);
  self->next = self->full_string;

  return self;
}

void
asm_tokenizer_destroy (AsmTokenizer * self)
{
  g_assert (self);
  g_free (self->full_string);
  g_free (self);
}

/* call these funcs in order... */
void
asm_tokenizer_remove_comment (AsmTokenizer * self)
{
  gchar *p;

  g_assert (self);
  g_assert (self->full_string);

  p = self->full_string;

  while (*p && *p != ';')
	p++;

  *p = '\0';
}

void
asm_tokenizer_strip (AsmTokenizer * self)
{
  g_assert (self);
  g_assert (self->full_string);

  g_strstrip (self->full_string);
  g_strdelimit (self->full_string, " \t", ' ');

  return;
}

static gboolean
is_char_of_code (gchar c)
{
  if (c == ':' || g_ascii_isalnum (c) || c == ',')
	return TRUE;
  return FALSE;
}

void
asm_tokenizer_compress (AsmTokenizer * self)
{
  gchar *tmp;
  gint tmp_len = 0;
  gchar *ptr;
  gboolean pcomma = FALSE;
  g_assert (self);
  g_assert (self->full_string);

  tmp = g_strdup (self->full_string);
  ptr = self->full_string;

  while (*ptr)
	{
		
	  if ( is_char_of_code (*ptr) )
		{
		  if ( *ptr == ',' )
			pcomma = TRUE;
		  else
			pcomma = FALSE;
		  if ( *ptr == ',' && tmp_len && !is_char_of_code(tmp[tmp_len-1]) )
			tmp[ tmp_len-1 ] = *ptr++;
		  else
			tmp[ tmp_len++ ] = *ptr++;
		}
	  else
		{
		  /* skip as many whites as possible and add one space */
		  if ( !pcomma )
			tmp[ tmp_len++ ] = *ptr++;
		  pcomma = FALSE;
		  while ( *ptr && !is_char_of_code(*ptr) )
			ptr++;
		  if ( *ptr == '\0' )
			break;
		}
	}
  tmp[ tmp_len ] = '\0';
	
  /* swap */
  g_free (self->full_string);
  self->full_string = g_strdup (tmp);
  self->next = self->full_string;

  g_free (tmp);
}

/* ....or call this once */
void
asm_tokenizer_ready (AsmTokenizer * self)
{
  asm_tokenizer_remove_comment (self);
  asm_tokenizer_strip (self);
  asm_tokenizer_compress (self);
}

/* returns next token 
 * caller should free the string */
gchar *asm_tokenizer_next (AsmTokenizer * self, gchar delimiter)
{
  gchar *token;
  gint token_iter = 0;
  gchar *p;
	
  g_assert (self);
  if ( self->next == '\0' )
	return NULL;
	
  token = g_strdup ( self->next );
	
  p = self->next;
  while ( *p && *p == delimiter )
	p++;
	
  if ( *p == '\0' )
	{
	  g_free (token);
	  self->next = NULL;
	  return NULL;
	}
	
  do 
	{
	  token[ token_iter++ ] = *p++;
	}while ( *p && *p != delimiter );
	
  token[ token_iter ] = '\0';
  self->next = (*p)?p+1:p;
	
  return token;
	
}
