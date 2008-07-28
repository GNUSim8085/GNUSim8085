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
//Instructions TODO: RIM, SIM, ~RST

#ifdef HAVE_CONFIG_
#  include <config.h>
#endif

#include "8085-instructions.h"

#define ERR_IV_OP _("Invalid Opcode")

void _eef_inst_assign_all (void);
void eef_call_stack_cb (gboolean pushed, gchar x);	/* refer 8085.c */

typedef gint (*eef_inst_func_t) (eef_addr_t opnd_addr);

eef_inst_func_t eef_instructions[MAX_INSTS];
gboolean eef_instructions_assigned = FALSE;

gboolean
eef_inst_is_halt_inst (eef_data_t op)
{
  switch (op)
	{
	case 0x76:
	case 0xc7:
	case 0xcf:
	case 0xd7:
	case 0xdf:
	case 0xe7:
	case 0xef:
	case 0xff:
	  return TRUE;
	}

  return FALSE;
}

gboolean
eef_inst_execute (eef_addr_t addr,
				  eef_addr_t * consumed_bytes, gboolean * is_halt_inst)
{
  eef_data_t op;		/* opcode */
  eef_addr_t returned_value;

  /* init */
  EEF_SET_PTR (consumed_bytes, 1);
  EEF_SET_PTR (is_halt_inst, FALSE);

  /* init inst. table */
  if (G_UNLIKELY (eef_instructions_assigned == FALSE))
	_eef_inst_assign_all ();

  /* get op */
  op = sys.mem[addr];

  /* call */
  returned_value = eef_instructions[op] (addr + 1);

  /* is halt */
  EEF_SET_PTR (is_halt_inst, eef_inst_is_halt_inst (op));

  if (consumed_bytes)
	{
	  *consumed_bytes = returned_value;
	  /* propogate error */
	  if (*consumed_bytes > 2)
		return FALSE;
	}
  return TRUE;
}

/* FLAG RELATED STUFFS */

/* check for carry in this operation */
static gboolean
_eef_is_carry (eef_data_t a, eef_data_t b, gchar op /* '+' or '-' */ )
{
  eef_addr_t big_a, big_b;
  g_assert (op == '+' || op == '-');

  big_a = a;
  big_b = b;

  if (op == '+')
	return !((big_a + big_b) < (eef_addr_t) (EEF_DATA_T_MAX + 1));
  else{
	//return !((big_a - big_b) <= big_a);
	return big_a < big_b;
  }
}

/* check AC */
gboolean
_eef_is_auxillary_carry (eef_data_t a, eef_data_t b, gchar op)
{
  /* zero the higher 4 bits */
  a <<= 4;
  a >>= 4;
  b <<= 4;
  b >>= 4;

  if (op == '+')
	return !((a + b) < 16);
  else
	return !((a - b) <= a);
}

/* count ones */
static gint
_eef_count_one_bits (eef_data_t data)
{
  eef_data_t mask = 1;
  gint cnt = 0;

  while (mask)
	{
	  if (data & mask)
		cnt++;
	  mask <<= 1;
	}
  return cnt;
}

/* give the result to this func and it will set flags except C, AC */
static void
_eef_find_and_set_flags (eef_data_t result)
{

  sys.flag.z = (result == 0);
  sys.flag.s =
	(result >=
	 (eef_data_t) (((eef_addr_t) (EEF_DATA_T_MAX + 1)) / 2));

  sys.flag.p = !(_eef_count_one_bits (result) % 2);
}

static void
_eef_flag_check_and_set_aux_c (eef_data_t a, eef_data_t b, gchar op)
{
  sys.flag.ac = _eef_is_auxillary_carry (a, b, op);
}

static void
_eef_flag_check_and_set_c (eef_data_t a, eef_data_t b, gchar op)
{
  sys.flag.c = _eef_is_carry (a, b, op);
}

/***************************************************************/
/************* BEGINNING OF INSTRUCTION DEFINITIONS ************/
/***************************************************************/


static gint
_eef_inst_func_lxi (eef_addr_t opnd_addr, gchar x)
{
  gint bytes_consumed = 0;
  eef_addr_t data;

  data = eef_mem16_get (opnd_addr);
  eef_regpair_put (x, data);
  bytes_consumed = 2;

  return bytes_consumed;
}

/* 00 NOP */
static gint
_eef_inst_func_0 (eef_addr_t opnd_addr)
{
  return 0;
}



/* 01 LXI B */
static gint
_eef_inst_func_1 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_lxi (opnd_addr, 'B');
}

static gint
_eef_inst_func_stax (eef_addr_t opnd_addr, gchar x)
{
  gint bytes_consumed = 0;
  eef_addr_t ta;

  ta = eef_regpair_get (x);
  sys.mem[ta] = sys.reg.a;

  return bytes_consumed;
}

/* 02 STAX B */
static gint
_eef_inst_func_2 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_stax (opnd_addr, 'B');
}

static gint
_eef_inst_func_incdec_reg (eef_addr_t opnd_addr, gchar r, gboolean is_inc)
{
  gint bytes_consumed = 0;
  gint dx = -1;
  eef_data_t *rloc;

  if (is_inc)
	dx = +1;
  rloc = eef_loc_of_reg (r);
  /* if memory loc */
  if (!rloc)
	{
	  /* use from hl pair */
	  eef_addr_t ta;
	  ta = eef_regpair_get ('H');
	  _eef_flag_check_and_set_aux_c (sys.mem[ta], 1,
									 is_inc ? '+' : '-');
	  sys.mem[ta] += dx;
	  _eef_find_and_set_flags (sys.mem[ta]);
	}
  else
	{
	  _eef_flag_check_and_set_aux_c (*rloc, 1, is_inc ? '+' : '-');
	  *rloc += dx;
	  _eef_find_and_set_flags (*rloc);
	}


  return bytes_consumed;
}

static gint
_eef_inst_func_incdec_regpair (eef_addr_t opnd_addr, gchar r, gboolean is_inc)
{
  gint bytes_consumed = 0;
  gint dx = -1;
  eef_data_t *rloc_h, *rloc_l;

  if (is_inc)
	dx = +1;
  eef_loc_of_regpair (r, &rloc_h, &rloc_l);
  *rloc_l += dx;
  if (*rloc_l == 0xFF && dx == -1)
	*rloc_h += dx;
  else if (*rloc_l == 0x00 && dx == +1)
	*rloc_h += dx;


  return bytes_consumed;
}

static gint
_eef_inst_func_mov (eef_addr_t opnd_addr, gchar dst, gchar src)
{
  eef_data_t *src_ptr, *dst_ptr;
  eef_data_t src_data, *dst_data;

  /* get ptrs of regs else NULL */
  src_ptr = eef_loc_of_reg (src);
  dst_ptr = eef_loc_of_reg (dst);

  /* load and refer data */
  src_data = (src_ptr) ? *src_ptr : sys.mem[eef_regpair_get ('H')];
  dst_data = (dst_ptr) ? dst_ptr : &sys.mem[eef_regpair_get ('H')];

  /* copy data */
  *dst_data = src_data;

  return 0;
}

static gint
_eef_inst_func_add_i (eef_addr_t opnd_addr, eef_data_t data, gchar op)
{
  /* check for flags */
  _eef_flag_check_and_set_c (sys.reg.a, data, op);
  _eef_flag_check_and_set_aux_c (sys.reg.a, data, op);

  /* add */
  sys.reg.a += (op == '+') ? data : -1 * data;

  _eef_find_and_set_flags (sys.reg.a);

  return 0;
}


static gint
_eef_inst_func_add (eef_addr_t opnd_addr, gchar sec, gchar op)
{
  eef_data_t data;
  eef_data_t *data_ptr;

  /* find operand data */
  data_ptr = eef_loc_of_reg (sec);
  if (data_ptr)
	data = *data_ptr;
  else
	data = sys.mem[eef_regpair_get ('H')];

  _eef_inst_func_add_i (opnd_addr, data, op);

  return 0;
}

static gint
_eef_inst_func_add_with_carry_i (eef_addr_t opnd_addr, eef_data_t data,
								 gchar op)
{
  eef_data_t data_1;

  /* I'm not sure abt the new code
   * Old code:
   if (op == '+')
   data_1 = data + sys.flag.c;
   else
   data_1 = data - sys.flag.c;
  */
  data_1 = data + sys.flag.c;
	
  /* check for flags */
  sys.flag.c = 0;
  sys.flag.c = (_eef_is_carry (sys.reg.a, data, op)
				|| _eef_is_carry (sys.reg.a, data_1, op));

  sys.flag.ac = (_eef_is_auxillary_carry (sys.reg.a, data, op) ||
				 _eef_is_auxillary_carry (sys.reg.a, data_1, op));

  /* add */
  sys.reg.a += (op == '+') ? data_1 : -1 * data_1;

  _eef_find_and_set_flags (sys.reg.a);

  /* sys.flag.c = 0;  Oops! misunderstood! */
  return 0;
}

static gint
_eef_inst_func_add_with_carry (eef_addr_t opnd_addr, gchar sec, gchar op)
{
  eef_data_t data;
  eef_data_t *data_ptr;

  /* find operand data */
  data_ptr = eef_loc_of_reg (sec);
  if (data_ptr)
	data = *data_ptr;
  else
	data = sys.mem[eef_regpair_get ('H')];

  _eef_inst_func_add_with_carry_i (opnd_addr, data, op);

  return 0;
}

static gint
_eef_inst_func_mvi (eef_addr_t opnd_addr, gchar dst)
{
  eef_data_t *dst_ptr;
  eef_data_t *dst_data;

  /* get ptrs of regs else NULL */
  dst_ptr = eef_loc_of_reg (dst);

  /* load and refer data */
  dst_data = (dst_ptr) ? dst_ptr : &sys.mem[eef_regpair_get ('H')];

  /* copy data */
  *dst_data = sys.mem[opnd_addr];

  return 1;
}

static gint
_eef_inst_func_dad (eef_addr_t opnd_addr, gchar sec)
{
  eef_data_t *dst_h, *dst_l;
  eef_addr_t h_data, sec_data;

  eef_loc_of_regpair (sec, &dst_h, &dst_l);
  if (dst_h == NULL)	/* M not supported */
	{
	  g_warning (ERR_IV_OP);
	  return -1;
	}

  /* load data */
  h_data = eef_regpair_get ('H');
  sec_data = eef_join16 (*dst_h, *dst_l);

  _eef_flag_check_and_set_c (h_data, sec_data, '+');

  eef_regpair_put ('H', h_data + sec_data);

  return 0;
}

static eef_data_t
_eef_util_get_val_from_rm_spec (eef_addr_t opnd_addr, gchar sec)
{
  eef_data_t *ta;

  ta = eef_loc_of_reg (sec);
  if (ta)
	return *ta;

  return sys.mem[eef_regpair_get ('H')];
}

static gint
_eef_inst_func_ana_i (eef_addr_t opnd_addr, eef_data_t data)
{
  sys.flag.c = 0;
  sys.flag.ac = 1;

  sys.reg.a &= data;

  _eef_find_and_set_flags (sys.reg.a);

  return 0;
}

static gint
_eef_inst_func_ana (eef_addr_t opnd_addr, gchar sec)
{
  _eef_inst_func_ana_i (opnd_addr,
						_eef_util_get_val_from_rm_spec (opnd_addr,
														sec));

  return 0;
}

static gint
_eef_inst_func_ora_i (eef_addr_t opnd_addr, eef_data_t data)
{
  sys.flag.c = 0;
  sys.flag.ac = 0;

  sys.reg.a |= data;

  _eef_find_and_set_flags (sys.reg.a);

  return 0;
}

static gint
_eef_inst_func_ora (eef_addr_t opnd_addr, gchar sec)
{
  _eef_inst_func_ora_i (opnd_addr,
						_eef_util_get_val_from_rm_spec (opnd_addr,
														sec));

  return 0;
}

static gint
_eef_inst_func_xra_i (eef_addr_t opnd_addr, eef_addr_t data)
{
  sys.flag.c = 0;
  sys.flag.ac = 0;

  sys.reg.a ^= data;

  _eef_find_and_set_flags (sys.reg.a);

  return 0;
}

static gint
_eef_inst_func_xra (eef_addr_t opnd_addr, gchar sec)
{
  _eef_inst_func_xra_i (opnd_addr,
						_eef_util_get_val_from_rm_spec (opnd_addr,
														sec));

  return 0;
}

static gint
_eef_inst_func_cmp_i (eef_addr_t opnd_addr, eef_data_t data)
{
  _eef_find_and_set_flags (sys.reg.a);

  sys.flag.s = (sys.reg.a < data);

  _eef_flag_check_and_set_aux_c (sys.reg.a, data, '-');

  if (sys.reg.a < data)
	sys.flag.c = 1, sys.flag.z = 0;
  else if (sys.reg.a > data)
	sys.flag.c = 0, sys.flag.z = 0;
  else
	sys.flag.c = 0, sys.flag.z = 1;

  return 0;
}

static gint
_eef_inst_func_cmp (eef_addr_t opnd_addr, gchar sec)
{
  _eef_inst_func_cmp_i (opnd_addr,
						_eef_util_get_val_from_rm_spec (opnd_addr,
														sec));

  return 0;
}

static gint
_eef_inst_func_push (eef_addr_t opnd_addr, gchar x)
{
  eef_data_t *high, *low;

  if ('P' != x)
	{
	  eef_loc_of_regpair (x, &high, &low);
	  g_assert (high);

	  eef_stack_push_byte (*high);
	  eef_stack_push_byte (*low);	
	} 
  else 
	{		
	  eef_stack_push_byte (sys.reg.a);
	  eef_stack_push_byte (eef_flag_to_data(sys.flag));
	}
	
  /* stack cb */
  eef_call_stack_cb (TRUE, x);

  return 0;
}

static gint
_eef_inst_func_pop (eef_addr_t opnd_addr, gchar x)
{
  eef_data_t *high, *low;

  if ('P' != x)
	{
	  eef_loc_of_regpair (x, &high, &low);
	  g_assert (high);

	  *low = eef_stack_pop_byte ();
	  *high = eef_stack_pop_byte ();

	} 
  else 
	{
	  eef_data_to_flag(eef_stack_pop_byte (), &sys.flag);
	  sys.reg.a = eef_stack_pop_byte ();
	}
	
  /* stack cb */
  eef_call_stack_cb (FALSE, x);

  return 0;
}

static gint
_eef_inst_func_rst (eef_addr_t opnd_addr, gint num /* 0-7 */ )
{
  g_assert (num >= 0 && num < 8);

  /* See first line of this file */
  g_warning ("Instruction 'RST' Not implemented (currently halt the \
execution like 'hlt')");
  return 0;
}

/* 03 INX B */
static gint
_eef_inst_func_3 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'B', TRUE);
}

/* 03 INR B */
static gint
_eef_inst_func_4 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'B', TRUE);
}

/* 05 DCR B */
static gint
_eef_inst_func_5 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'B', FALSE);
}

/* 06 MVI B */
static gint
_eef_inst_func_6 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'B');
}

/* 07 RLC */
static gint
_eef_inst_func_7 (eef_addr_t opnd_addr)
{
  if (sys.reg.a & 128)
	sys.flag.c = 1;
  else
	sys.flag.c = 0;

  sys.reg.a <<= 1;
  sys.reg.a |= (sys.flag.c) ? 1 : 0;
  return 0;
}

static gint
_eef_inst_func_jmpc (eef_addr_t opnd_addr, eef_data_t flag_bit, gboolean val)
{
  if (flag_bit == val)
	{
	  /* jump */
	  sys.reg.pcl = sys.mem[opnd_addr];
	  sys.reg.pch = sys.mem[opnd_addr + 1];
	}

  return 2;
}

static gint
_eef_inst_func_callc (eef_addr_t opnd_addr, eef_data_t flag_bit, gboolean val)
{
  if (flag_bit == val)
  { 
    eef_save_pc_into_stack ();
  }
  _eef_inst_func_jmpc (opnd_addr, flag_bit, val);

  return 2;
}

static gint
_eef_inst_func_retc (eef_addr_t opnd_addr, eef_data_t flag_bit, gboolean val)
{
  if (flag_bit == val)
	eef_peek_pc_from_stack ();

  return 0;
}

static gint
_eef_inst_func_8 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* 09 DAD B */
static gint
_eef_inst_func_9 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_dad (opnd_addr, 'B');
}

/* 0A LDAX B ==> 1A*/
static gint
_eef_inst_func_10 (eef_addr_t opnd_addr)
{
  gint bytes_consumed = 0;

  sys.reg.a = sys.mem[eef_regpair_get ('B')];

  return bytes_consumed;
}

/* 0B DCX B */
static gint
_eef_inst_func_11 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'B', FALSE);
}

/* 0C INR C */
static gint
_eef_inst_func_12 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'C', TRUE);
}

/* 0D DCR C */
static gint
_eef_inst_func_13 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'C', FALSE);
}

/* 0E MVI C */
static gint
_eef_inst_func_14 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'C');
}

/* 0F RRC */
static gint
_eef_inst_func_15 (eef_addr_t opnd_addr)
{
  if (sys.reg.a & 1)
	sys.flag.c = 1;
  else
	sys.flag.c = 0;

  sys.reg.a >>= 1;
  sys.reg.a |= (sys.flag.c) ? 128 : 0;

  return 0;
}

static gint
_eef_inst_func_16 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* 11 LXI B */
static gint
_eef_inst_func_17 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_lxi (opnd_addr, 'D');
}

/* 12 STAX H */
static gint
_eef_inst_func_18 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_stax (opnd_addr, 'D');
}

/* 13 INX D */
static gint
_eef_inst_func_19 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'D', TRUE);
}

/* 14 INR D */
static gint
_eef_inst_func_20 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'D', TRUE);
}

/* 15 DCR D */
static gint
_eef_inst_func_21 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'D', FALSE);
}

/* 16 MVI D */
static gint
_eef_inst_func_22 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'D');
}

/* 17 RAL */
static gint
_eef_inst_func_23 (eef_addr_t opnd_addr)
{
  gboolean pc = sys.flag.c;
  // if (sys.reg.a & 255)
  if (sys.reg.a & 128)
	sys.flag.c = 1;
  else
	sys.flag.c = 0;

  sys.reg.a <<= 1;
  // debug sys.reg.a |= (pc) ? 128 : 0;
  if (pc)
	sys.reg.a |= 1;
  else
	sys.reg.a &= (~1);

  return 0;
}

static gint
_eef_inst_func_24 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* 19 DAD D */
static gint
_eef_inst_func_25 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_dad (opnd_addr, 'D');
}

/* 1A LDAX D <== 0A */
static gint
_eef_inst_func_26 (eef_addr_t opnd_addr)
{
  gint bytes_consumed = 0;

  sys.reg.a = sys.mem[eef_regpair_get ('D')];

  return bytes_consumed;
}

/* 1B DCX D */
static gint
_eef_inst_func_27 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'D', FALSE);
}

/* 1C INR E */
static gint
_eef_inst_func_28 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'E', TRUE);
}

/* 1D DCR E */
static gint
_eef_inst_func_29 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'E', FALSE);
}

/* 1E MVI E */
static gint
_eef_inst_func_30 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'E');
}

/* 1F RAR */
static gint
_eef_inst_func_31 (eef_addr_t opnd_addr)
{
  gboolean pc = sys.flag.c;
  if (sys.reg.a & 1)
	sys.flag.c = 1;
  else
	sys.flag.c = 0;

  sys.reg.a >>= 1;
  // debug sys.reg.a |= ( (pc) ? 128 : 0 );
  if (pc)
	sys.reg.a |= 128;
  else
	sys.reg.a &= (~128);

  return 0;
}

/* 20 RIM */
static gint
_eef_inst_func_32 (eef_addr_t opnd_addr)
{
  //TODO (page 748)
  g_warning (_("Instruction 'RIM' Not implemented"));
  return 0;
}

/* 21 LXI H */
static gint
_eef_inst_func_33 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_lxi (opnd_addr, 'H');
}

/* 22 SHLD addr */
static gint
_eef_inst_func_34 (eef_addr_t opnd_addr)
{
  gint bytes_consumed = 2;

  eef_mem16_put (eef_swap_bytes_in_addr (opnd_addr),
				 eef_regpair_get ('H'));

  return bytes_consumed;
}

/* 23 INX H */
static gint
_eef_inst_func_35 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'H', TRUE);
}

/* 24 INR H */
static gint
_eef_inst_func_36 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'H', TRUE);
}

/* 25 DCR H */
static gint
_eef_inst_func_37 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'H', FALSE);
}

/* 26 MVI H */
static gint
_eef_inst_func_38 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'H');
}

/* 27 DAA */
static gint
_eef_inst_func_39 (eef_addr_t opnd_addr)
{
  eef_data_t low;
  eef_data_t high;

  low = sys.reg.a & 0x0F;

  if (low > 9 || sys.flag.ac)
	{
	  sys.reg.a += 6;
	  if((low + 6) > 9)
		sys.flag.ac = 1;     
	  else 
		sys.flag.ac = 0;
	}
  high = sys.reg.a >> 4;
  if (high > 9 || sys.flag.c)
	{
	  sys.reg.a += (6 << 4);
	  if((high + 6) > 9)
		sys.flag.c = 1;
	  else 
		sys.flag.c = 0;
	}
  return 0;
}

static gint
_eef_inst_func_40 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* 29 DAD H */
static gint
_eef_inst_func_41 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_dad (opnd_addr, 'H');
}

/* 2A LHLD addr */
static gint
_eef_inst_func_42 (eef_addr_t opnd_addr)
{
  gint bytes_consumed = 2;
  eef_addr_t st;

  /* bug sys.reg.l = sys.mem[sys.mem[opnd_addr]];
   * sys.reg.h = sys.mem[sys.mem[opnd_addr + 1] + 1]; */

  st = eef_mem16_get (opnd_addr);
  sys.reg.l = sys.mem[st];
  sys.reg.h = sys.mem[st + 1];

  return bytes_consumed;
}

/* 2B DCX H */
static gint
_eef_inst_func_43 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'H', FALSE);
}

/* 2C INR L */
static gint
_eef_inst_func_44 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'L', TRUE);
}

/* 2D DCR L */
static gint
_eef_inst_func_45 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'L', FALSE);
}

/* 2E MVI L */
static gint
_eef_inst_func_46 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'L');
}

/* 2F CMA */
static gint
_eef_inst_func_47 (eef_addr_t opnd_addr)
{
  sys.reg.a = ~sys.reg.a;

  return 0;
}

/* 30 SIM */
static gint
_eef_inst_func_48 (eef_addr_t opnd_addr)
{
  //TODO (page 752)
  g_warning (_("Instruction 'SIM' Not implemented"));
  return 0;
}

/* 31 LXI SP */
static gint
_eef_inst_func_49 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_lxi (opnd_addr, 'S');
}

/* 32 STA addr */
static gint
_eef_inst_func_50 (eef_addr_t opnd_addr)
{
  gint bytes_consumed = 2;

  sys.mem[eef_swap_bytes_in_addr (opnd_addr)] = sys.reg.a;

  return bytes_consumed;
}

/* 33 INX SP */
static gint
_eef_inst_func_51 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'S', TRUE);
}

/* 34 INR M */
static gint
_eef_inst_func_52 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'M', TRUE);
}

/* 35 DCR M */
static gint
_eef_inst_func_53 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'M', FALSE);
}

/* 36 MVI M */
static gint
_eef_inst_func_54 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'M');
}

/* 37 STC */
static gint
_eef_inst_func_55 (eef_addr_t opnd_addr)
{
  sys.flag.c = 1;

  return 0;
}

static gint
_eef_inst_func_56 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* 39 DAD SP */
static gint
_eef_inst_func_57 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_dad (opnd_addr, 'S');
}

/* 3A LDA addr */
static gint
_eef_inst_func_58 (eef_addr_t opnd_addr)
{
  gint bytes_consumed = 2;

  sys.reg.a = sys.mem[eef_mem16_get (opnd_addr)];

  return bytes_consumed;
}

/* 3B DCX SP */
static gint
_eef_inst_func_59 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_regpair (opnd_addr, 'S', FALSE);
}

/* 3C INR A */
static gint
_eef_inst_func_60 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'A', TRUE);
}

/* 3D DCR A */
static gint
_eef_inst_func_61 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_incdec_reg (opnd_addr, 'A', FALSE);
}

/* 3E MVI A */
static gint
_eef_inst_func_62 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mvi (opnd_addr, 'A');
}

/* 3F CMC */
static gint
_eef_inst_func_63 (eef_addr_t opnd_addr)
{
  sys.flag.c = !sys.flag.c;

  return 0;
}

/* MOV inst starts here -------------- */
static gint
_eef_inst_func_64 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'B');
}

static gint
_eef_inst_func_65 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'C');
}

static gint
_eef_inst_func_66 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'D');
}

static gint
_eef_inst_func_67 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'E');
}

static gint
_eef_inst_func_68 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'H');
}

static gint
_eef_inst_func_69 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'L');
}

static gint
_eef_inst_func_70 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'M');
}

static gint
_eef_inst_func_71 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'B', 'A');
}

static gint
_eef_inst_func_72 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'B');
}

static gint
_eef_inst_func_73 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'C');
}

static gint
_eef_inst_func_74 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'D');
}

static gint
_eef_inst_func_75 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'E');
}

static gint
_eef_inst_func_76 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'H');
}

static gint
_eef_inst_func_77 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'L');
}

static gint
_eef_inst_func_78 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'M');
}

static gint
_eef_inst_func_79 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'C', 'A');
}

static gint
_eef_inst_func_80 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'B');
}

static gint
_eef_inst_func_81 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'C');
}

static gint
_eef_inst_func_82 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'D');
}

static gint
_eef_inst_func_83 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'E');
}

static gint
_eef_inst_func_84 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'H');
}

static gint
_eef_inst_func_85 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'L');
}

static gint
_eef_inst_func_86 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'M');
}

static gint
_eef_inst_func_87 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'D', 'A');
}

static gint
_eef_inst_func_88 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'B');
}

static gint
_eef_inst_func_89 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'C');
}

static gint
_eef_inst_func_90 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'D');
}

static gint
_eef_inst_func_91 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'E');
}

static gint
_eef_inst_func_92 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'H');
}

static gint
_eef_inst_func_93 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'L');
}

static gint
_eef_inst_func_94 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'M');
}

static gint
_eef_inst_func_95 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'E', 'A');
}

static gint
_eef_inst_func_96 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'B');
}

static gint
_eef_inst_func_97 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'C');
}

static gint
_eef_inst_func_98 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'D');
}

static gint
_eef_inst_func_99 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'E');
}

static gint
_eef_inst_func_100 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'H');
}

static gint
_eef_inst_func_101 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'L');
}

static gint
_eef_inst_func_102 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'M');
}

static gint
_eef_inst_func_103 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'H', 'A');
}

static gint
_eef_inst_func_104 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'B');
}

static gint
_eef_inst_func_105 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'C');
}

static gint
_eef_inst_func_106 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'D');
}

static gint
_eef_inst_func_107 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'E');
}

static gint
_eef_inst_func_108 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'H');
}

static gint
_eef_inst_func_109 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'L');
}

static gint
_eef_inst_func_110 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'M');
}

static gint
_eef_inst_func_111 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'L', 'A');
}

static gint
_eef_inst_func_112 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'B');
}

static gint
_eef_inst_func_113 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'C');
}

static gint
_eef_inst_func_114 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'D');
}

static gint
_eef_inst_func_115 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'E');
}

static gint
_eef_inst_func_116 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'H');
}

static gint
_eef_inst_func_117 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'L');
}

/* 76 HLT */
static gint
_eef_inst_func_118 (eef_addr_t opnd_addr)
{
  return 0;
}

static gint
_eef_inst_func_119 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'M', 'A');
}

static gint
_eef_inst_func_120 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'B');
}

static gint
_eef_inst_func_121 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'C');
}

static gint
_eef_inst_func_122 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'D');
}

static gint
_eef_inst_func_123 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'E');
}

static gint
_eef_inst_func_124 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'H');
}

static gint
_eef_inst_func_125 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'L');
}

static gint
_eef_inst_func_126 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'M');
}

static gint
_eef_inst_func_127 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_mov (opnd_addr, 'A', 'A');
}

/* End of Mov inst. ------------------- */


/* ADD begin ----- */
static gint
_eef_inst_func_128 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'B', '+');
}

static gint
_eef_inst_func_129 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'C', '+');
}

static gint
_eef_inst_func_130 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'D', '+');
}

static gint
_eef_inst_func_131 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'E', '+');
}

static gint
_eef_inst_func_132 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'H', '+');
}

static gint
_eef_inst_func_133 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'L', '+');
}

static gint
_eef_inst_func_134 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'M', '+');
}

static gint
_eef_inst_func_135 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'A', '+');
}

/* end of ADD ------ */

/* ADC begin ----- */
static gint
_eef_inst_func_136 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'B', '+');
}

static gint
_eef_inst_func_137 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'C', '+');
}

static gint
_eef_inst_func_138 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'D', '+');
}

static gint
_eef_inst_func_139 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'E', '+');
}

static gint
_eef_inst_func_140 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'H', '+');
}

static gint
_eef_inst_func_141 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'L', '+');
}

static gint
_eef_inst_func_142 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'M', '+');
}

static gint
_eef_inst_func_143 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'A', '+');
}

/* end of ADC ----- */

/* SUB begin ... */
static gint
_eef_inst_func_144 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'B', '-');
}

static gint
_eef_inst_func_145 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'C', '-');
}

static gint
_eef_inst_func_146 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'D', '-');
}

static gint
_eef_inst_func_147 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'E', '-');
}

static gint
_eef_inst_func_148 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'H', '-');
}

static gint
_eef_inst_func_149 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'L', '-');
}

static gint
_eef_inst_func_150 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'M', '-');
}

static gint
_eef_inst_func_151 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add (opnd_addr, 'A', '-');
}

/* end of SUB .... */

/* SBB begin .... */
static gint
_eef_inst_func_152 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'B', '-');
}

static gint
_eef_inst_func_153 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'C', '-');
}

static gint
_eef_inst_func_154 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'D', '-');
}

static gint
_eef_inst_func_155 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'E', '-');
}

static gint
_eef_inst_func_156 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'H', '-');
}

static gint
_eef_inst_func_157 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'L', '-');
}

static gint
_eef_inst_func_158 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'M', '-');
}

static gint
_eef_inst_func_159 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_add_with_carry (opnd_addr, 'A', '-');
}

/* ANA begin .... */
static gint
_eef_inst_func_160 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'B');
}

static gint
_eef_inst_func_161 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'C');
}

static gint
_eef_inst_func_162 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'D');
}

static gint
_eef_inst_func_163 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'E');
}

static gint
_eef_inst_func_164 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'H');
}

static gint
_eef_inst_func_165 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'L');
}

static gint
_eef_inst_func_166 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'M');
}

static gint
_eef_inst_func_167 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ana (opnd_addr, 'A');
}

/* end of ANA..... */

/* XRA begin ... */
static gint
_eef_inst_func_168 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'B');
}

static gint
_eef_inst_func_169 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'C');
}

static gint
_eef_inst_func_170 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'D');
}

static gint
_eef_inst_func_171 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'E');
}

static gint
_eef_inst_func_172 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'H');
}

static gint
_eef_inst_func_173 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'L');
}

static gint
_eef_inst_func_174 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'M');
}

static gint
_eef_inst_func_175 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_xra (opnd_addr, 'A');
}

/* end of XRA ... */

/* ORA begin .... */
static gint
_eef_inst_func_176 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'B');
}

static gint
_eef_inst_func_177 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'C');
}

static gint
_eef_inst_func_178 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'D');
}

static gint
_eef_inst_func_179 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'E');
}

static gint
_eef_inst_func_180 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'H');
}

static gint
_eef_inst_func_181 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'L');
}

static gint
_eef_inst_func_182 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'M');
}

static gint
_eef_inst_func_183 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_ora (opnd_addr, 'A');
}

/* end of ORA */

/* CMP begin ... */
static gint
_eef_inst_func_184 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'B');
}

static gint
_eef_inst_func_185 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'C');
}

static gint
_eef_inst_func_186 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'D');
}

static gint
_eef_inst_func_187 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'E');
}

static gint
_eef_inst_func_188 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'H');
}

static gint
_eef_inst_func_189 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'L');
}

static gint
_eef_inst_func_190 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'M');
}

static gint
_eef_inst_func_191 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_cmp (opnd_addr, 'A');
}

/* end of CMP ... */

/* C0 RNZ */
static gint
_eef_inst_func_192 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.z, FALSE);
}

/* C1 POP B */
static gint
_eef_inst_func_193 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_pop (opnd_addr, 'B');
}

/* C2 JNZ */
static gint
_eef_inst_func_194 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.z, FALSE);
}

/* C3 JMP */
static gint
_eef_inst_func_195 (eef_addr_t opnd_addr)
{
  sys.reg.pcl = sys.mem[opnd_addr];
  sys.reg.pch = sys.mem[opnd_addr + 1];

  return 2;
}

/* C4 CNZ */
static gint
_eef_inst_func_196 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.z, FALSE);
}

/* C5 PUSH B */
static gint
_eef_inst_func_197 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_push (opnd_addr, 'B');
}

/* C6 ADI */
static gint
_eef_inst_func_198 (eef_addr_t opnd_addr)
{
  _eef_inst_func_add_i (opnd_addr, sys.mem[opnd_addr], '+');

  return 1;
}

/* C7 RST 0 */
static gint
_eef_inst_func_199 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 0);
}

/* C8 RZ */
static gint
_eef_inst_func_200 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.z, TRUE);
}

/* C9 RET */
static gint
_eef_inst_func_201 (eef_addr_t opnd_addr)
{
  eef_peek_pc_from_stack ();
  return 0;
}

/* CA JZ */
static gint
_eef_inst_func_202 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.z, TRUE);
}

static gint
_eef_inst_func_203 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* CC CZ */
static gint
_eef_inst_func_204 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.z, TRUE);
}

/* CD CALL */
static gint
_eef_inst_func_205 (eef_addr_t opnd_addr)
{
  eef_addr_t ta;

  ta = eef_mem16_get (opnd_addr);
  eef_save_pc_into_stack ();

  _eef_inst_func_jmpc (opnd_addr, TRUE, TRUE);

  return 2;
}

/* CE ACI */
static gint
_eef_inst_func_206 (eef_addr_t opnd_addr)
{
  _eef_inst_func_add_with_carry_i (opnd_addr, sys.mem[opnd_addr], '+');
  return 1;
}

/* CF RST 1 */
static gint
_eef_inst_func_207 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 1);
}

/* D0 RNC */
static gint
_eef_inst_func_208 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.c, FALSE);
}

/* D1 POP D */
static gint
_eef_inst_func_209 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_pop (opnd_addr, 'D');
}

/* D2 JNC */
static gint
_eef_inst_func_210 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.c, FALSE);
}

/* D3 OUT */
static gint
_eef_inst_func_211 (eef_addr_t opnd_addr)
{
  eef_port_put (sys.mem[opnd_addr], sys.reg.a);

  return 1;
}

/* D4 CNC */
static gint
_eef_inst_func_212 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.c, FALSE);
}

/* D5 PUSH D */
static gint
_eef_inst_func_213 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_push (opnd_addr, 'D');
}

/* D6 SUI */
static gint
_eef_inst_func_214 (eef_addr_t opnd_addr)
{
  _eef_inst_func_add_i (opnd_addr, sys.mem[opnd_addr], '-');

  return 1;
}

/* D7 RST 2 */
static gint
_eef_inst_func_215 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 2);
}

/* D8 RC */
static gint
_eef_inst_func_216 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.c, TRUE);
}

static gint
_eef_inst_func_217 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* DA JC */
static gint
_eef_inst_func_218 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.c, TRUE);
}

/* DB IN */
static gint
_eef_inst_func_219 (eef_addr_t opnd_addr)
{
  sys.reg.a = eef_port_get (sys.mem[opnd_addr]);
  return 1;
}

/* DC CC */
static gint
_eef_inst_func_220 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.c, TRUE);
}

static gint
_eef_inst_func_221 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* DE SBI */
static gint
_eef_inst_func_222 (eef_addr_t opnd_addr)
{
  _eef_inst_func_add_with_carry_i (opnd_addr, sys.mem[opnd_addr], '-');

  return 1;
}

/* DF RST 3 */
static gint
_eef_inst_func_223 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 3);
}

/* E0 RPO */
static gint
_eef_inst_func_224 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.p, FALSE);
}

/* E1 POP H */
static gint
_eef_inst_func_225 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_pop (opnd_addr, 'H');
}

/* E2 JPO */
static gint
_eef_inst_func_226 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.p, FALSE);
}

/* E3 XTHL */
static gint
_eef_inst_func_227 (eef_addr_t opnd_addr)
{
  eef_data_t tmp;

  tmp = sys.reg.l;
  sys.reg.l = sys.mem[eef_regpair_get ('S')];
  sys.mem[eef_regpair_get ('S')] = tmp;

  tmp = sys.reg.h;
  sys.reg.h = sys.mem[eef_regpair_get ('S') + 1];
  sys.mem[eef_regpair_get ('S') + 1] = tmp;
 
  return 0;
}

/* E4 CPO */
static gint
_eef_inst_func_228 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.p, FALSE);
}

/* E5 PUSH H */
static gint
_eef_inst_func_229 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_push (opnd_addr, 'H');
}

/* E6 ANI */
static gint
_eef_inst_func_230 (eef_addr_t opnd_addr)
{
  _eef_inst_func_ana_i (opnd_addr, sys.mem[opnd_addr]);

  return 1;
}

/* E7 RST 4 */
static gint
_eef_inst_func_231 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 4);
}

/* E8 RPE */
static gint
_eef_inst_func_232 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.p, TRUE);
}

/* E9 PCHL */
static gint
_eef_inst_func_233 (eef_addr_t opnd_addr)
{
  sys.reg.pch = sys.reg.h;
  sys.reg.pcl = sys.reg.l;

  return 0;
}

/* EA JPE */
static gint
_eef_inst_func_234 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.p, TRUE);
}

/* EB XCHG */
static gint
_eef_inst_func_235 (eef_addr_t opnd_addr)
{
  eef_data_t tmp;
  tmp = sys.reg.h;
  sys.reg.h = sys.reg.d;
  sys.reg.d = tmp;
	
  tmp = sys.reg.l;
  sys.reg.l = sys.reg.e;
  sys.reg.e = tmp;
  return 0;
}

/* EC CPE */
static gint
_eef_inst_func_236 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.p, TRUE);
}

static gint
_eef_inst_func_237 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* EE XRI */
static gint
_eef_inst_func_238 (eef_addr_t opnd_addr)
{
  _eef_inst_func_xra_i (opnd_addr, sys.mem[opnd_addr]);

  return 1;
}

/* EF RST 5 */
static gint
_eef_inst_func_239 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 5);
}

/* F0 RP */
static gint
_eef_inst_func_240 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.s, FALSE);
}

/* F1 POP PSW */
static gint
_eef_inst_func_241 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_pop (opnd_addr, 'P');
}

/* F2 JP */
static gint
_eef_inst_func_242 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.s, FALSE);
}

/* F3 DI */
static gint
_eef_inst_func_243 (eef_addr_t opnd_addr)
{
  eef_interrupt_enable (FALSE);

  return 0;
}

/* F4 CP */
static gint
_eef_inst_func_244 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.s, FALSE);
}

/* F5 PUSH PSW */
static gint
_eef_inst_func_245 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_push (opnd_addr, 'P');
}

/* F6 ORI */
static gint
_eef_inst_func_246 (eef_addr_t opnd_addr)
{
  _eef_inst_func_ora_i (opnd_addr, sys.mem[opnd_addr]);

  return 1;
}

/* F7 RST 6 */
static gint
_eef_inst_func_247 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 6);
}

/* F8 RM */
static gint
_eef_inst_func_248 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_retc (opnd_addr, sys.flag.s, TRUE);
}

/* F9 SPHL */
static gint
_eef_inst_func_249 (eef_addr_t opnd_addr)
{
  sys.reg.sph = sys.reg.h;
  sys.reg.spl = sys.reg.l;

  return 0;
}

/* FA JM */
static gint
_eef_inst_func_250 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_jmpc (opnd_addr, sys.flag.s, TRUE);
}

/* FB EI */
static gint
_eef_inst_func_251 (eef_addr_t opnd_addr)
{
  eef_interrupt_enable (TRUE);

  return 0;
}

/* FC CM */
static gint
_eef_inst_func_252 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_callc (opnd_addr, sys.flag.s, TRUE);
}

static gint
_eef_inst_func_253 (eef_addr_t opnd_addr)
{
  g_warning (ERR_IV_OP);
  return -1;
}

/* FE CPI */
static gint
_eef_inst_func_254 (eef_addr_t opnd_addr)
{
  _eef_inst_func_cmp_i (opnd_addr, sys.mem[opnd_addr]);

  return 1;
}

/* FF RST 7 */
static gint
_eef_inst_func_255 (eef_addr_t opnd_addr)
{
  return _eef_inst_func_rst (opnd_addr, 7);
}

void
_eef_inst_assign_all (void)
{

  eef_instructions[0] = _eef_inst_func_0;
  eef_instructions[1] = _eef_inst_func_1;
  eef_instructions[2] = _eef_inst_func_2;
  eef_instructions[3] = _eef_inst_func_3;
  eef_instructions[4] = _eef_inst_func_4;
  eef_instructions[5] = _eef_inst_func_5;
  eef_instructions[6] = _eef_inst_func_6;
  eef_instructions[7] = _eef_inst_func_7;
  eef_instructions[8] = _eef_inst_func_8;
  eef_instructions[9] = _eef_inst_func_9;
  eef_instructions[10] = _eef_inst_func_10;
  eef_instructions[11] = _eef_inst_func_11;
  eef_instructions[12] = _eef_inst_func_12;
  eef_instructions[13] = _eef_inst_func_13;
  eef_instructions[14] = _eef_inst_func_14;
  eef_instructions[15] = _eef_inst_func_15;
  eef_instructions[16] = _eef_inst_func_16;
  eef_instructions[17] = _eef_inst_func_17;
  eef_instructions[18] = _eef_inst_func_18;
  eef_instructions[19] = _eef_inst_func_19;
  eef_instructions[20] = _eef_inst_func_20;
  eef_instructions[21] = _eef_inst_func_21;
  eef_instructions[22] = _eef_inst_func_22;
  eef_instructions[23] = _eef_inst_func_23;
  eef_instructions[24] = _eef_inst_func_24;
  eef_instructions[25] = _eef_inst_func_25;
  eef_instructions[26] = _eef_inst_func_26;
  eef_instructions[27] = _eef_inst_func_27;
  eef_instructions[28] = _eef_inst_func_28;
  eef_instructions[29] = _eef_inst_func_29;
  eef_instructions[30] = _eef_inst_func_30;
  eef_instructions[31] = _eef_inst_func_31;
  eef_instructions[32] = _eef_inst_func_32;
  eef_instructions[33] = _eef_inst_func_33;
  eef_instructions[34] = _eef_inst_func_34;
  eef_instructions[35] = _eef_inst_func_35;
  eef_instructions[36] = _eef_inst_func_36;
  eef_instructions[37] = _eef_inst_func_37;
  eef_instructions[38] = _eef_inst_func_38;
  eef_instructions[39] = _eef_inst_func_39;
  eef_instructions[40] = _eef_inst_func_40;
  eef_instructions[41] = _eef_inst_func_41;
  eef_instructions[42] = _eef_inst_func_42;
  eef_instructions[43] = _eef_inst_func_43;
  eef_instructions[44] = _eef_inst_func_44;
  eef_instructions[45] = _eef_inst_func_45;
  eef_instructions[46] = _eef_inst_func_46;
  eef_instructions[47] = _eef_inst_func_47;
  eef_instructions[48] = _eef_inst_func_48;
  eef_instructions[49] = _eef_inst_func_49;
  eef_instructions[50] = _eef_inst_func_50;
  eef_instructions[51] = _eef_inst_func_51;
  eef_instructions[52] = _eef_inst_func_52;
  eef_instructions[53] = _eef_inst_func_53;
  eef_instructions[54] = _eef_inst_func_54;
  eef_instructions[55] = _eef_inst_func_55;
  eef_instructions[56] = _eef_inst_func_56;
  eef_instructions[57] = _eef_inst_func_57;
  eef_instructions[58] = _eef_inst_func_58;
  eef_instructions[59] = _eef_inst_func_59;
  eef_instructions[60] = _eef_inst_func_60;
  eef_instructions[61] = _eef_inst_func_61;
  eef_instructions[62] = _eef_inst_func_62;
  eef_instructions[63] = _eef_inst_func_63;
  eef_instructions[64] = _eef_inst_func_64;
  eef_instructions[65] = _eef_inst_func_65;
  eef_instructions[66] = _eef_inst_func_66;
  eef_instructions[67] = _eef_inst_func_67;
  eef_instructions[68] = _eef_inst_func_68;
  eef_instructions[69] = _eef_inst_func_69;
  eef_instructions[70] = _eef_inst_func_70;
  eef_instructions[71] = _eef_inst_func_71;
  eef_instructions[72] = _eef_inst_func_72;
  eef_instructions[73] = _eef_inst_func_73;
  eef_instructions[74] = _eef_inst_func_74;
  eef_instructions[75] = _eef_inst_func_75;
  eef_instructions[76] = _eef_inst_func_76;
  eef_instructions[77] = _eef_inst_func_77;
  eef_instructions[78] = _eef_inst_func_78;
  eef_instructions[79] = _eef_inst_func_79;
  eef_instructions[80] = _eef_inst_func_80;
  eef_instructions[81] = _eef_inst_func_81;
  eef_instructions[82] = _eef_inst_func_82;
  eef_instructions[83] = _eef_inst_func_83;
  eef_instructions[84] = _eef_inst_func_84;
  eef_instructions[85] = _eef_inst_func_85;
  eef_instructions[86] = _eef_inst_func_86;
  eef_instructions[87] = _eef_inst_func_87;
  eef_instructions[88] = _eef_inst_func_88;
  eef_instructions[89] = _eef_inst_func_89;
  eef_instructions[90] = _eef_inst_func_90;
  eef_instructions[91] = _eef_inst_func_91;
  eef_instructions[92] = _eef_inst_func_92;
  eef_instructions[93] = _eef_inst_func_93;
  eef_instructions[94] = _eef_inst_func_94;
  eef_instructions[95] = _eef_inst_func_95;
  eef_instructions[96] = _eef_inst_func_96;
  eef_instructions[97] = _eef_inst_func_97;
  eef_instructions[98] = _eef_inst_func_98;
  eef_instructions[99] = _eef_inst_func_99;
  eef_instructions[100] = _eef_inst_func_100;
  eef_instructions[101] = _eef_inst_func_101;
  eef_instructions[102] = _eef_inst_func_102;
  eef_instructions[103] = _eef_inst_func_103;
  eef_instructions[104] = _eef_inst_func_104;
  eef_instructions[105] = _eef_inst_func_105;
  eef_instructions[106] = _eef_inst_func_106;
  eef_instructions[107] = _eef_inst_func_107;
  eef_instructions[108] = _eef_inst_func_108;
  eef_instructions[109] = _eef_inst_func_109;
  eef_instructions[110] = _eef_inst_func_110;
  eef_instructions[111] = _eef_inst_func_111;
  eef_instructions[112] = _eef_inst_func_112;
  eef_instructions[113] = _eef_inst_func_113;
  eef_instructions[114] = _eef_inst_func_114;
  eef_instructions[115] = _eef_inst_func_115;
  eef_instructions[116] = _eef_inst_func_116;
  eef_instructions[117] = _eef_inst_func_117;
  eef_instructions[118] = _eef_inst_func_118;
  eef_instructions[119] = _eef_inst_func_119;
  eef_instructions[120] = _eef_inst_func_120;
  eef_instructions[121] = _eef_inst_func_121;
  eef_instructions[122] = _eef_inst_func_122;
  eef_instructions[123] = _eef_inst_func_123;
  eef_instructions[124] = _eef_inst_func_124;
  eef_instructions[125] = _eef_inst_func_125;
  eef_instructions[126] = _eef_inst_func_126;
  eef_instructions[127] = _eef_inst_func_127;
  eef_instructions[128] = _eef_inst_func_128;
  eef_instructions[129] = _eef_inst_func_129;
  eef_instructions[130] = _eef_inst_func_130;
  eef_instructions[131] = _eef_inst_func_131;
  eef_instructions[132] = _eef_inst_func_132;
  eef_instructions[133] = _eef_inst_func_133;
  eef_instructions[134] = _eef_inst_func_134;
  eef_instructions[135] = _eef_inst_func_135;
  eef_instructions[136] = _eef_inst_func_136;
  eef_instructions[137] = _eef_inst_func_137;
  eef_instructions[138] = _eef_inst_func_138;
  eef_instructions[139] = _eef_inst_func_139;
  eef_instructions[140] = _eef_inst_func_140;
  eef_instructions[141] = _eef_inst_func_141;
  eef_instructions[142] = _eef_inst_func_142;
  eef_instructions[143] = _eef_inst_func_143;
  eef_instructions[144] = _eef_inst_func_144;
  eef_instructions[145] = _eef_inst_func_145;
  eef_instructions[146] = _eef_inst_func_146;
  eef_instructions[147] = _eef_inst_func_147;
  eef_instructions[148] = _eef_inst_func_148;
  eef_instructions[149] = _eef_inst_func_149;
  eef_instructions[150] = _eef_inst_func_150;
  eef_instructions[151] = _eef_inst_func_151;
  eef_instructions[152] = _eef_inst_func_152;
  eef_instructions[153] = _eef_inst_func_153;
  eef_instructions[154] = _eef_inst_func_154;
  eef_instructions[155] = _eef_inst_func_155;
  eef_instructions[156] = _eef_inst_func_156;
  eef_instructions[157] = _eef_inst_func_157;
  eef_instructions[158] = _eef_inst_func_158;
  eef_instructions[159] = _eef_inst_func_159;
  eef_instructions[160] = _eef_inst_func_160;
  eef_instructions[161] = _eef_inst_func_161;
  eef_instructions[162] = _eef_inst_func_162;
  eef_instructions[163] = _eef_inst_func_163;
  eef_instructions[164] = _eef_inst_func_164;
  eef_instructions[165] = _eef_inst_func_165;
  eef_instructions[166] = _eef_inst_func_166;
  eef_instructions[167] = _eef_inst_func_167;
  eef_instructions[168] = _eef_inst_func_168;
  eef_instructions[169] = _eef_inst_func_169;
  eef_instructions[170] = _eef_inst_func_170;
  eef_instructions[171] = _eef_inst_func_171;
  eef_instructions[172] = _eef_inst_func_172;
  eef_instructions[173] = _eef_inst_func_173;
  eef_instructions[174] = _eef_inst_func_174;
  eef_instructions[175] = _eef_inst_func_175;
  eef_instructions[176] = _eef_inst_func_176;
  eef_instructions[177] = _eef_inst_func_177;
  eef_instructions[178] = _eef_inst_func_178;
  eef_instructions[179] = _eef_inst_func_179;
  eef_instructions[180] = _eef_inst_func_180;
  eef_instructions[181] = _eef_inst_func_181;
  eef_instructions[182] = _eef_inst_func_182;
  eef_instructions[183] = _eef_inst_func_183;
  eef_instructions[184] = _eef_inst_func_184;
  eef_instructions[185] = _eef_inst_func_185;
  eef_instructions[186] = _eef_inst_func_186;
  eef_instructions[187] = _eef_inst_func_187;
  eef_instructions[188] = _eef_inst_func_188;
  eef_instructions[189] = _eef_inst_func_189;
  eef_instructions[190] = _eef_inst_func_190;
  eef_instructions[191] = _eef_inst_func_191;
  eef_instructions[192] = _eef_inst_func_192;
  eef_instructions[193] = _eef_inst_func_193;
  eef_instructions[194] = _eef_inst_func_194;
  eef_instructions[195] = _eef_inst_func_195;
  eef_instructions[196] = _eef_inst_func_196;
  eef_instructions[197] = _eef_inst_func_197;
  eef_instructions[198] = _eef_inst_func_198;
  eef_instructions[199] = _eef_inst_func_199;
  eef_instructions[200] = _eef_inst_func_200;
  eef_instructions[201] = _eef_inst_func_201;
  eef_instructions[202] = _eef_inst_func_202;
  eef_instructions[203] = _eef_inst_func_203;
  eef_instructions[204] = _eef_inst_func_204;
  eef_instructions[205] = _eef_inst_func_205;
  eef_instructions[206] = _eef_inst_func_206;
  eef_instructions[207] = _eef_inst_func_207;
  eef_instructions[208] = _eef_inst_func_208;
  eef_instructions[209] = _eef_inst_func_209;
  eef_instructions[210] = _eef_inst_func_210;
  eef_instructions[211] = _eef_inst_func_211;
  eef_instructions[212] = _eef_inst_func_212;
  eef_instructions[213] = _eef_inst_func_213;
  eef_instructions[214] = _eef_inst_func_214;
  eef_instructions[215] = _eef_inst_func_215;
  eef_instructions[216] = _eef_inst_func_216;
  eef_instructions[217] = _eef_inst_func_217;
  eef_instructions[218] = _eef_inst_func_218;
  eef_instructions[219] = _eef_inst_func_219;
  eef_instructions[220] = _eef_inst_func_220;
  eef_instructions[221] = _eef_inst_func_221;
  eef_instructions[222] = _eef_inst_func_222;
  eef_instructions[223] = _eef_inst_func_223;
  eef_instructions[224] = _eef_inst_func_224;
  eef_instructions[225] = _eef_inst_func_225;
  eef_instructions[226] = _eef_inst_func_226;
  eef_instructions[227] = _eef_inst_func_227;
  eef_instructions[228] = _eef_inst_func_228;
  eef_instructions[229] = _eef_inst_func_229;
  eef_instructions[230] = _eef_inst_func_230;
  eef_instructions[231] = _eef_inst_func_231;
  eef_instructions[232] = _eef_inst_func_232;
  eef_instructions[233] = _eef_inst_func_233;
  eef_instructions[234] = _eef_inst_func_234;
  eef_instructions[235] = _eef_inst_func_235;
  eef_instructions[236] = _eef_inst_func_236;
  eef_instructions[237] = _eef_inst_func_237;
  eef_instructions[238] = _eef_inst_func_238;
  eef_instructions[239] = _eef_inst_func_239;
  eef_instructions[240] = _eef_inst_func_240;
  eef_instructions[241] = _eef_inst_func_241;
  eef_instructions[242] = _eef_inst_func_242;
  eef_instructions[243] = _eef_inst_func_243;
  eef_instructions[244] = _eef_inst_func_244;
  eef_instructions[245] = _eef_inst_func_245;

  eef_instructions[246] = _eef_inst_func_246;
  eef_instructions[247] = _eef_inst_func_247;
  eef_instructions[248] = _eef_inst_func_248;
  eef_instructions[249] = _eef_inst_func_249;
  eef_instructions[250] = _eef_inst_func_250;
  eef_instructions[251] = _eef_inst_func_251;
  eef_instructions[252] = _eef_inst_func_252;
  eef_instructions[253] = _eef_inst_func_253;
  eef_instructions[254] = _eef_inst_func_254;
  eef_instructions[255] = _eef_inst_func_255;

}



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@ EOF - Instructions @@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* returns the length of a instruction
 * similar to decoding stage of microprocessor
 */
gint
eef_instruction_length (eef_data_t op)
{
  /* 1, 2, 3 */
  switch (op)
	{

	case 0x0:
	  return 1;
	case 0x1:
	  return 3;
	case 0x2:
	  return 1;
	case 0x3:
	  return 1;
	case 0x4:
	  return 1;
	case 0x5:
	  return 1;
	case 0x6:
	  return 2;
	case 0x7:
	  return 1;
	case 0x8:
	  return 1;
	case 0x9:
	  return 1;
	case 0xA:
	  return 1;
	case 0xB:
	  return 1;
	case 0xC:
	  return 1;
	case 0xD:
	  return 1;
	case 0xE:
	  return 2;
	case 0xF:
	  return 1;
	case 0x10:
	  return 1;
	case 0x11:
	  return 3;
	case 0x12:
	  return 1;
	case 0x13:
	  return 1;
	case 0x14:
	  return 1;
	case 0x15:
	  return 1;
	case 0x16:
	  return 2;
	case 0x17:
	  return 1;
	case 0x18:
	  return 1;
	case 0x19:
	  return 1;
	case 0x1A:
	  return 1;
	case 0x1B:
	  return 1;
	case 0x1C:
	  return 1;
	case 0x1D:
	  return 1;
	case 0x1E:
	  return 2;
	case 0x1F:
	  return 1;
	case 0x20:
	  return 1;
	case 0x21:
	  return 3;
	case 0x22:
	  return 3;
	case 0x23:
	  return 1;
	case 0x24:
	  return 1;
	case 0x25:
	  return 1;
	case 0x26:
	  return 2;
	case 0x27:
	  return 1;
	case 0x28:
	  return 1;
	case 0x29:
	  return 1;
	case 0x2A:
	  return 3;
	case 0x2B:
	  return 1;
	case 0x2C:
	  return 1;
	case 0x2D:
	  return 1;
	case 0x2E:
	  return 2;
	case 0x2F:
	  return 1;
	case 0x30:
	  return 1;
	case 0x31:
	  return 3;
	case 0x32:
	  return 3;
	case 0x33:
	  return 1;
	case 0x34:
	  return 1;
	case 0x35:
	  return 1;
	case 0x36:
	  return 2;
	case 0x37:
	  return 1;
	case 0x38:
	  return 1;
	case 0x39:
	  return 1;
	case 0x3A:
	  return 3;
	case 0x3B:
	  return 1;
	case 0x3C:
	  return 1;
	case 0x3D:
	  return 1;
	case 0x3E:
	  return 2;
	case 0x3F:
	  return 1;
	case 0x40:
	  return 1;
	case 0x41:
	  return 1;
	case 0x42:
	  return 1;
	case 0x43:
	  return 1;
	case 0x44:
	  return 1;
	case 0x45:
	  return 1;
	case 0x46:
	  return 1;
	case 0x47:
	  return 1;
	case 0x48:
	  return 1;
	case 0x49:
	  return 1;
	case 0x4A:
	  return 1;
	case 0x4B:
	  return 1;
	case 0x4C:
	  return 1;
	case 0x4D:
	  return 1;
	case 0x4E:
	  return 1;
	case 0x4F:
	  return 1;
	case 0x50:
	  return 1;
	case 0x51:
	  return 1;
	case 0x52:
	  return 1;
	case 0x53:
	  return 1;
	case 0x54:
	  return 1;
	case 0x55:
	  return 1;
	case 0x56:
	  return 1;
	case 0x57:
	  return 1;
	case 0x58:
	  return 1;
	case 0x59:
	  return 1;
	case 0x5A:
	  return 1;
	case 0x5B:
	  return 1;
	case 0x5C:
	  return 1;
	case 0x5D:
	  return 1;
	case 0x5E:
	  return 1;
	case 0x5F:
	  return 1;
	case 0x60:
	  return 1;
	case 0x61:
	  return 1;
	case 0x62:
	  return 1;
	case 0x63:
	  return 1;
	case 0x64:
	  return 1;
	case 0x65:
	  return 1;
	case 0x66:
	  return 1;
	case 0x67:
	  return 1;
	case 0x68:
	  return 1;
	case 0x69:
	  return 1;
	case 0x6A:
	  return 1;
	case 0x6B:
	  return 1;
	case 0x6C:
	  return 1;
	case 0x6D:
	  return 1;
	case 0x6E:
	  return 1;
	case 0x6F:
	  return 1;
	case 0x70:
	  return 1;
	case 0x71:
	  return 1;
	case 0x72:
	  return 1;
	case 0x73:
	  return 1;
	case 0x74:
	  return 1;
	case 0x75:
	  return 1;
	case 0x76:
	  return 1;
	case 0x77:
	  return 1;
	case 0x78:
	  return 1;
	case 0x79:
	  return 1;
	case 0x7A:
	  return 1;
	case 0x7B:
	  return 1;
	case 0x7C:
	  return 1;
	case 0x7D:
	  return 1;
	case 0x7E:
	  return 1;
	case 0x7F:
	  return 1;
	case 0x80:
	  return 1;
	case 0x81:
	  return 1;
	case 0x82:
	  return 1;
	case 0x83:
	  return 1;
	case 0x84:
	  return 1;
	case 0x85:
	  return 1;
	case 0x86:
	  return 1;
	case 0x87:
	  return 1;
	case 0x88:
	  return 1;
	case 0x89:
	  return 1;
	case 0x8A:
	  return 1;
	case 0x8B:
	  return 1;
	case 0x8C:
	  return 1;
	case 0x8D:
	  return 1;
	case 0x8E:
	  return 1;
	case 0x8F:
	  return 1;
	case 0x90:
	  return 1;
	case 0x91:
	  return 1;
	case 0x92:
	  return 1;
	case 0x93:
	  return 1;
	case 0x94:
	  return 1;
	case 0x95:
	  return 1;
	case 0x96:
	  return 1;
	case 0x97:
	  return 1;
	case 0x98:
	  return 1;
	case 0x99:
	  return 1;
	case 0x9A:
	  return 1;
	case 0x9B:
	  return 1;
	case 0x9C:
	  return 1;
	case 0x9D:
	  return 1;
	case 0x9E:
	  return 1;
	case 0x9F:
	  return 1;
	case 0xA0:
	  return 1;
	case 0xA1:
	  return 1;
	case 0xA2:
	  return 1;
	case 0xA3:
	  return 1;
	case 0xA4:
	  return 1;
	case 0xA5:
	  return 1;
	case 0xA6:
	  return 1;
	case 0xA7:
	  return 1;
	case 0xA8:
	  return 1;
	case 0xA9:
	  return 1;
	case 0xAA:
	  return 1;
	case 0xAB:
	  return 1;
	case 0xAC:
	  return 1;
	case 0xAD:
	  return 1;
	case 0xAE:
	  return 1;
	case 0xAF:
	  return 1;
	case 0xB0:
	  return 1;
	case 0xB1:
	  return 1;
	case 0xB2:
	  return 1;
	case 0xB3:
	  return 1;
	case 0xB4:
	  return 1;
	case 0xB5:
	  return 1;
	case 0xB6:
	  return 1;
	case 0xB7:
	  return 1;
	case 0xB8:
	  return 1;
	case 0xB9:
	  return 1;
	case 0xBA:
	  return 1;
	case 0xBB:
	  return 1;
	case 0xBC:
	  return 1;
	case 0xBD:
	  return 1;
	case 0xBE:
	  return 1;
	case 0xBF:
	  return 1;
	case 0xC0:
	  return 1;
	case 0xC1:
	  return 1;
	case 0xC2:
	  return 3;
	case 0xC3:
	  return 3;
	case 0xC4:
	  return 3;
	case 0xC5:
	  return 1;
	case 0xC6:
	  return 2;
	case 0xC7:
	  return 1;
	case 0xC8:
	  return 1;
	case 0xC9:
	  return 1;
	case 0xCA:
	  return 3;
	case 0xCB:
	  return 1;
	case 0xCC:
	  return 3;
	case 0xCD:
	  return 3;
	case 0xCE:
	  return 2;
	case 0xCF:
	  return 1;
	case 0xD0:
	  return 1;
	case 0xD1:
	  return 1;
	case 0xD2:
	  return 3;
	case 0xD3:
	  return 2;
	case 0xD4:
	  return 3;
	case 0xD5:
	  return 1;
	case 0xD6:
	  return 2;
	case 0xD7:
	  return 1;
	case 0xD8:
	  return 1;
	case 0xD9:
	  return 1;
	case 0xDA:
	  return 3;
	case 0xDB:
	  return 2;
	case 0xDC:
	  return 3;
	case 0xDD:
	  return 1;
	case 0xDE:
	  return 2;
	case 0xDF:
	  return 1;
	case 0xE0:
	  return 1;
	case 0xE1:
	  return 1;
	case 0xE2:
	  return 3;
	case 0xE3:
	  return 1;
	case 0xE4:
	  return 3;
	case 0xE5:
	  return 1;
	case 0xE6:
	  return 2;
	case 0xE7:
	  return 1;
	case 0xE8:
	  return 1;
	case 0xE9:
	  return 1;
	case 0xEA:
	  return 3;
	case 0xEB:
	  return 1;
	case 0xEC:
	  return 3;
	case 0xED:
	  return 1;
	case 0xEE:
	  return 2;
	case 0xEF:
	  return 1;
	case 0xF0:
	  return 1;
	case 0xF1:
	  return 1;
	case 0xF2:
	  return 3;
	case 0xF3:
	  return 1;
	case 0xF4:
	  return 3;
	case 0xF5:
	  return 1;
	case 0xF6:
	  return 2;
	case 0xF7:
	  return 1;
	case 0xF8:
	  return 1;
	case 0xF9:
	  return 1;
	case 0xFA:
	  return 3;
	case 0xFB:
	  return 1;
	case 0xFC:
	  return 3;
	case 0xFD:
	  return 1;
	case 0xFE:
	  return 2;
	case 0xFF:
	  return 1;
	}

  g_assert_not_reached ();
  return -1;		/* supress warning */
}
