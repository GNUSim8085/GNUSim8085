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

#include "asm-id.h"
#include "asm-ds-limits.h"

static IdOpcode id_opcode[] = {
  /* 
   * { 0x, "", 0, "X", "X" }, 
   */
  {0xCE, "ACI", 0, "X", "X", 1}
  ,
  {0x8F, "ADC", 1, "A", "X", 0}
  ,
  {0x88, "ADC", 1, "B", "X", 0}
  ,
  {0x89, "ADC", 1, "C", "X", 0}
  ,
  {0x8A, "ADC", 1, "D", "X", 0}
  ,
  {0x8B, "ADC", 1, "E", "X", 0}
  ,
  {0x8C, "ADC", 1, "H", "X", 0}
  ,
  {0x8D, "ADC", 1, "L", "X", 0}
  ,
  {0x8E, "ADC", 1, "M", "X", 0}
  ,

  {0x87, "ADD", 1, "A", "X", 0}
  ,
  {0x80, "ADD", 1, "B", "X", 0}
  ,
  {0x81, "ADD", 1, "C", "X", 0}
  ,
  {0x82, "ADD", 1, "D", "X", 0}
  ,
  {0x83, "ADD", 1, "E", "X", 0}
  ,
  {0x84, "ADD", 1, "H", "X", 0}
  ,
  {0x85, "ADD", 1, "L", "X", 0}
  ,
  {0x86, "ADD", 1, "M", "X", 0}
  ,

  {0xC6, "ADI", 0, "X", "X", 1}
  ,

  {0xA7, "ANA", 1, "A", "X", 0}
  ,
  {0xA0, "ANA", 1, "B", "X", 0}
  ,
  {0xA1, "ANA", 1, "C", "X", 0}
  ,
  {0xA2, "ANA", 1, "D", "X", 0}
  ,
  {0xA3, "ANA", 1, "E", "X", 0}
  ,
  {0xA4, "ANA", 1, "H", "X", 0}
  ,
  {0xA5, "ANA", 1, "L", "X", 0}
  ,
  {0xA6, "ANA", 1, "M", "X", 0}
  ,

  {0xE6, "ANI", 0, "M", "X", 1}
  ,

  {0xCD, "CALL", 0, "M", "X", 2}
  ,
  {0xDC, "CC", 0, "M", "X", 2}
  ,
  {0xFC, "CM", 0, "M", "X", 2}
  ,
  {0x2F, "CMA", 0, "M", "X", 0}
  ,
  {0x3F, "CMC", 0, "M", "X", 0}
  ,

  {0xBF, "CMP", 1, "A", "X", 0}
  ,
  {0xB8, "CMP", 1, "B", "X", 0}
  ,
  {0xB9, "CMP", 1, "C", "X", 0}
  ,
  {0xBA, "CMP", 1, "D", "X", 0}
  ,
  {0xBB, "CMP", 1, "E", "X", 0}
  ,
  {0xBC, "CMP", 1, "H", "X", 0}
  ,
  {0xBD, "CMP", 1, "L", "X", 0}
  ,
  {0xBE, "CMP", 1, "M", "X", 0}
  ,

  {0xD4, "CNC", 0, "M", "X", 2}
  ,
  {0xC4, "CNZ", 0, "M", "X", 2}
  ,
  {0xF4, "CP", 0, "M", "X", 2}
  ,
  {0xEC, "CPE", 0, "M", "X", 2}
  ,
  {0xFE, "CPI", 0, "M", "X", 1}
  ,
  {0xE4, "CPO", 0, "M", "X", 2}
  ,
  {0xCC, "CZ", 0, "M", "X", 2}
  ,

  {0x27, "DAA", 0, "M", "X", 0}
  ,

  {0x09, "DAD", 1, "B", "X", 0}
  ,
  {0x19, "DAD", 1, "D", "X", 0}
  ,
  {0x29, "DAD", 1, "H", "X", 0}
  ,
  {0x39, "DAD", 1, "SP", "X", 0}
  ,

  {0x3D, "DCR", 1, "A", "X", 0}
  ,
  {0x05, "DCR", 1, "B", "X", 0}
  ,
  {0x0D, "DCR", 1, "C", "X", 0}
  ,
  {0x15, "DCR", 1, "D", "X", 0}
  ,
  {0x1D, "DCR", 1, "E", "X", 0}
  ,
  {0x25, "DCR", 1, "H", "X", 0}
  ,
  {0x2D, "DCR", 1, "L", "X", 0}
  ,
  {0x35, "DCR", 1, "M", "X", 0}
  ,

  {0x0B, "DCX", 1, "B", "X", 0}
  ,
  {0x1B, "DCX", 1, "D", "X", 0}
  ,
  {0x2B, "DCX", 1, "H", "X", 0}
  ,
  {0x3B, "DCX", 1, "SP", "X", 0}
  ,

  {0xF3, "DI", 0, "X", "X", 0}
  ,
  {0xFB, "EI", 0, "X", "X", 0}
  ,
  {0x76, "HLT", 0, "X", "X", 0}
  ,

  {0xDB, "IN", 0, "X", "X", 1}
  ,

  {0x3C, "INR", 1, "A", "X", 0}
  ,
  {0x04, "INR", 1, "B", "X", 0}
  ,
  {0x0C, "INR", 1, "C", "X", 0}
  ,
  {0x14, "INR", 1, "D", "X", 0}
  ,
  {0x1C, "INR", 1, "E", "X", 0}
  ,
  {0x24, "INR", 1, "H", "X", 0}
  ,
  {0x2C, "INR", 1, "L", "X", 0}
  ,
  {0x34, "INR", 1, "M", "X", 0}
  ,

  {0x03, "INX", 1, "B", "X", 0}
  ,
  {0x13, "INX", 1, "D", "X", 0}
  ,
  {0x23, "INX", 1, "H", "X", 0}
  ,
  {0x33, "INX", 1, "SP", "X", 0}
  ,

  {0xDA, "JC", 0, "X", "X", 2}
  ,
  {0xFA, "JM", 0, "X", "X", 2}
  ,
  {0xC3, "JMP", 0, "X", "X", 2}
  ,
  {0xD2, "JNC", 0, "X", "X", 2}
  ,
  {0xC2, "JNZ", 0, "X", "X", 2}
  ,
  {0xF2, "JP", 0, "X", "X", 2}
  ,
  {0xEA, "JPE", 0, "X", "X", 2}
  ,
  {0xE2, "JPO", 0, "X", "X", 2}
  ,
  {0xCA, "JZ", 0, "X", "X", 2}
  ,

  {0x3A, "LDA", 0, "X", "X", 2}
  ,

  {0x0A, "LDAX", 1, "B", "X", 0}
  ,
  {0x1A, "LDAX", 1, "D", "X", 0}
  ,

  {0x2A, "LHLD", 0, "X", "X", 2}
  ,

  {0x01, "LXI", 1, "B", "X", 2}
  ,
  {0x11, "LXI", 1, "D", "X", 2}
  ,
  {0x21, "LXI", 1, "H", "X", 2}
  ,
  {0x31, "LXI", 1, "SP", "X", 2}
  ,

  {0x7F, "MOV", 2, "A", "A", 0}
  ,
  {0x78, "MOV", 2, "A", "B", 0}
  ,
  {0x79, "MOV", 2, "A", "C", 0}
  ,
  {0x7A, "MOV", 2, "A", "D", 0}
  ,
  {0x7B, "MOV", 2, "A", "E", 0}
  ,
  {0x7C, "MOV", 2, "A", "H", 0}
  ,
  {0x7D, "MOV", 2, "A", "L", 0}
  ,
  {0x7E, "MOV", 2, "A", "M", 0}
  ,

  {0x47, "MOV", 2, "B", "A", 0}
  ,
  {0x40, "MOV", 2, "B", "B", 0}
  ,
  {0x41, "MOV", 2, "B", "C", 0}
  ,
  {0x42, "MOV", 2, "B", "D", 0}
  ,
  {0x43, "MOV", 2, "B", "E", 0}
  ,
  {0x44, "MOV", 2, "B", "H", 0}
  ,
  {0x45, "MOV", 2, "B", "L", 0}
  ,
  {0x46, "MOV", 2, "B", "M", 0}
  ,

  {0x4F, "MOV", 2, "C", "A", 0}
  ,
  {0x48, "MOV", 2, "C", "B", 0}
  ,
  {0x49, "MOV", 2, "C", "C", 0}
  ,
  {0x4A, "MOV", 2, "C", "D", 0}
  ,
  {0x4B, "MOV", 2, "C", "E", 0}
  ,
  {0x4C, "MOV", 2, "C", "H", 0}
  ,
  {0x4D, "MOV", 2, "C", "L", 0}
  ,
  {0x4E, "MOV", 2, "C", "M", 0}
  ,

  {0x57, "MOV", 2, "D", "A", 0}
  ,
  {0x50, "MOV", 2, "D", "B", 0}
  ,
  {0x51, "MOV", 2, "D", "C", 0}
  ,
  {0x52, "MOV", 2, "D", "D", 0}
  ,
  {0x53, "MOV", 2, "D", "E", 0}
  ,
  {0x54, "MOV", 2, "D", "H", 0}
  ,
  {0x55, "MOV", 2, "D", "L", 0}
  ,
  {0x56, "MOV", 2, "D", "M", 0}
  ,

  {0x5F, "MOV", 2, "E", "A", 0}
  ,
  {0x58, "MOV", 2, "E", "B", 0}
  ,
  {0x59, "MOV", 2, "E", "C", 0}
  ,
  {0x5A, "MOV", 2, "E", "D", 0}
  ,
  {0x5B, "MOV", 2, "E", "E", 0}
  ,
  {0x5C, "MOV", 2, "E", "H", 0}
  ,
  {0x5D, "MOV", 2, "E", "L", 0}
  ,
  {0x5E, "MOV", 2, "E", "M", 0}
  ,

  {0x67, "MOV", 2, "H", "A", 0}
  ,
  {0x60, "MOV", 2, "H", "B", 0}
  ,
  {0x61, "MOV", 2, "H", "C", 0}
  ,
  {0x62, "MOV", 2, "H", "D", 0}
  ,
  {0x63, "MOV", 2, "H", "E", 0}
  ,
  {0x64, "MOV", 2, "H", "H", 0}
  ,
  {0x65, "MOV", 2, "H", "L", 0}
  ,
  {0x66, "MOV", 2, "H", "M", 0}
  ,

  {0x6F, "MOV", 2, "L", "A", 0}
  ,
  {0x68, "MOV", 2, "L", "B", 0}
  ,
  {0x69, "MOV", 2, "L", "C", 0}
  ,
  {0x6A, "MOV", 2, "L", "D", 0}
  ,
  {0x6B, "MOV", 2, "L", "E", 0}
  ,
  {0x6C, "MOV", 2, "L", "H", 0}
  ,
  {0x6D, "MOV", 2, "L", "L", 0}
  ,
  {0x6E, "MOV", 2, "L", "M", 0}
  ,

  {0x77, "MOV", 2, "M", "A", 0}
  ,
  {0x70, "MOV", 2, "M", "B", 0}
  ,
  {0x71, "MOV", 2, "M", "C", 0}
  ,
  {0x72, "MOV", 2, "M", "D", 0}
  ,
  {0x73, "MOV", 2, "M", "E", 0}
  ,
  {0x74, "MOV", 2, "M", "H", 0}
  ,
  {0x75, "MOV", 2, "M", "L", 0}
  ,

  {0x3E, "MVI", 1, "A", "X", 1}
  ,
  {0x06, "MVI", 1, "B", "X", 1}
  ,
  {0x0E, "MVI", 1, "C", "X", 1}
  ,
  {0x16, "MVI", 1, "D", "X", 1}
  ,
  {0x1E, "MVI", 1, "E", "X", 1}
  ,
  {0x26, "MVI", 1, "H", "X", 1}
  ,
  {0x2E, "MVI", 1, "L", "X", 1}
  ,
  {0x36, "MVI", 1, "M", "X", 1}
  ,

  {0x00, "NOP", 0, "M", "X", 0}
  ,

  {0xB7, "ORA", 1, "A", "X", 0}
  ,
  {0xB0, "ORA", 1, "B", "X", 0}
  ,
  {0xB1, "ORA", 1, "C", "X", 0}
  ,
  {0xB2, "ORA", 1, "D", "X", 0}
  ,
  {0xB3, "ORA", 1, "E", "X", 0}
  ,
  {0xB4, "ORA", 1, "H", "X", 0}
  ,
  {0xB5, "ORA", 1, "L", "X", 0}
  ,
  {0xB6, "ORA", 1, "M", "X", 0}
  ,

  {0xF6, "ORI", 0, "M", "X", 1}
  ,
  {0xD3, "OUT", 0, "M", "X", 1}
  ,

  {0xE9, "PCHL", 0, "M", "X", 0}
  ,

  {0xC0, "RNZ", 0, "B", "X", 0}
  ,
  {0xC1, "POP", 1, "B", "X", 0}
  ,
  {0xD1, "POP", 1, "D", "X", 0}
  ,
  {0xE1, "POP", 1, "H", "X", 0}
  ,
  {0xF1, "POP", 1, "PSW", "X", 0}
  ,

  {0xC5, "PUSH", 1, "B", "X", 0}
  ,
  {0xD5, "PUSH", 1, "D", "X", 0}
  ,
  {0xE5, "PUSH", 1, "H", "X", 0}
  ,
  {0xF5, "PUSH", 1, "PSW", "X", 0}
  ,

  {0x17, "RAL", 0, "M", "X", 0}
  ,
  {0x1F, "RAR", 0, "M", "X", 0}
  ,
  {0xD8, "RC", 0, "M", "X", 0}
  ,
  {0xC9, "RET", 0, "M", "X", 0}
  ,
  {0x20, "RIM", 0, "M", "X", 0}
  ,
  {0x07, "RLC", 0, "M", "X", 0}
  ,
  {0xF8, "RM", 0, "M", "X", 0}
  ,
  {0xD0, "RNC", 0, "M", "X", 0}
  ,
  {0xF0, "RP", 0, "M", "X", 0}
  ,
  {0xE8, "RPE", 0, "M", "X", 0}
  ,
  {0xE0, "RPO", 0, "M", "X", 0}
  ,
  {0x0F, "RRC", 0, "M", "X", 0}
  ,

  {0xC7, "RST", 1, "0", "X", 0}
  ,
  {0xCF, "RST", 1, "1", "X", 0}
  ,
  {0xD7, "RST", 1, "2", "X", 0}
  ,
  {0xDF, "RST", 1, "3", "X", 0}
  ,
  {0xE7, "RST", 1, "4", "X", 0}
  ,
  {0xEF, "RST", 1, "5", "X", 0}
  ,
  {0xF7, "RST", 1, "6", "X", 0}
  ,
  {0xFF, "RST", 1, "7", "X", 0}
  ,

  {0xC8, "RZ", 0, "X", "X", 0}
  ,

  {0x9F, "SBB", 1, "A", "X", 0}
  ,
  {0x98, "SBB", 1, "B", "X", 0}
  ,
  {0x99, "SBB", 1, "C", "X", 0}
  ,
  {0x9A, "SBB", 1, "D", "X", 0}
  ,
  {0x9B, "SBB", 1, "E", "X", 0}
  ,
  {0x9C, "SBB", 1, "H", "X", 0}
  ,
  {0x9D, "SBB", 1, "L", "X", 0}
  ,
  {0x9E, "SBB", 1, "M", "X", 0}
  ,

  {0xDE, "SBI", 0, "M", "X", 1}
  ,
  {0x22, "SHLD", 0, "M", "X", 2}
  ,

  {0x30, "SIM", 0, "M", "X", 0}
  ,
  {0xF9, "SPHL", 0, "M", "X", 0}
  ,

  {0x32, "STA", 0, "M", "X", 2}
  ,
  {0x02, "STAX", 1, "B", "X", 0}
  ,
  {0x12, "STAX", 1, "D", "X", 0}
  ,

  {0x37, "STC", 0, "M", "X", 0}
  ,			//modified 

  {0x97, "SUB", 1, "A", "X", 0}
  ,
  {0x90, "SUB", 1, "B", "X", 0}
  ,
  {0x91, "SUB", 1, "C", "X", 0}
  ,
  {0x92, "SUB", 1, "D", "X", 0}
  ,
  {0x93, "SUB", 1, "E", "X", 0}
  ,
  {0x94, "SUB", 1, "H", "X", 0}
  ,
  {0x95, "SUB", 1, "L", "X", 0}
  ,
  {0x96, "SUB", 1, "M", "X", 0}
  ,

  {0xD6, "SUI", 0, "M", "X", 1}
  ,

  {0xEB, "XCHG", 0, "M", "X", 0}
  ,

  {0xAF, "XRA", 1, "A", "X", 0}
  ,
  {0xA8, "XRA", 1, "B", "X", 0}
  ,
  {0xA9, "XRA", 1, "C", "X", 0}
  ,
  {0xAA, "XRA", 1, "D", "X", 0}
  ,
  {0xAB, "XRA", 1, "E", "X", 0}
  ,
  {0xAC, "XRA", 1, "H", "X", 0}
  ,
  {0xAD, "XRA", 1, "L", "X", 0}
  ,
  {0xAE, "XRA", 1, "M", "X", 0}
  ,

  {0xEE, "XRI", 0, "M", "X", 1}
  ,
  {0xE3, "XTHL", 0, "M", "X", 0}
};

IdOpcode *
asm_id_get_opcode (void)
{
  return id_opcode;
}

IdOpcode *
asm_id_opcode_lookup (gchar * op_name, gchar * arg1, gchar * arg2)
{
  int i;

  g_assert (op_name);

  /* linear search! Not faster. but assembling will take no time */
  for (i = 0; i < sizeof (id_opcode) / sizeof (id_opcode[0]); i++)
	{
	  if (g_ascii_strcasecmp (op_name, id_opcode[i].op_str))
		continue;

	  if (arg1 && g_ascii_strcasecmp (arg1, id_opcode[i].arg1))
		continue;

	  if (arg2 && g_ascii_strcasecmp (arg2, id_opcode[i].arg2))
		continue;

	  return &id_opcode[i];
	}

  return NULL;
}

static IdPseudo id_pseudo[] = {
  {ID_PSEUDO_EQU, "EQU", 1},

  {ID_PSEUDO_DB, "DB", 1},
  {ID_PSEUDO_DS, "DS", 1}

};

IdPseudo *
asm_id_get_pseudo (void)
{
  return id_pseudo;
}

IdPseudo *
asm_id_pseudo_lookup (gchar * op_name)
{
  int i;

  g_assert (op_name);

  for (i = 0; i < sizeof (id_pseudo) / sizeof (id_pseudo[0]); i++)
	{
	  if (g_ascii_strcasecmp (op_name, id_pseudo[i].op_str))
		continue;

	  return &id_pseudo[i];
	}

  return NULL;
}

gchar *
asm_id_return_all_opcode_names (void)
{
  gchar *all, *next;
  gint i, tot;

  all = g_malloc (ASM_DS_MAX_OPCODE_LENGTH * 0xFF);
  next = all;

  tot = sizeof (id_opcode) / sizeof (id_opcode[0]);
  g_assert (tot == 246);

  /* for each opcode */
  for (i = 0; i < tot; i++)
	{
	  next = g_stpcpy (next, id_opcode[i].op_str);
	  next = g_stpcpy (next, " ");
	}

  next = g_ascii_strdown (all, -1);
  g_free (all);

  return next;
}

gint asm_id_total_opcodes (void)
{
  return sizeof(id_opcode)/sizeof(id_opcode[0]);
}
gint asm_id_total_pseudos (void)
{
  return sizeof(id_pseudo)/sizeof(id_pseudo[0]);
}
