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
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 * This module links between memory address of opcode 
 * and its corresponding line (line no) on source code
 * Given the address it will return the line no
 *
 * R. Sridhar
 */

#ifndef __8085_LINK_H__
#define __8085_LINK_H__

#include <glib.h>
#include <string.h> /* for memset */

G_BEGIN_DECLS

/* clear stuff - call this before assembly */
void eef_link_clear (void);

/* add address, lineno */
void eef_link_add (gint addr, gint lineno);

/* main func */
gint eef_link_get_line_no (gint addr);

G_END_DECLS

#endif /* __8085_LINK_H__ */
