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
 * Keypad GUI
 *
 * Contains functions related to handling of keypad features
 * This module will communicate with gui-editor module for
 * inserting code into editor
 */




#ifndef __GUI_KEYPAD_H__
#define __GUI_KEYPAD_H__


#include <gtk/gtk.h>

G_BEGIN_DECLS 

void gui_keypad_attach_me (void);

G_END_DECLS
#endif /* __GUI_KEYPAD_H__ */
