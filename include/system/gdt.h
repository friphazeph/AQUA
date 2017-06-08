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
	
	struct GDT_entry_bits {
		uint32 limit_low : 16;
		uint32 base_low : 24;
		uint32 accessed : 1;
		uint32 
		
	};
	
	void gdt_set_gate(uint32 num, uint16 limit, uint32 base, uint8 access, uint8 granularity);
	void load_gdt(void);
	
#endif
