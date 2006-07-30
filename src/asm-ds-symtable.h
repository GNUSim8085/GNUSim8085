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
 * Assembler DataStructures: Symbol Table
 *
 * R. Sridhar
 */

#ifndef __ASM_DS_SYMTABLE_H__
#define __ASM_DS_SYMTABLE_H__

#include <glib.h>
#include "asm-ds-limits.h"

G_BEGIN_DECLS typedef enum
{
	ASM_SYM_MACRO,
	ASM_SYM_VARIABLE,
	ASM_SYM_LABEL,

	ASM_SYM_NA
} AsmSymType;

typedef struct
{
	AsmSymType type;	/* type of sym */
	char name[ASM_DS_MAX_IDENTIFIER_LENGTH];	/* name of sym */
	gpointer data;		/* other needed data  - value */

	gint no_of_data;	/* if variable, no. of bytes allocated 
				 * for ds, = operand, for db = no. of commas + 1 */
	gint listing_no;	/* line number of src file(listing) where this symbol is defined */
} AsmSymEntry;

/* init */
void asm_sym_init (void);

/* destroy */
void asm_sym_clear (void);

/* clear sym table */
//void asm_sym_clear (void);

/* add symbols, data should be allocated by caller and deallocated by me
 * returns NULL if a symbol is redifined */
AsmSymEntry *asm_sym_add (AsmSymType type, char *name, gpointer data,
			  gint no_of_data, gint listing_no);

/* query symbol */
AsmSymEntry *asm_sym_query (char *name);

/* scanning the symtab */
void asm_sym_scan (GHFunc cb, gpointer userdata);

/* given a line no, type search for a symbol and return it 
 * returns internal string, don't delete! */
gchar *asm_sym_symbol_at (guint16 mem, AsmSymType type);

G_END_DECLS
#endif /* __ASM_DS_SYMTABLE_H__ */
