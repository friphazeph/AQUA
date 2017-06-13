/*
 * tss.h
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


#ifndef TSS_H
	#define TSS_H
	
	#include "../types.h"
	#include "gdt.h"
	
	typedef struct tss_entry {
		uint32 prev_tss;
		uint32 esp0;
		uint32 ss0;
		uint32 esp1;
		uint32 ss1;
		uint32 esp2;
		
		uint32 ss2;
		uint32 cr3;
		
		uint32 eip;
		uint32 eflags;
		
		uint32 eax;
		uint32 ecx;
		uint32 edx;
		uint32 ebx;
		
		uint32 esp;
		uint32 ebp;
		
		uint32 esi;
		uint32 edi;
		
		uint32 es;
		uint32 cs;
		uint32 ss;
		uint32 ds;
		uint32 fs;
		uint32 gs;
		
		uint32 ldt;
		
		uint16 trap;
		uint16 iomap_base;
		
	} tss_entry_t __attribute__((packed));
	
	extern void tss_flush(void);
	extern void tss_install(uint32);
	
	void write_tss(GDT_entry_bits* g);
	void set_kernel_stack(uint32 stack);
	
	void install_tss(GDT_entry* entry);
	void flush_tss(void);
	
#endif
