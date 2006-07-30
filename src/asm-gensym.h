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
 * Do a first pass over source
 * It just add the symbols and its value to the symtab
 *
 * R. Sridhar
 */

#ifndef __ASM_GENSYM_H__
#define __ASM_GENSYM_H__

#include <glib.h>
#include "asm-source.h"

G_BEGIN_DECLS

gboolean asm_gensym_generate (AsmSource *src, gint sa);

G_END_DECLS

#endif /* __ASM_GENSYM_H__ */
