/*
 * idt.c
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



#include "idt.h"

static uint8 flags = 0x8E;
static struct IDT_entry idt[256];

struct IDT_pointer idtp;

void set_idt_gate(uint8 num, uint32 loc) {
	idt[num].offset_low = low_16(loc);
	idt[num].offset_high = high_16(loc);
	idt[num].selector = KERNEL_CS;
	
	idt[num].zero = 0;
	idt[num].attrs = flags;
	
}

void register_idt(void) {
	idtp._size = (sizeof(struct IDT_entry) * 256) - 1;
	idtp.offset = (int) &idt;
	
	memset(&idt, 0, sizeof(struct IDT_entry) * 256);
	idt_load();
	
}

void set_idt_flags(uint8 _flags) {
	flags = _flags;
	
}

struct IDT_entry get_entry(int num) {
	return idt[num];
	
}
