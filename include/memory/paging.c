/*
 * paging.c
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


#include "paging.h"

uint32 page_directory[1024] __attribute__((aligned(4096)));
uint32 exec_page_table[1024] __attribute__((aligned(4096)));

#define kernel_start_phys = &kernel_start - HIGHER_HALF
#define kernel_end_phys = &kernel_end - HIGHER_HALF

extern uint32 boot_page_directory;

void setup_paging(void) {
	page_directory[0] = 0x83;
	
	uint32 i;
	for (i = 1; i < (HIGHER_HALF >> 22); i++) {
		page_directory[i] = 0x0;
		
	}
	
	page_directory[i] = 0x83;
	i++;
	
	for (i = i; i < 1024; i++) {
		page_directory[i] = 0;
		
	}
	
	uint32* d = (uint32*) 0x1000;
	load_page_dir((uint32*) ((uint32) &page_directory[0] - HIGHER_HALF));
	
}

void exec(uint8* program) {
	exec_page_table[0] = ((uint32) &program[0] - HIGHER_HALF) | 0x3;
	page_directory[0] = ((uint32) &exec_page_table[0] - HIGHER_HALF) | 0x3;
	
	load_page_dir((uint32*) ((uint32) &page_directory[0] - HIGHER_HALF));
	((void(*)())0)();
	
	exec_page_table[0] = 0x83;
	load_page_dir((uint32*) ((uint32) &page_directory[0] - HIGHER_HALF));
	
}

void page_fault_handler(struct registers* r) {
	asm volatile("cli");
	
	boolean other = FALSE;
	switch (r->err_code) {
		case 0:
		case 1: {
			println("Kernel reading in nonpaged area.", 0x06);
			break;
			
		} case 2:
		case 3: {
			println("Kernel writing in nonpaged area.", 0x06);
			break;
			
		} case 4:
		case 5: {
			println("User reading in nonpaged area.", 0x06);
			break;
			
		} case 6:
		case 7: {
			println("User writing in nonpaged area.", 0x06);
			break;
			
		} default: {
			println("Unknown page fault.", 0x06);
			other = TRUE;
			break;
			
		}
		
	}
	
	//print_regs(r);
	while (TRUE);
	
}
