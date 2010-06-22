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

#include "asm-id.h"
#include "asm-ds-limits.h"

static IdOpcode id_opcode[] = {
  /* 
   * { 0x, "", 0, "X", "X" }, 
   */
  {0xCE, "ACI", 0, "X", "X", 1, "Add immediate to accumulator with carry"}	/* TODO - Add descriptions for all the instructions */
  ,
  {0x8F, "ADC", 1, "A", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x88, "ADC", 1, "B", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x89, "ADC", 1, "C", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x8A, "ADC", 1, "D", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x8B, "ADC", 1, "E", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x8C, "ADC", 1, "H", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x8D, "ADC", 1, "L", "X", 0, "Add register to accumulator with carry"}
  ,
  {0x8E, "ADC", 1, "M", "X", 0, "Add register to accumulator with carry"}
  ,

  {0x87, "ADD", 1, "A", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x80, "ADD", 1, "B", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x81, "ADD", 1, "C", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x82, "ADD", 1, "D", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x83, "ADD", 1, "E", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x84, "ADD", 1, "H", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x85, "ADD", 1, "L", "X", 0, "Add register or memory to accumulator"}
  ,
  {0x86, "ADD", 1, "M", "X", 0, "Add register or memory to accumulator"}
  ,

  {0xC6, "ADI", 0, "X", "X", 1, "Add immediate to accumulator"}
  ,

  {0xA7, "ANA", 1, "A", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA0, "ANA", 1, "B", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA1, "ANA", 1, "C", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA2, "ANA", 1, "D", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA3, "ANA", 1, "E", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA4, "ANA", 1, "H", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA5, "ANA", 1, "L", "X", 0, "Logical AND register or memory with accumulator"}
  ,
  {0xA6, "ANA", 1, "M", "X", 0, "Logical AND register or memory with accumulator"}
  ,

  {0xE6, "ANI", 0, "M", "X", 1, "Logical AND immediate with accumulator"}
  ,

  {0xCD, "CALL", 0, "M", "X", 2, "Subroutine Call"}
  ,
  {0xDC, "CC", 0, "M", "X", 2, "Call on Carry"}
  ,
  {0xFC, "CM", 0, "M", "X", 2, "Call on minus"}
  ,
  {0x2F, "CMA", 0, "M", "X", 0, "Complement accumulator"}
  ,
  {0x3F, "CMC", 0, "M", "X", 0, "Complement carry"}
  ,

  {0xBF, "CMP", 1, "A", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xB8, "CMP", 1, "B", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xB9, "CMP", 1, "C", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xBA, "CMP", 1, "D", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xBB, "CMP", 1, "E", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xBC, "CMP", 1, "H", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xBD, "CMP", 1, "L", "X", 0, "Compare register or memory with accumulator"}
  ,
  {0xBE, "CMP", 1, "M", "X", 0, "Compare register or memory with accumulator"}
  ,

  {0xD4, "CNC", 0, "M", "X", 2, "Call on no Carry"}
  ,
  {0xC4, "CNZ", 0, "M", "X", 2, "Call on no zero"}
  ,
  {0xF4, "CP", 0, "M", "X", 2, "Call on positive"}
  ,
  {0xEC, "CPE", 0, "M", "X", 2, "Call on parity even"}
  ,
  {0xFE, "CPI", 0, "M", "X", 1, "Compare immediate with accumulator"}
  ,
  {0xE4, "CPO", 0, "M", "X", 2, "Call on parity odd"}
  ,
  {0xCC, "CZ", 0, "M", "X", 2, "Call on zero"}
  ,

  {0x27, "DAA", 0, "M", "X", 0, "Decimal adjust accumulator"}
  ,

  {0x09, "DAD", 1, "B", "X", 0, "Add register pair to H and L registers"}
  ,
  {0x19, "DAD", 1, "D", "X", 0, "Add register pair to H and L registers"}
  ,
  {0x29, "DAD", 1, "H", "X", 0, "Add register pair to H and L registers"}
  ,
  {0x39, "DAD", 1, "SP", "X", 0, "Add register pair to H and L registers"}
  ,

  {0x3D, "DCR", 1, "A", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x05, "DCR", 1, "B", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x0D, "DCR", 1, "C", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x15, "DCR", 1, "D", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x1D, "DCR", 1, "E", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x25, "DCR", 1, "H", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x2D, "DCR", 1, "L", "X", 0, "Decrement register or memory by 1"}
  ,
  {0x35, "DCR", 1, "M", "X", 0, "Decrement register or memory by 1"}
  ,

  {0x0B, "DCX", 1, "B", "X", 0, "Decrement register pair by 1"}
  ,
  {0x1B, "DCX", 1, "D", "X", 0, "Decrement register pair by 1"}
  ,
  {0x2B, "DCX", 1, "H", "X", 0, "Decrement register pair by 1"}
  ,
  {0x3B, "DCX", 1, "SP", "X", 0, "Decrement register pair by 1"}
  ,

  {0xF3, "DI", 0, "X", "X", 0, "Disable Interrupts"}
  ,
  {0xFB, "EI", 0, "X", "X", 0, "Enable Interrupts"}
  ,
  {0x76, "HLT", 0, "X", "X", 0, "Halt and enter wait state"}
  ,

  {0xDB, "IN", 0, "X", "X", 1, "Input data to accumulator from a port with 8-bit address"}
  ,

  {0x3C, "INR", 1, "A", "X", 0, "Increment register or memory by 1"}
  ,
  {0x04, "INR", 1, "B", "X", 0, "Increment register or memory by 1"}
  ,
  {0x0C, "INR", 1, "C", "X", 0, "Increment register or memory by 1"}
  ,
  {0x14, "INR", 1, "D", "X", 0, "Increment register or memory by 1"}
  ,
  {0x1C, "INR", 1, "E", "X", 0, "Increment register or memory by 1"}
  ,
  {0x24, "INR", 1, "H", "X", 0, "Increment register or memory by 1"}
  ,
  {0x2C, "INR", 1, "L", "X", 0, "Increment register or memory by 1"}
  ,
  {0x34, "INR", 1, "M", "X", 0, "Increment register or memory by 1"}
  ,

  {0x03, "INX", 1, "B", "X", 0, "Increment register pair by 1"}
  ,
  {0x13, "INX", 1, "D", "X", 0, "Increment register pair by 1"}
  ,
  {0x23, "INX", 1, "H", "X", 0, "Increment register pair by 1"}
  ,
  {0x33, "INX", 1, "SP", "X", 0, "Increment register pair by 1"}
  ,

  {0xDA, "JC", 0, "X", "X", 2, "Jump on Carry"}
  ,
  {0xFA, "JM", 0, "X", "X", 2, "Jump on minus"}
  ,
  {0xC3, "JMP", 0, "X", "X", 2, "Jump"}
  ,
  {0xD2, "JNC", 0, "X", "X", 2, "Jump on no Carry"}
  ,
  {0xC2, "JNZ", 0, "X", "X", 2, "Jump on no zero"}
  ,
  {0xF2, "JP", 0, "X", "X", 2, "Jump on positive"}
  ,
  {0xEA, "JPE", 0, "X", "X", 2, "Jump on parity even"}
  ,
  {0xE2, "JPO", 0, "X", "X", 2, "Jump on parity odd"}
  ,
  {0xCA, "JZ", 0, "X", "X", 2, "Jump on zero"}
  ,

  {0x3A, "LDA", 0, "X", "X", 2, "Load accumulator"}
  ,

  {0x0A, "LDAX", 1, "B", "X", 0, "Load accumulator indirect"}
  ,
  {0x1A, "LDAX", 1, "D", "X", 0, "Load accumulator indirect"}
  ,

  {0x2A, "LHLD", 0, "X", "X", 2, "Load H and L registers direct"}
  ,

  {0x01, "LXI", 1, "B", "X", 2, "Load register pair immediate"}
  ,
  {0x11, "LXI", 1, "D", "X", 2, "Load register pair immediate"}
  ,
  {0x21, "LXI", 1, "H", "X", 2, "Load register pair immediate"}
  ,
  {0x31, "LXI", 1, "SP", "X", 2, "Load register pair immediate"}
  ,

  {0x7F, "MOV", 2, "A", "A", 0, "Copy from source to destination"}
  ,
  {0x78, "MOV", 2, "A", "B", 0, "Copy from source to destination"}
  ,
  {0x79, "MOV", 2, "A", "C", 0, "Copy from source to destination"}
  ,
  {0x7A, "MOV", 2, "A", "D", 0, "Copy from source to destination"}
  ,
  {0x7B, "MOV", 2, "A", "E", 0, "Copy from source to destination"}
  ,
  {0x7C, "MOV", 2, "A", "H", 0, "Copy from source to destination"}
  ,
  {0x7D, "MOV", 2, "A", "L", 0, "Copy from source to destination"}
  ,
  {0x7E, "MOV", 2, "A", "M", 0, "Copy from source to destination"}
  ,

  {0x47, "MOV", 2, "B", "A", 0, "Copy from source to destination"}
  ,
  {0x40, "MOV", 2, "B", "B", 0, "Copy from source to destination"}
  ,
  {0x41, "MOV", 2, "B", "C", 0, "Copy from source to destination"}
  ,
  {0x42, "MOV", 2, "B", "D", 0, "Copy from source to destination"}
  ,
  {0x43, "MOV", 2, "B", "E", 0, "Copy from source to destination"}
  ,
  {0x44, "MOV", 2, "B", "H", 0, "Copy from source to destination"}
  ,
  {0x45, "MOV", 2, "B", "L", 0, "Copy from source to destination"}
  ,
  {0x46, "MOV", 2, "B", "M", 0, "Copy from source to destination"}
  ,

  {0x4F, "MOV", 2, "C", "A", 0, "Copy from source to destination"}
  ,
  {0x48, "MOV", 2, "C", "B", 0, "Copy from source to destination"}
  ,
  {0x49, "MOV", 2, "C", "C", 0, "Copy from source to destination"}
  ,
  {0x4A, "MOV", 2, "C", "D", 0, "Copy from source to destination"}
  ,
  {0x4B, "MOV", 2, "C", "E", 0, "Copy from source to destination"}
  ,
  {0x4C, "MOV", 2, "C", "H", 0, "Copy from source to destination"}
  ,
  {0x4D, "MOV", 2, "C", "L", 0, "Copy from source to destination"}
  ,
  {0x4E, "MOV", 2, "C", "M", 0, "Copy from source to destination"}
  ,

  {0x57, "MOV", 2, "D", "A", 0, "Copy from source to destination"}
  ,
  {0x50, "MOV", 2, "D", "B", 0, "Copy from source to destination"}
  ,
  {0x51, "MOV", 2, "D", "C", 0, "Copy from source to destination"}
  ,
  {0x52, "MOV", 2, "D", "D", 0, "Copy from source to destination"}
  ,
  {0x53, "MOV", 2, "D", "E", 0, "Copy from source to destination"}
  ,
  {0x54, "MOV", 2, "D", "H", 0, "Copy from source to destination"}
  ,
  {0x55, "MOV", 2, "D", "L", 0, "Copy from source to destination"}
  ,
  {0x56, "MOV", 2, "D", "M", 0, "Copy from source to destination"}
  ,

  {0x5F, "MOV", 2, "E", "A", 0, "Copy from source to destination"}
  ,
  {0x58, "MOV", 2, "E", "B", 0, "Copy from source to destination"}
  ,
  {0x59, "MOV", 2, "E", "C", 0, "Copy from source to destination"}
  ,
  {0x5A, "MOV", 2, "E", "D", 0, "Copy from source to destination"}
  ,
  {0x5B, "MOV", 2, "E", "E", 0, "Copy from source to destination"}
  ,
  {0x5C, "MOV", 2, "E", "H", 0, "Copy from source to destination"}
  ,
  {0x5D, "MOV", 2, "E", "L", 0, "Copy from source to destination"}
  ,
  {0x5E, "MOV", 2, "E", "M", 0, "Copy from source to destination"}
  ,

  {0x67, "MOV", 2, "H", "A", 0, "Copy from source to destination"}
  ,
  {0x60, "MOV", 2, "H", "B", 0, "Copy from source to destination"}
  ,
  {0x61, "MOV", 2, "H", "C", 0, "Copy from source to destination"}
  ,
  {0x62, "MOV", 2, "H", "D", 0, "Copy from source to destination"}
  ,
  {0x63, "MOV", 2, "H", "E", 0, "Copy from source to destination"}
  ,
  {0x64, "MOV", 2, "H", "H", 0, "Copy from source to destination"}
  ,
  {0x65, "MOV", 2, "H", "L", 0, "Copy from source to destination"}
  ,
  {0x66, "MOV", 2, "H", "M", 0, "Copy from source to destination"}
  ,

  {0x6F, "MOV", 2, "L", "A", 0, "Copy from source to destination"}
  ,
  {0x68, "MOV", 2, "L", "B", 0, "Copy from source to destination"}
  ,
  {0x69, "MOV", 2, "L", "C", 0, "Copy from source to destination"}
  ,
  {0x6A, "MOV", 2, "L", "D", 0, "Copy from source to destination"}
  ,
  {0x6B, "MOV", 2, "L", "E", 0, "Copy from source to destination"}
  ,
  {0x6C, "MOV", 2, "L", "H", 0, "Copy from source to destination"}
  ,
  {0x6D, "MOV", 2, "L", "L", 0, "Copy from source to destination"}
  ,
  {0x6E, "MOV", 2, "L", "M", 0, "Copy from source to destination"}
  ,

  {0x77, "MOV", 2, "M", "A", 0, "Copy from source to destination"}
  ,
  {0x70, "MOV", 2, "M", "B", 0, "Copy from source to destination"}
  ,
  {0x71, "MOV", 2, "M", "C", 0, "Copy from source to destination"}
  ,
  {0x72, "MOV", 2, "M", "D", 0, "Copy from source to destination"}
  ,
  {0x73, "MOV", 2, "M", "E", 0, "Copy from source to destination"}
  ,
  {0x74, "MOV", 2, "M", "H", 0, "Copy from source to destination"}
  ,
  {0x75, "MOV", 2, "M", "L", 0, "Copy from source to destination"}
  ,

  {0x3E, "MVI", 1, "A", "X", 1, "Move immediate 8-bit"}
  ,
  {0x06, "MVI", 1, "B", "X", 1, "Move immediate 8-bit"}
  ,
  {0x0E, "MVI", 1, "C", "X", 1, "Move immediate 8-bit"}
  ,
  {0x16, "MVI", 1, "D", "X", 1, "Move immediate 8-bit"}
  ,
  {0x1E, "MVI", 1, "E", "X", 1, "Move immediate 8-bit"}
  ,
  {0x26, "MVI", 1, "H", "X", 1, "Move immediate 8-bit"}
  ,
  {0x2E, "MVI", 1, "L", "X", 1, "Move immediate 8-bit"}
  ,
  {0x36, "MVI", 1, "M", "X", 1, "Move immediate 8-bit"}
  ,

  {0x00, "NOP", 0, "M", "X", 0, "No operation"}
  ,

  {0xB7, "ORA", 1, "A", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB0, "ORA", 1, "B", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB1, "ORA", 1, "C", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB2, "ORA", 1, "D", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB3, "ORA", 1, "E", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB4, "ORA", 1, "H", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB5, "ORA", 1, "L", "X", 0, "Logical OR register or memory with accumulator"}
  ,
  {0xB6, "ORA", 1, "M", "X", 0, "Logical OR register or memory with accumulator"}
  ,

  {0xF6, "ORI", 0, "M", "X", 1, "Logical OR immediate with accumulator"}
  ,
  {0xD3, "OUT", 0, "M", "X", 1, "Output data from accumulator to a port with 8-bit address"}
  ,

  {0xE9, "PCHL", 0, "M", "X", 0, "Load program counter with HL contents"}
  ,

  {0xC0, "RNZ", 0, "B", "X", 0, "Return on no zero"}
  ,
  {0xC1, "POP", 1, "B", "X", 0, "Pop off stack to register pair"}
  ,
  {0xD1, "POP", 1, "D", "X", 0, "Pop off stack to register pair"}
  ,
  {0xE1, "POP", 1, "H", "X", 0, "Pop off stack to register pair"}
  ,
  {0xF1, "POP", 1, "PSW", "X", 0, "Pop off stack to register pair"}
  ,

  {0xC5, "PUSH", 1, "B", "X", 0, "Push register pair onto stack"}
  ,
  {0xD5, "PUSH", 1, "D", "X", 0, "Push register pair onto stack"}
  ,
  {0xE5, "PUSH", 1, "H", "X", 0, "Push register pair onto stack"}
  ,
  {0xF5, "PUSH", 1, "PSW", "X", 0, "Push register pair onto stack"}
  ,

  {0x17, "RAL", 0, "M", "X", 0, "Rotate accumulator left through carry"}
  ,
  {0x1F, "RAR", 0, "M", "X", 0, "Rotate accumulator right through carry"}
  ,
  {0xD8, "RC", 0, "M", "X", 0, "Return on Carry"}
  ,
  {0xC9, "RET", 0, "M", "X", 0, "Return from subroutine"}
  ,
  {0x20, "RIM", 0, "M", "X", 0, "Read interrupt mask"}
  ,
  {0x07, "RLC", 0, "M", "X", 0, "Rotate accumulator left"}
  ,
  {0xF8, "RM", 0, "M", "X", 0, "Return on minus"}
  ,
  {0xD0, "RNC", 0, "M", "X", 0, "Return on no Carry"}
  ,
  {0xF0, "RP", 0, "M", "X", 0, "Return on positive"}
  ,
  {0xE8, "RPE", 0, "M", "X", 0, "Return on parity even"}
  ,
  {0xE0, "RPO", 0, "M", "X", 0, "Return on parity odd"}
  ,
  {0x0F, "RRC", 0, "M", "X", 0, "Rotate accumulator right"}
  ,

  {0xC7, "RST", 1, "0", "X", 0, "Restart"}
  ,
  {0xCF, "RST", 1, "1", "X", 0, "Restart"}
  ,
  {0xD7, "RST", 1, "2", "X", 0, "Restart"}
  ,
  {0xDF, "RST", 1, "3", "X", 0, "Restart"}
  ,
  {0xE7, "RST", 1, "4", "X", 0, "Restart"}
  ,
  {0xEF, "RST", 1, "5", "X", 0, "Restart"}
  ,
  {0xF7, "RST", 1, "6", "X", 0, "Restart"}
  ,
  {0xFF, "RST", 1, "7", "X", 0, "Restart"}
  ,

  {0xC8, "RZ", 0, "X", "X", 0, "Return on zero"}
  ,

  {0x9F, "SBB", 1, "A", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x98, "SBB", 1, "B", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x99, "SBB", 1, "C", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x9A, "SBB", 1, "D", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x9B, "SBB", 1, "E", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x9C, "SBB", 1, "H", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x9D, "SBB", 1, "L", "X", 0, "Subtract source and borrow from accumulator"}
  ,
  {0x9E, "SBB", 1, "M", "X", 0, "Subtract source and borrow from accumulator"}
  ,

  {0xDE, "SBI", 0, "M", "X", 1, "Subtract immediate from accumulator with borrow"}
  ,
  {0x22, "SHLD", 0, "M", "X", 2, "Store H and L registers direct"}
  ,

  {0x30, "SIM", 0, "M", "X", 0, "Set interrupt mask"}
  ,
  {0xF9, "SPHL", 0, "M", "X", 0, "Copy H and L registers to the stack pointer"}
  ,

  {0x32, "STA", 0, "M", "X", 2, "Store accumulator direct"}
  ,
  {0x02, "STAX", 1, "B", "X", 0, "Store accumulator indirect (RegPair)"}
  ,
  {0x12, "STAX", 1, "D", "X", 0, "Store accumulator indirect (RegPair)"}
  ,

  {0x37, "STC", 0, "M", "X", 0, "Set Carry"}
  ,			//modified 

  {0x97, "SUB", 1, "A", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x90, "SUB", 1, "B", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x91, "SUB", 1, "C", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x92, "SUB", 1, "D", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x93, "SUB", 1, "E", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x94, "SUB", 1, "H", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x95, "SUB", 1, "L", "X", 0, "Subtract register or memory from accumulator"}
  ,
  {0x96, "SUB", 1, "M", "X", 0, "Subtract register or memory from accumulator"}
  ,

  {0xD6, "SUI", 0, "M", "X", 1, "Subtract immediate from accumulator"}
  ,

  {0xEB, "XCHG", 0, "M", "X", 0, "Exchange H and L with D and E"}
  ,

  {0xAF, "XRA", 1, "A", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xA8, "XRA", 1, "B", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xA9, "XRA", 1, "C", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xAA, "XRA", 1, "D", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xAB, "XRA", 1, "E", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xAC, "XRA", 1, "H", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xAD, "XRA", 1, "L", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,
  {0xAE, "XRA", 1, "M", "X", 0, "Exclusive OR register or memory with accumulator"}
  ,

  {0xEE, "XRI", 0, "M", "X", 1, "Exclusive OR immediate with accumulator"}
  ,
  {0xE3, "XTHL", 0, "M", "X", 0, "Exchange H and L with top of stack"}
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
