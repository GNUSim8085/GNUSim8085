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
 * View updaters 
 *
 * R. Sridhar
 */

#ifndef __GUI_VIEW_H__
#define __GUI_VIEW_H__

#include <gtk/gtk.h>
#include "support.h"
#include "8085.h"

G_BEGIN_DECLS

void gui_view_update_io_mem(void);
void gui_view_update_reg_flag(gboolean reset);
void gui_view_update_all(void);

void
gui_util_gen_hex (guint8 val, gchar * a, gchar * b);

G_END_DECLS

#endif /* __GUI_VIEW_H__*/
