/*
  Copyright (C) 2003  Sridhar Ratnakumar <sridhar.ratna@gmail.com>
	
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
 * Assembler Input table
 * This is very imp. ds.  
 * It represents source file in table form
 * The fields in this data structure will be descriptive
 *
 * R. Sridhar
 */

#ifndef __ASM_SOURCE_H__
#define __ASM_SOURCE_H__

#include <libintl.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "asm-ds-limits.h"
#include "asm-id.h"

#define _(String) gettext (String)

G_BEGIN_DECLS typedef struct
{
  /* point to actual source line no - used in generating listing */
  gint listing_buffer_line_no;

  /* generated during first scan */
  gchar s_id[ASM_DS_MAX_IDENTIFIER_LENGTH];	/* identifier in first column */
  gint16 s_op;		/* < 256 - opcode, otherwise pseudo op */
  IdOpcode *s_op_id_opcode;
  IdPseudo *s_op_id_pseudo;
  gint s_opnd_size;	/* 0, 1, 2 */
  gchar s_opnd[ASM_DS_MAX_OPERAND];	/* = one of s_id (or) number */

  /* generated after final assembling - binary */
  guint8 b_op;
  guint8 b_opnd1;
  guint8 b_opnd2;

  /* address in memory */
  gint address;

} AsmSourceEntry;

/* parses "str", whose line_no is "line_no" , returns new */
AsmSourceEntry *asm_source_entry_new (gchar * str, gint line_no);
void asm_source_entry_destroy (AsmSourceEntry * entry);

typedef struct
{
  /* actual source code */
  GString *listing_buffer[ASM_SOURCE_MAX_LINES];
  guint listing_buffer_total;

  /* table - used by assembler */
  AsmSourceEntry *entries[ASM_SOURCE_MAX_LINES];
  guint entries_total;	/* 0 - entries_total-1 in entries */

} AsmSource;

/* returns the ds */
AsmSource *asm_source_new (const gchar * text);
void asm_source_destroy (AsmSource * src);

/* util */

/* very common func
 * parse the user opnd, query systab if necessary
 * or parse user number and store it in integer form in "value" */
gboolean asm_source_entry_parse_operand (AsmSourceEntry * entry,
										 gint * value);
gboolean
asm_source_entry_parse_not_operand_but_this (AsmSourceEntry * entry, gint * value, gchar *symbol);


/* returns the length instruction
 * retunrs 0 if a pseudo op,
 * exception ( for DB-returns no. of comma+1 and DS value of opnd ) */
gboolean asm_source_entry_get_size (AsmSourceEntry * entry, gint * size);

gboolean asm_util_parse_number (gchar * str, gint * value);

G_END_DECLS
#endif /* __ASM_SOURCE_H__ */
