/*
  Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
	
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
 * Error communication channel
 * between assembler and interface (gui?)
 *
 * R. Sridhar
 */

#ifndef __ASM_ERR_COMM_H__
#define __ASM_ERR_COMM_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum
{
  ASM_ERR_ERROR,
  ASM_ERR_WARNING,
  ASM_ERR_MESSAGE
}AsmErrType;

void asm_err_comm_send (gint line_no, gchar *err_msg, AsmErrType type);

G_END_DECLS

#endif /* __ASM_ERR_COMM_H__ */
