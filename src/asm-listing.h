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

/*
 * Assembler generated Listing
 *
 * R. Sridhar
 */

#ifndef __ASM_LISTING_H__
#define __ASM_LISTING_H__

#include <glib.h>
#include "asm-source.h"

G_BEGIN_DECLS

/* get listing */
GString *asm_listing_generate (AsmSource *src);

G_END_DECLS

#endif /* __ASM_LISTING_H__ */
