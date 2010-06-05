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


#ifndef __GUI_LIST_MEMORY_H__
#define __GUI_LIST_MEMORY_H__

#include <gtk/gtk.h>
#include "gui-app.h"
#include "8085.h"
#include "asm-source.h"
#include "gui-view.h"

G_BEGIN_DECLS

#define MAX_LIST_LEN 1000

void gui_list_memory_attach_me (void);

void gui_list_memory_clear (void);

void gui_list_memory_set_start (gint st);

void gui_list_memory_initialize (void);

void gui_list_memory_update (void);

void gui_list_memory_update_single (eef_addr_t addr);

G_END_DECLS

#endif /* __GUI_LIST_MEMORY_H__*/
