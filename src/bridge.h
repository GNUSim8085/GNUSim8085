/*
	Copyright (C) 2006  Sridhar Ratna <srid@nearfar.org>
	
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
 * This is module acts like a bridge between user interface funtionality
 * and other core modules
 * In most of the cases, gui callbacks will make use of this module for
 * their job to get done
 *
 * R. Sridhar
 */

#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include <gtk/gtk.h>
#include "8085.h"
#include "asm-source.h"

G_BEGIN_DECLS

/* the state of the application */
typedef enum
{
	B_STATE_IDLE,
	B_STATE_DEBUG
}BState;
BState b_get_state(void);

typedef enum
{
	B_TRACE_IN,
	B_TRACE_OVER,
	B_TRACE_OUT
}BTraceMode;

void b_init(void);

gboolean b_assemble(char *text, guint16 start_addr);
gboolean b_execute(void);

/* after this mode is set, execution is traced 
 * after program halt of stop it will automatically switch back
 * to normal mode */
void b_enter_debug_mode (void);

/* stop the current debugging */
void b_debug_stop(void);

/* toggles at current line number */
void b_toggle_breakpoint (void);

gboolean
b_resume_execution (BTraceMode tmode);

/* on 4 stuffs */
gboolean
b_resume_execution(BTraceMode tmode);

/* get src */
AsmSource *
b_get_src(void);

extern GtkWidget *appbar ;

G_END_DECLS

#endif /* __BRIDGE_H__*/
