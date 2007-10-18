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
 * This module handles Memory blocks
 * It is used by assembler to store object code
 * It is also used by 8085 module to load it into memory
 * This way assembler can communicate with 8085 directly 
 *
 * R. Sridhar
 */

#ifndef __8085_MEMBLOCK_H__
#define __8085_MEMBLOCK_H__

#include <glib.h>
#include "8085.h"

G_BEGIN_DECLS 

typedef struct
{
	eef_addr_t start_addr;	/* starting addr of memory to load this block in */
	eef_addr_t size_allocated;	/* total size allocated */
	eef_addr_t size;	/* size that should be copied to mm */
	
	eef_data_t *buffer;
} EefMemBlock;

EefMemBlock *eef_mem_block_new (eef_addr_t max_size);
void eef_mem_block_delete (EefMemBlock *self, gboolean also_buffer);
void eef_mem_block_set_start_addr (EefMemBlock *self, eef_addr_t sa);

/* operations */

/* realloc */
void eef_mem_block_realloc_buffer (EefMemBlock *self, eef_addr_t nsz);

/* inc size by adding data */
void eef_mem_block_grow (EefMemBlock *self, eef_data_t data);

/* inc size by adding data n times */
void eef_mem_block_grow_n (EefMemBlock *self, eef_data_t data, gint n);

G_END_DECLS
#endif /* __8085_MEMBLOCK_H__ */
