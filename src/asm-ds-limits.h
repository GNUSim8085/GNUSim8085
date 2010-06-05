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
 * Assembler DataStructures: Limits
 *
 * R. Sridhar
 */
 
#ifndef __ASM_DS_LIMITS_H__
#define __ASM_DS_LIMITS_H__

#define ASM_DS_MAX_LINE_LENGTH 1024
#define ASM_DS_MAX_IDENTIFIER_LENGTH 32
#define ASM_DS_MAX_OPCODE_LENGTH 5

#define ASM_DS_MAX_OPERAND 100

#define ASM_SOURCE_MAX_LINES 5000 /* < 65536 */

#endif
