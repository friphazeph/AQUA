/*
 * idt.h
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


#ifndef IDT_H
	#define IDT_H
	
	#include "../types.h"
	#include "../memory/memory.h"
	
	#define KERNEL_CS 0x08
	
	struct IDT_entry {
		uint16 offset_low;
		uint16 selector;
		
		uint8 zero;
		uint8 attrs;
		
		uint16 offset_high;
		
	} __attribute__((packed));
	
	struct IDT_pointer {
		uint16 _size;
		uint offset;
		
	} __attribute__((packed));
	
	extern void idt_load();
	
	void set_idt_gate(uint8 num, uint32 loc);
	void register_idt(void);
	void set_idt_flags(uint8 _flags);
	
	struct IDT_entry get_entry(int num);
	
#endif
