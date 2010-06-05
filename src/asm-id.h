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
 * This contains the opcode and pseudo information
 * Each is assigned an ID. if ID < 256 it is a opcode otherwise pseudo op
 *
 * R. Sridhar
 */

#ifndef __ASM_ID_H__
#define __ASM_ID_H__

#include <glib.h>
#include "asm-ds-limits.h"

G_BEGIN_DECLS
/* OPCODE */

typedef struct
{
  gint op_num;
  gchar op_str[ASM_DS_MAX_OPCODE_LENGTH];
  gint num_args;
  gchar arg1[4];
  gchar arg2[4];

  gint user_args;		/* 0 - no arg, 1 - byte, 2 - word */
  gchar *op_desc;		/* describes the instructions */

} IdOpcode;


IdOpcode *asm_id_opcode_lookup (gchar * op_name, gchar * arg1, gchar * arg2);

/* PSEUDO */

typedef struct
{
  gint op_num; /* >= 256 */
  gchar op_str[ASM_DS_MAX_OPCODE_LENGTH];
	
  gint user_args;		/* 0 - no arg, n args seperated by comma */

} IdPseudo;

typedef enum
  {
	ID_PSEUDO_DB = 500,
	ID_PSEUDO_DS,
	
	ID_PSEUDO_EQU = 300
  }IdPseudoId;

IdPseudo *asm_id_pseudo_lookup (gchar * op_name);

/* returns space seperated list of all opcode names 
 * for use in syntax highlighting
 */
gchar *asm_id_return_all_opcode_names(void);

/* Get datastructure pointer */
IdOpcode * asm_id_get_opcode (void);
IdPseudo * asm_id_get_pseudo (void);

gint asm_id_total_opcodes (void);
gint asm_id_total_pseudos (void);

G_END_DECLS
#endif /* __ASM_ID_H__ */
