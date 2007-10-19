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

/*
 * This module accepts a line of code
 * and do some stripping 
 * and returns tokens depending upon the delimiters.
 * Before tokenizing you can filter the string to the maximum
 * extent.  like "label1: mvi a,34h"
 *
 * This module is very specific to the assembler syntax
 *
 * R. Sridhar
 */

#ifndef __ASM_TOKEN_H__
#define __ASM_TOKEN_H__

#include <glib.h>

G_BEGIN_DECLS

typedef struct
{
  gchar *full_string;
  gchar *next;
}AsmTokenizer;

/* allocation */
AsmTokenizer *asm_tokenizer_new(gchar *str);
void asm_tokenizer_destroy(AsmTokenizer *self);

/* call these funcs in order... */
void asm_tokenizer_remove_comment(AsmTokenizer *self);
void asm_tokenizer_strip(AsmTokenizer *self);
void asm_tokenizer_compress(AsmTokenizer *self);

/* ....or call this once */
void asm_tokenizer_ready (AsmTokenizer *self);

/* returns next token 
 * caller should free the string */
gchar *asm_tokenizer_next (AsmTokenizer *self, gchar delimiter);


G_END_DECLS

#endif /* __ASM_TOKEN_H__ */
