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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "asm-genobj.h"
#include "asm-token.h"
#include "asm-err-comm.h"
#include "8085.h"
#include "8085-link.h"

#define ASM_GENOBJ_INIT_BYTES 500
#define ASM_GENOBJ_DS_MAX 1024

static gboolean
str_equal (const gchar * a, const gchar * b)
{
  return !g_ascii_strcasecmp (a, b);
}

static gboolean
add_numbers_to_block (AsmSourceEntry *entry, const gchar * str, EefMemBlock * mblock, gint * total)
{
  gchar **slist, **p;
  gint val;
  g_assert (str);
  g_assert (total);

  /* split */
  slist = g_strsplit (str, ",", -1);

  /* scan */
  p = slist;
  *total = 0;
  while (*p)
	{
	  //if (!asm_util_parse_number (*p, &val))
	  if (!asm_source_entry_parse_not_operand_but_this (entry, &val, *p))
		{
		  return FALSE;
		}
	  (*total)++;
	  /* add to mem block */
	  eef_mem_block_grow (mblock, val);

	  p++;
	}

  g_strfreev (slist);
  return TRUE;
}

EefMemBlock *
asm_genobj_generate (AsmSource * src, gint sa)
{
  gint i;
  gint address = sa;
  EefMemBlock *mblock;

  g_assert (src);

#define SEND_ERR(ln, str)						\
  asm_err_comm_send (ln, str, ASM_ERR_ERROR);	\
  return NULL;

  /* create mem block */
  mblock = eef_mem_block_new (ASM_GENOBJ_INIT_BYTES);
  g_assert (mblock);
  eef_mem_block_set_start_addr (mblock, sa);

  /* For each source line */
  for (i = 0; i < src->entries_total; i++)
	{
	  AsmSourceEntry *entry = src->entries[i];
	  gint line_no = entry->listing_buffer_line_no;

	  /* store address info */
	  entry->address = address;

	  /* If pseudo op */
	  if (entry->s_op >= 256)
		{
		  /* If DS */
		  if (str_equal ("DS", entry->s_op_id_pseudo->op_str))
			{
			  gint val;
			  if (!asm_source_entry_parse_operand
				  (entry, &val))
				{
				  SEND_ERR (line_no,
							_("Invalid data size"));
				}
			  if (val < 0 || val > ASM_GENOBJ_INIT_BYTES)
				{
				  SEND_ERR (line_no,
							_("DS: Exceeded limit"));
				}
			  address += val;
			  entry->b_opnd1 = val;
			  eef_mem_block_grow_n (mblock, 0, val);
			}
		  /* If DB */
		  else if (str_equal
				   ("DB", entry->s_op_id_pseudo->op_str))
			{
			  gint val;
			  if (!add_numbers_to_block
				  (entry, entry->s_opnd, mblock, &val))
				{
				  SEND_ERR (line_no,
							_("DB: Operand error"));
				}
			  address += val;
			}
		}
	  /* If opcode */
	  else
		{
		  IdOpcode *id_op = entry->s_op_id_opcode;

		  g_assert (id_op);

		  /* link */
		  eef_link_add (address, entry->listing_buffer_line_no);

		  /* add opcode */
		  address += 1;
		  eef_mem_block_grow (mblock, id_op->op_num);
		  entry->b_op = id_op->op_num;

		  /* add operands if any */
		  if (id_op->user_args)
			{
			  gint val;
			  guint8 val_h, val_l;

			  if (!asm_source_entry_parse_operand
				  (entry, &val))
				{
				  SEND_ERR (line_no,
							_("Invalid operand or symbol"));
				}

			  eef_split16 (val, &val_h, &val_l);
			  entry->b_opnd1 = val_l;
			  entry->b_opnd2 = val_h;
			  /* should we add 1 byte or 2 bytes */
			  if (id_op->user_args == 1)
				{
				  address++;
				  eef_mem_block_grow (mblock, val);
				  entry->b_opnd1 = val;
				}
			  else
				{
				  address += 2;
				  eef_mem_block_grow (mblock, val_l);
				  entry->b_opnd1 = val_l;
				  eef_mem_block_grow (mblock, val_h);
				  entry->b_opnd2 = val_h;
					
				}
			  g_assert (id_op->user_args < 3);
			}	/* if operands */
		}		/* If opcode */
	}			/* for each source line */

  return mblock;
}
