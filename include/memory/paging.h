/*
 * paging.h
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

//http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
//http://www.jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html
//http://www.jamesmolloy.co.uk/tutorial_html/10.-User%20Mode.html

#ifndef PAGING_H
	#define PAGING_H
	
	#include "../types.h"
	#include "../screen.h"
	
	#include "kheap.h"
	#include "../interrupts/isr.h"
	#include "../interrupts/irq.h"
	
	/*extern long kstart;
	extern long kend;
	extern void load_page_dir(unsigned int*);
	
	void setup_paging(void);
	void page_fault_handler(struct registers* r);
	void exec(uint8* program);
	*/
	
	typedef struct {
		uint32 present : 1;
		uint32 rw : 1;
		uint32 user : 1;
		uint32 accessed : 1;
		uint32 dirty : 1;
		
		uint32 unused : 7;
		uint32 _frame : 20;
		
	} page;
	
	typedef struct {
		page pages[1024];
		
	} page_table;
	
	typedef struct {
		page_table* tables[1024];
		uint32 tables_physical[1024];
		uint32 physical_address;
		
	} page_directory;
	
	void init_paging(void);
	void switch_page_directory(page_directory* new);
	
	page* get_page(uint32 address, int make, page_directory* directory);
	void page_fault(struct registers* regs);
	
#endif
