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

/*
 * Handles showing selection dialog for symbols
 *  -used with KeyPad
 */
 
 


#ifndef __GUI_INPUT_SIGNAL_H__
#define __GUI_INPUT_SIGNAL_H__


#include <gtk/gtk.h>

G_BEGIN_DECLS

/* shows a input dialog for getting code symbol
 * returns symbol name newly allocated
 */
gchar *gui_input_symbol (void);

/* shows a input dialog for getting register name
 * returns symbol name newly allocated
 */
gchar *gui_input_reg (gchar * set);

G_END_DECLS

#endif /* __GUI_INPUT_SIGNAL_H__*/
