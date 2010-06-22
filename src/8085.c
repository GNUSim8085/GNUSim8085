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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "8085.h"
#include "8085-instructions.h"
#include "8085-memblock.h"

/* errors */
char *err_regname = "None of RegName"; 

/* signal handlers */
EefSigHandle hdl_mm_update = NULL;
EefSigHandle hdl_io_update = NULL;
EefSigHandle hdl_error = NULL;

/* Only one System */
EefSystem sys;

/* cb */
static EefTraceCallback trace_cb = NULL;
static EefStackModifiedCallBack stack_cb = NULL;
/* reset */
void
eef_reset_reg (void)
{
  EEF_CLEAR (sys.reg);

  sys.reg.sph = 0xff;
  sys.reg.spl = 0xff;
}

void
eef_reset_flag (void)
{
  EEF_CLEAR (sys.flag);
}

void
eef_reset_io (void)
{
  EEF_CLEAR (sys.io);
}

void
eef_reset_mem (void)
{
  EEF_CLEAR (sys.mem);
}

void
eef_reset_all (void)
{
  EEF_CLEAR (sys);
  sys.reg.sph = 0xff;
  sys.reg.spl = 0xff;
}

#undef EEF_CLEAR

eef_addr_t
eef_swap_bytes (eef_addr_t data)
{
  eef_addr_t r = 0;
  r = (eef_data_t) data;
  r <<= 8;
  r |= (data >> 8);
  return r;
}

eef_addr_t
eef_swap_bytes_in_addr (eef_addr_t addr)
{
  eef_addr_t r = 0;
  r = sys.mem[addr + 1];
  r <<= 8;
  r |= (sys.mem[addr]);
  return r;
}

void
eef_split16 (eef_addr_t data, eef_data_t * h, eef_data_t * l)
{
  g_assert (h);
  g_assert (l);

  *l = (eef_data_t) data;
  *h = (eef_data_t) (data >> 8);

}

eef_addr_t
eef_join16 (eef_data_t h, eef_data_t l)
{
  eef_addr_t r;
  r = h;
  r <<= 8;
  r |= l;
  return r;
}


eef_addr_t
eef_mem16_get (eef_addr_t addr)
{
  return eef_swap_bytes_in_addr (addr);
}

void
eef_mem16_put (eef_addr_t addr, eef_addr_t bigendi)
{
  eef_split16 (bigendi, &sys.mem[addr + 1], &sys.mem[addr]);
}

/* reg pairs - x = 'B'(BC) or 'D'(DE) or 'H'(HL) */
void
eef_regpair_put (gchar x, eef_addr_t data)
{
  eef_data_t h, l;
  eef_split16 (data, &h, &l);

  switch (x)
	{
	case 'B':
	  sys.reg.b = h;
	  sys.reg.c = l;
	  break;

	case 'D':
	  sys.reg.d = h;
	  sys.reg.e = l;
	  break;

	case 'H':
	  sys.reg.h = h;
	  sys.reg.l = l;
	  break;

	case 'S':
	  sys.reg.sph = h;
	  sys.reg.spl = l;
	  break;

	default:
	  g_assert (!err_regname);
	  break;
	}
}

/* reg pairs - x = 'B'(BC) or 'D'(DE) or 'H'(HL) */
eef_addr_t
eef_regpair_get (gchar x)
{
  switch (x)
	{
	case 'B':
	  return eef_join16 (sys.reg.b, sys.reg.c);

	case 'D':
	  return eef_join16 (sys.reg.d, sys.reg.e);

	case 'H':
	  return eef_join16 (sys.reg.h, sys.reg.l);

	case 'S':
	  return eef_join16 (sys.reg.sph, sys.reg.spl);

	}
  g_assert (!err_regname);
  return 0;
}

eef_data_t *
eef_loc_of_reg (gchar x)
{
  switch (x)
	{
	case 'A':
	  return &sys.reg.a;
	case 'B':
	  return &sys.reg.b;
	case 'C':
	  return &sys.reg.c;
	case 'D':
	  return &sys.reg.d;
	case 'E':
	  return &sys.reg.e;
	case 'H':
	  return &sys.reg.h;
	case 'L':
	  return &sys.reg.l;
	case 'M':
	  return NULL;
	}
  g_assert (!err_regname);
  return NULL;
}

void
eef_loc_of_regpair (gchar x, eef_data_t ** h, eef_data_t ** l)
{
  switch (x)
	{
	case 'B':
	  *h = &sys.reg.b;
	  *l = &sys.reg.c;
	  break;
	case 'D':
	  *h = &sys.reg.d;
	  *l = &sys.reg.e;
	  break;
	case 'H':
	  *h = &sys.reg.h;
	  *l = &sys.reg.l;
	  break;
	case 'S':
	  *h = &sys.reg.sph;
	  *l = &sys.reg.spl;
	  break;
	case 'P':
	  *h = &sys.reg.pswh;
	  *l = &sys.reg.pswl;
	  g_warning ("Unwanted %c reached", x);
	  break;
	case 'M':
	  *h = NULL;
	  *l = NULL;
	  break;
	default:
	  g_critical (_("Unwanted %c reached"), x);
	  g_assert (!err_regname);
	}

  return;
}

void
eef_pc_set (eef_addr_t val)
{
  eef_data_t h, l;

  eef_split16 (val, &h, &l);
  sys.reg.pch = h;
  sys.reg.pcl = l;
}

eef_addr_t
eef_pc_get (void)
{
  return eef_join16 (sys.reg.pch, sys.reg.pcl);
}

void
eef_stack_push_byte (eef_data_t data)
{
  eef_regpair_put ('S', eef_regpair_get ('S') - 1);
  sys.mem[eef_regpair_get ('S')] = data;
}

eef_data_t
eef_stack_pop_byte (void)
{
  eef_regpair_put ('S', eef_regpair_get ('S') + 1);
  return sys.mem[eef_regpair_get ('S') - 1];
}

/* calls stack changed callback */
void
eef_call_stack_cb (gboolean pushed, gchar x)
{
  if (stack_cb)
	(*stack_cb) (eef_join16 (sys.reg.sph, sys.reg.spl), pushed,
			     x);
}

void
eef_save_pc_into_stack (void)
{
  eef_stack_push_byte (sys.reg.pch);
  eef_stack_push_byte (sys.reg.pcl);

  eef_call_stack_cb (TRUE, 'P');
}

void
eef_peek_pc_from_stack (void)
{
  sys.reg.pcl = eef_stack_pop_byte ();
  sys.reg.pch = eef_stack_pop_byte ();

  eef_call_stack_cb (FALSE, 'P');
}

eef_data_t
eef_port_get (eef_data_t addr)
{
  return sys.io[addr];
}

void
eef_port_put (eef_data_t addr, eef_data_t data)
{
  sys.io[addr] = data;
}

void
eef_interrupt_enable (gboolean val)
{
  sys.int_enable = val;
}

gboolean
eef_fetch_pcinc_execute (eef_addr_t * executed_bytes, gboolean * is_halt_inst)
{
  //static gint cnt = 0;
  eef_data_t opcode;
  eef_addr_t pc_contents;
  gint ilen;
  gboolean r;
  EEF_SET_PTR (is_halt_inst, FALSE);
  EEF_SET_PTR (executed_bytes, 1);	/* atleast */

  /* get pc contents */
  pc_contents = eef_pc_get ();


  /* fetch */
  opcode = sys.mem[pc_contents];


  /* inc pc */
  ilen = eef_instruction_length (opcode);

  /* debug g_print ("\n%4X: [[%x]] len=%d [", pc_contents, opcode, ilen);
   * {
   * int i;
   * for (i=0; i<ilen-1; i++)
   * g_print ("%2X ", sys.mem[pc_contents+i+1]);
   * g_print ("]");
   * } */

  eef_pc_set (pc_contents + ilen);

  /* execute */

  r = eef_inst_execute (pc_contents, executed_bytes, is_halt_inst);
  if (executed_bytes)
	(*executed_bytes)++;	/* eef_ins_execute will set to 0,1 or 2 only */

  /* check for integrity */
  g_assert (!executed_bytes || ilen == *executed_bytes);

  return r;

}


gboolean
eef_execute_from (eef_addr_t sa, eef_addr_t * executed_bytes,
				  eef_addr_t max_bytes)
{
  gboolean is_halt_inst = FALSE;
  gboolean entering = TRUE;
  eef_addr_t paddr = 0;
  eef_addr_t dummy = 0;
  if (!executed_bytes)
	executed_bytes = &dummy;

  /* load pc */
  eef_pc_set (sa);

  *executed_bytes = 0;
  while (!is_halt_inst /* FIXME && *executed_bytes < max_bytes */ )
	{
	  eef_addr_t eb_this = 0;

	  /* cb */
	  if (!entering && trace_cb)
		{
		  eef_addr_t na;
		  na = eef_pc_get ();	/* sa + *executed_bytes */
		  if ((*trace_cb) (na, paddr, FALSE) == FALSE)
			return TRUE;
		}
	  entering = FALSE;

	  paddr = eef_pc_get ();
	  if (!eef_fetch_pcinc_execute (&eb_this, &is_halt_inst))
		return FALSE;

	  *executed_bytes += eb_this;
	}

  if (trace_cb)
	{
	  (*trace_cb) (0, 0, TRUE);
	}

  return TRUE;
}

void
eef_load_mem_block (gpointer mem_block)
{
  EefMemBlock *block = (EefMemBlock *) mem_block;
  gint i;
  g_assert (block);

  for (i = 0; i < block->size; i++)
	{
	  sys.mem[i + block->start_addr] = block->buffer[i];
	}
}

void
eef_set_trace_callback (EefTraceCallback cb)
{
  trace_cb = cb;
}

void
eef_set_stack_callback (EefStackModifiedCallBack cb)
{
  stack_cb = cb;
}

eef_data_t
eef_get_op_at_addr (eef_addr_t addr)
{
  return sys.mem[addr];
}


gboolean
eef_is_call_instr (eef_data_t op)
{
  switch (op)
	{
	case 0xd4:
	case 0xc4:
	case 0xf4:
	case 0xec:
	case 0xfe:
	case 0xe4:
	case 0xcc:
	case 0xcd:
	case 0xdc:
	case 0xfc:
	  return TRUE;
	}
  return FALSE;
}

gboolean
eef_is_ret_instr (eef_data_t op)
{
  return op == 0xC9;
}

gchar
eef_regpair_get_another (gchar x)
{
  switch (x)
	{
	case 'B':
	  return 'C';
	case 'D':
	  return 'E';
	case 'H':
	  return 'L';
	case 'S':
	  return 'P';
	}
  g_assert_not_reached ();
  return 0; /* supress warning */
}

/* Position of flag in flag register 
   sz0h0p1c
*/
#define C_FLAG_POSITION 0x01
#define AC_FLAG_POSITION 0x10
#define P_FLAG_POSITION 0x04
#define Z_FLAG_POSITION 0x40
#define S_FLAG_POSITION 0x80
/* Default value of flag register */
#define DEFAULT_FLAG 0x02

eef_data_t 
eef_flag_to_data (EefFlag flag)
{
  eef_data_t td;
  td = DEFAULT_FLAG;
  td |= (0 == flag.c) ? 0 : C_FLAG_POSITION;
  td |= (0 == flag.ac) ? 0 : AC_FLAG_POSITION;
  td |= (0 == flag.p) ? 0 : P_FLAG_POSITION;
  td |= (0 == flag.z) ? 0 : Z_FLAG_POSITION;
  td |= (0 == flag.s) ? 0 : S_FLAG_POSITION;
        
  return td;
  /*return DEFAULT_FLAG | 
	((0 == flag.c) ? 0 : C_FLAG_POSITION) |
	((0 == flag.ac) ? 0 : AC_FLAG_POSITION) | 
	((0 == flag.p) ? 0 : P_FLAG_POSITION) | 
	((0 == flag.z) ? 0 : Z_FLAG_POSITION) | 
	((0 == flag.s) ? 0 : S_FLAG_POSITION);*/
}

//EefFlag *
void
eef_data_to_flag (eef_data_t from, EefFlag *flag)
{       
  /*	flag.c  = (0 == (from && C_FLAG_POSITION)) ? 0 : 1;
		flag.ac = (0 == (from && AC_FLAG_POSITION)) ? 0 : 1;
		flag.p  = (0 == (from && P_FLAG_POSITION)) ? 0 : 1;
		flag.s  = (0 == (from && S_FLAG_POSITION)) ? 0 : 1;
		flag.z  = (0 == (from && Z_FLAG_POSITION)) ? 0 : 1;*/

  //	EefFlag * flag;
	
  flag->c  = (0 == (from & C_FLAG_POSITION)) ? 0 : 1;
  flag->ac = (0 == (from & AC_FLAG_POSITION)) ? 0 : 1;
  flag->p  = (0 == (from & P_FLAG_POSITION)) ? 0 : 1;
  flag->s  = (0 == (from & S_FLAG_POSITION)) ? 0 : 1;
  flag->z  = (0 == (from & Z_FLAG_POSITION)) ? 0 : 1; 
	
  //	return flag;
}
