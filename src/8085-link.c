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
  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "8085-link.h"

#define LINK_TOTAL 65536

static gint link_data[LINK_TOTAL] = { -1 };

/* clear stuff - call this before assembly */
void eef_link_clear (void)
{
  int i;
  for (i=0; i<LINK_TOTAL; i++)
	link_data[i] = -1;
}

/* add address, lineno */
void eef_link_add (gint addr, gint lineno)
{
  g_assert (addr < LINK_TOTAL && addr >= 0 );
  link_data[addr] = lineno;
}

/* main func */
gint eef_link_get_line_no (gint addr)
{
  g_assert (addr < LINK_TOTAL && addr >= 0 );
  return link_data[addr];
}
