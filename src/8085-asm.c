/*
	Copyright (C) 2006  Sridhar Ratna <srid@nearfar.org>
	
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

#include "8085-asm.h"
#include "8085.h"
#include "8085-link.h"
#include "gui-list-message.h"
#include "gui-app.h"
#include "file-op.h"
#include "asm-err-comm.h"
#include <gnome.h>

/* call this when you are prepraring to assemble */
void
eef_asm_init (void)
{
	/* clear msg */
	gui_list_message_clear();
	//TODO data, stack
	
	/* Initialize symtab */
	asm_sym_clear ();

	/* clear link */
	eef_link_clear ();

}

/* call this to deallocate memory used by assembler */
void
eef_asm_destroy (void)
{
	/* */
}

/* assemble this text and load it into memory */
gboolean
eef_asm_assemble (const char *text, gint load_address, AsmSource **src_r, EefMemBlock **block_r)
{
	AsmSource *src;
	EefMemBlock *block;

	file_op_editor_save();
	
	src = asm_source_new (text);
	g_return_val_if_fail (src, FALSE);

	/* First pass */
	if (asm_gensym_generate (src, load_address) == FALSE)
		return FALSE;

	/* Second Pass */
	if (block = asm_genobj_generate (src, load_address), block == NULL)
		return FALSE;

	/* load block into memory */
	eef_load_mem_block (block);
	
	g_assert (src_r);
	g_assert (block_r);
	
	*src_r = src;
	*block_r = block;

	/* show success message */
	asm_err_comm_send (0, _("Program assembled successfully"), ASM_ERR_MESSAGE);
	
	return TRUE;
}
