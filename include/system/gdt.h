/*
 * gdt.h
 * 
 * Copyright 2017 Obiwac
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef GDT_H
	#define GDT_H
	
	#include "../types.h"
	//#include "tss.h"
	
	#define GDT_ENTRIES 5
	
	typedef struct GDT_entry {
		uint16 limit;
		uint16 base_low;
		
		uint8 base_middle;
		uint8 access;
		uint8 granularity;
		uint8 base_high;
		
	} __attribute__((packed)) GDT_entry;
	
	typedef struct GDT_pointer {
		uint16 limit;
		uint base;
		
	} __attribute__((packed)) GDT_pointer;
	
	typedef struct GDT_entry_bits {
		uint32 limit_low : 16;
		uint32 base_low : 24;
		
		uint32 accessed : 1;
		uint32 read_write : 1;
		uint32 conforming_expand_down : 1;
		uint32 code : 1;
		uint32 always_1 : 1;
		uint32 DPL : 2;
		
		uint32 limit_high : 4;
		uint32 available : 1;
		uint32 always_0 : 1;
		uint32 big : 1;
		uint32 gran : 1;
		uint32 base_high : 8;
		
	} GDT_entry_bits __attribute__((packed));
	
	void gdt_set_gate(uint32 num, uint16 limit, uint32 base, uint8 access, uint8 granularity);
	void load_gdt(void);
	void enter_user_space(void);
	
	void user_function(void);
	
#endif
