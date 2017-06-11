/*
 * gdt.c
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

#include "gdt.h"

static GDT_entry gdt[GDT_ENTRIES];
GDT_pointer gdt_ptr;

extern void gdt_flush(void);

void gdt_set_gate(uint32 num, uint16 limit, uint32 base, uint8 access, uint8 granularity) {
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	
	gdt[num].limit = limit;
	gdt[num].granularity = granularity;
	gdt[num].access = access;
	
}

void load_gdt(void) {
	gdt_ptr.limit = (sizeof(GDT_entry) * GDT_ENTRIES) - 1;
	gdt_ptr.base = (uint32) &gdt;
	
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0xFFFF, 0, 0b10011010, 0b11001111);
	gdt_set_gate(2, 0xFFFF, 0, 0b10010010, 0b11001111);
	gdt_set_gate(3, 0xFFFF, 0, 0b11111010, 0b11001111);
	gdt_set_gate(4, 0xFFFF, 0, 0b11110010, 0b11001111);
	
	gdt_flush();
	
}
