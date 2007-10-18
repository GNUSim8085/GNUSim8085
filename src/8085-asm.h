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
 * Main assembler definition file
 * Use this file to interface with the assembler
 * and possibly to some extent with 8085
 *
 * R. Sridhar
 */

#ifndef __8085_ASM_H__
#define __8085_ASM_H__

#include <glib.h>
#include <libgnome/libgnome.h>

#include "asm-ds-limits.h"
#include "asm-id.h"
#include "asm-source.h"
#include "asm-genobj.h"
#include "asm-gensym.h"
#include "asm-err-comm.h"
#include "asm-ds-symtable.h"

G_BEGIN_DECLS

/* call this when you are prepraring to assemble */
void eef_asm_init(void);

/* call this to deallocate memory used by assembler */
void eef_asm_destroy(void);

/* assemble this text and load it into memory */
gboolean eef_asm_assemble (const char *text, gint load_address, AsmSource **src_r, EefMemBlock **block_r);

G_END_DECLS

#endif /* __8085_ASM_H__ */
