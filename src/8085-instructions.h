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
 * This is the Instructuions module.
 * All the instruction definitions are found here
 * The interface is minimal. You have to give only the starting address of inst.
 *
 * R. Sridhar
 */

#include <glib.h>
#include "8085.h"

#ifndef __8085_INSTRUCTIONS_H__
#define __8085_INSTRUCTIONS_H__

G_BEGIN_DECLS
#define MAX_INSTS 246
/* execute this opcode and set is_halt_inst if a terminating inst
 * Returns FALSE of error */
gboolean eef_inst_execute (eef_addr_t addr,
						   eef_addr_t * consumed_bytes,
						   gboolean * is_halt_inst);

gint eef_instruction_length (eef_data_t op);

gboolean
eef_inst_is_halt_inst (eef_data_t op);

G_END_DECLS
#endif
