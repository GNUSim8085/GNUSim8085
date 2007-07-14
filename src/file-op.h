/*
	Copyright (C) 2003  Sridhar Ratna <srid@nearfar.org>
	
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
 * File operations are handled in this module
 * save, open, save as, save text....
 *
 * R. Sridhar
 */

#ifndef __FILE_OP_H__
#define __FILE_OP_H__

#include <glib.h>
#include <libgnome/libgnome.h>

G_BEGIN_DECLS

/* funcs related to main editor */
void file_op_editor_new (void);
void file_op_editor_save (void);
void file_op_editor_save_as (void);
void file_op_editor_open (void);

/* -- related to listing window */
void file_op_listing_save (gchar *text);

/* this is will be called from main.c passing argv[1] */
void ori_open (gchar * fn, gboolean replace);

G_END_DECLS

#endif /* __FILE_OP_H__*/
