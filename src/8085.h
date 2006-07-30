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
 * The Core 8085 Microprocessor definition
 * R. Sridhar
 */

#ifndef __8085_H__
#define __8085_H__

#include <glib.h>

G_BEGIN_DECLS
/* 16bit Address, 8bit Data */
typedef guint16 eef_addr_t;
typedef guint8 eef_data_t;
#define EEF_DATA_T_MAX 255
#define EEF_ADDR_T_MAX 65535

/* CPU Registers */
typedef struct _EefReg
{
	eef_data_t a, b, c, d, e, h, l;
	eef_data_t pswh, pswl;
	eef_data_t pch, pcl, sph, spl;
	eef_data_t int_reg;
} EefReg;

/* CPU Flags */
/*typedef struct _EefFlag
{
	unsigned s:1;
	unsigned z:1;
	unsigned ac:1;
	unsigned p:1;
	unsigned c:1;
} EefFlag;*/
typedef struct _EefFlag
{
	gboolean s;
	gboolean z;
	gboolean ac;
	gboolean p;
	gboolean c;
} EefFlag;

/* EefFlag to eef_data_t for push to stack then push psw */
eef_data_t eef_flag_to_data (EefFlag flag);
//EefFlag * 
void eef_data_to_flag (eef_data_t from, EefFlag *flag);

/* Memory - 64K */
typedef eef_data_t EefMem[EEF_ADDR_T_MAX + 1];

/* IO Ports - FF */
typedef eef_data_t EefIO[EEF_DATA_T_MAX + 1];

/* System (One Instance) */
typedef struct _EefSystem
{
	EefReg reg;
	EefFlag flag;
	EefMem mem;
	EefIO io;

	/* --private-- (don't touch directly) */
	gboolean int_enable;
} EefSystem;

typedef enum
{
	EEF_MM_UPDATE,		/* memory change */
	EEF_IO_UPDATE,		/* io change */

	EEF_ERROR		/* Execute error */
} EefEvent;

typedef void (*EefSigHandle) (EefEvent evt, gpointer data);

#define EEF_SET_PTR(ptr, val) \
	if ( ptr ) *ptr=val

/* reset func */
void eef_reset_reg (void);
void eef_reset_flag (void);
void eef_reset_io (void);
void eef_reset_mem (void);
void eef_reset_all (void);

/* Signal related */
void eef_signal_connect (EefEvent evt, EefSigHandle hdl);

/* start execution */

/* return if exceeds max_bytes */
gboolean
eef_execute_from (eef_addr_t sa, eef_addr_t * executed_bytes,
		  eef_addr_t max_bytes);

/* byte order */
eef_addr_t eef_swap_bytes (eef_addr_t data);
eef_addr_t eef_swap_bytes_in_addr (eef_addr_t addr);
void eef_split16 (eef_addr_t data, eef_data_t * h, eef_data_t * l);
eef_addr_t eef_join16 (eef_data_t h, eef_data_t l);

/* reads 16 bit little endian numbers and returning
 * it in big endian */
eef_addr_t eef_mem16_get (eef_addr_t addr);
void eef_mem16_put (eef_addr_t addr, eef_addr_t bigendi);

/* reg pairs - x = 'B'(BC) or 'D'(DE) or 'H'(HL) */
void eef_regpair_put (gchar x, eef_addr_t data);
eef_addr_t eef_regpair_get (gchar x);
gchar eef_regpair_get_another (gchar x);

/* returns NULL of 'M' */
eef_data_t *eef_loc_of_reg (gchar x);
/* for reg pairs: eg: D in h, E in l */
void eef_loc_of_regpair (gchar x, eef_data_t ** h, eef_data_t ** l);

/* stack abstraction */
void eef_stack_push_byte (eef_data_t data);
eef_data_t eef_stack_pop_byte (void);

void eef_save_pc_into_stack (void);
void eef_peek_pc_from_stack (void);

/* io */
eef_data_t eef_port_get (eef_data_t addr);
void eef_port_put (eef_data_t addr, eef_data_t data);

/* interrupt */
void eef_interrupt_enable (gboolean val);

/* load mem block */
/* mem_block should be a instance of EefMemBlock */
void eef_load_mem_block (gpointer mem_block);

/* system */
extern EefSystem sys;

#define EEF_CLEAR(structure) \
	memset( &structure, 0, sizeof(structure) )

eef_data_t eef_get_op_at_addr (eef_addr_t addr);

gboolean eef_is_call_instr (eef_data_t op);
gboolean eef_is_ret_instr (eef_data_t op);

eef_addr_t eef_pc_get (void);

/* Callbacks */
typedef gboolean (*EefTraceCallback) (eef_addr_t addr, eef_addr_t prev_addr,
				      gboolean finished);

void eef_set_trace_callback (EefTraceCallback cb);

typedef void (*EefStackModifiedCallBack) (eef_addr_t sp, gboolean pushed, gchar what);
void eef_set_stack_callback (EefStackModifiedCallBack cb);

#define CALL_LEN 3;

G_END_DECLS
#endif
