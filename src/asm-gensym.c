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

#include "asm-gensym.h"
#include "asm-ds-symtable.h"
#include "asm-err-comm.h"
#include <gnome.h>

gboolean
asm_gensym_generate (AsmSource * src, gint sa)
{
	int i;
	gint address = sa;

	g_assert (src);

#define SEND_ERR(ln, str)\
	asm_err_comm_send (ln, str, ASM_ERR_ERROR); \
	return FALSE;

	/* scan src */
	for (i = 0; i < src->entries_total; i++)
	{
		gint sz;
		AsmSourceEntry *entry = src->entries[i];

		/* Assign Label */
		if (entry->s_id[0])
		{
			AsmSymType type = ASM_SYM_NA;

			/* find type */
			if (entry->s_op >= 256)
			{
				if (!g_ascii_strcasecmp
				    ("EQU", entry->s_op_id_pseudo->op_str))
					type = ASM_SYM_MACRO;
				else if (!g_ascii_strcasecmp
					 ("DS", entry->s_op_id_pseudo->op_str)
					 || !g_ascii_strcasecmp ("DB",
								 entry->
								 s_op_id_pseudo->
								 op_str))
					type = ASM_SYM_VARIABLE;

			}
			else
				type = ASM_SYM_LABEL;

			if (type == ASM_SYM_NA)
			{
				asm_err_comm_send (entry->
						   listing_buffer_line_no,
						   _("Redundant label"),
						   ASM_ERR_WARNING);
			}
			else
			{
				gint val;

				/* fetch data */
				if (type == ASM_SYM_MACRO)
				{
					if (!asm_source_entry_parse_operand
					    (entry, &val))
					{
						SEND_ERR (entry->
							  listing_buffer_line_no,
							  _("Invalid Operand in EQU"));
					}
				}
				else
				{
					val = address;
				}
				/* get size */
				if (!asm_source_entry_get_size (entry, &sz))
				{
					SEND_ERR (entry->
						  listing_buffer_line_no,
						  _("Invalid operand"));
				}

				g_assert (entry->s_id[0]);

				/* add SYMBOL to table */
				if (!asm_sym_add
				    (type, entry->s_id,
				     GINT_TO_POINTER (val), sz, entry->listing_buffer_line_no))
				{
					SEND_ERR (entry->
						  listing_buffer_line_no,
						  _("Redefinition of symbol"));
				}
			}

		}		/* Finished - Assign Label */

		/* inc size */
		if (!asm_source_entry_get_size (entry, &sz))
		{
			SEND_ERR (entry->listing_buffer_line_no,
				  _("Invalid operand"));
		}
		address += sz;

	}

	return TRUE;
}
