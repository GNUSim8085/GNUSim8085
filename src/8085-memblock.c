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

#include "8085-memblock.h"

EefMemBlock *
eef_mem_block_new (eef_addr_t max_size)
{
  EefMemBlock *self;
  self = g_malloc (sizeof (EefMemBlock));
  g_return_val_if_fail (self, NULL);
	
  self->buffer = g_malloc (sizeof (eef_data_t) * max_size);
  g_return_val_if_fail (self->buffer, NULL);

  self->size_allocated = max_size;
  self->size = 0;

  return self;
}

void
eef_mem_block_delete (EefMemBlock * self, gboolean also_buffer)
{
  if (self && also_buffer)
	g_free (self->buffer);

  g_free (self);
}

void
eef_mem_block_set_start_addr (EefMemBlock * self, eef_addr_t sa)
{
  g_assert (self);
  self->start_addr = sa;
}

void
eef_mem_block_realloc_buffer (EefMemBlock * self, eef_addr_t nsz)
{
  g_assert (self);
  g_return_if_fail (nsz > self->size_allocated);

  self->buffer = g_realloc (self->buffer, nsz);
  self->size_allocated = nsz;
}

void
eef_mem_block_grow (EefMemBlock * self, eef_data_t data)
{
  g_assert (self);
  /* realloc if necessary */
  if (self->size == self->size_allocated)
	{
	  eef_addr_t nsz = self->size + 10;
	  /* check for size exceeding range */
	  g_assert (nsz > self->size);
	  eef_mem_block_realloc_buffer (self, nsz);
	}

  self->buffer[self->size++] = data;
}

/* inc size by adding data n times */
void
eef_mem_block_grow_n (EefMemBlock * self, eef_data_t data, gint n)
{
  int i=0;
	
  for (i=0; i<n; i++)
	eef_mem_block_grow (self, data);
}
