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

static uint32* frames;
static uint32 frame_count;

extern uint32 placement_address;

#define INDEX_FROM_BIT(a) (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

static void set_frame(uint32 frame_address) {
	uint32 _frame = frame_address / 0x1000; /// / MAX_FRAMES
	uint32 idx = INDEX_FROM_BIT(_frame);
	uint32 off = OFFSET_FROM_BIT(_frame);
	frames[idx] |= (0x1 << off);
	
}

static void clear_frame(uint32 frame_address) {
	uint32 _frame = frame_address / 0x1000; /// / MAX_FRAMES
	uint32 idx = INDEX_FROM_BIT(_frame);
	uint32 off = OFFSET_FROM_BIT(_frame);
	frames[idx] &= ~(0x1 << off);
	
}

static uint32 test_frame(uint32 frame_address) {
	uint32 _frame = frame_address / 0x1000; /// / MAX_FRAMES
	uint32 idx = INDEX_FROM_BIT(_frame);
	uint32 off = OFFSET_FROM_BIT(_frame);
	
	return frames[idx] & (0x1 << off);
	
}

static uint32 first_frame(void) {
	int i;
	int j;
	for (i = 0; i < INDEX_FROM_BIT(frame_count); i++) {
		if (frames[i] != 0xFFFFFFFF) {
			for (j = 0; j < 32; j++) {
				uint32 to_test;
				
				if (!(frames[i] & to_test)) {
					return i * 32 + j;
					
				}
				
			}
			
		}
		
	}
	
}

void alloc_frame(page* _page, boolean is_kernel, boolean is_writeable) {
	if (_page->_frame != 0) return;
	uint32 idx = first_frame();
	
	if (idx == (uint32) - 1) {
		println("No more free frames were found.", 0x06);
		
	}
	
	set_frame(idx * 0x1000); /// / MAX_FRAMES
	_page->present = TRUE;
	_page->rw = (is_writeable) ? TRUE : FALSE;
	_page->user = (is_kernel) ? FALSE : TRUE;
	_page->_frame = idx;
	
}

void free_frame(page* _page) {
	uint32 _frame;
	if (!(_frame = _page->_frame)) return;
	
	clear_frame(_frame);
	_page->_frame = 0x0;
	
}

void init_paging(void) {
	uint32 mem_end_page = 0x1000000; // 16MB big
	
	frame_count = mem_end_page / 0x1000; /// / MAX_FRAMES
	frames = (uint32*) page_kmalloc(INDEX_FROM_BIT(frame_count));
	memset(frames, 0, INDEX_FROM_BIT(frame_count));
	
	kernel_directory = (page_directory*) page_kmalloc_a(sizeof(page_directory));
	memset(kernel_directory, 0, sizeof(page_directory));
	current_directory = kernel_directory;
	
	int i = 0;
	while (i < placement_address) {
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000; /// MAX_FRAMES;
		
	}
	
	irq_add_handler(14, page_fault);
	switch_page_directory(kernel_directory);
	
}

void switch_page_directory(page_directory* new) {
	current_directory = new;
	asm volatile("mov %0, %%cr3" :: "r"(&new->tables_physical));
	
	uint32 cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0" :: "r"(cr0));
	
}

page* get_page(uint32 address, int make, page_directory* directory) {
	address /= 0x1000; /// / MAX_FRAMES;
	uint32 table_idx = address / 1024;
	
	if (directory->tables[table_idx]) return &directory->tables[table_idx]->pages[address % 1024];
	if (make) {
		uint32 temp;
		directory->tables[table_idx] = (page_table*) page_kmalloc_ap(sizeof(page_table), &temp)
		memset(dir->tables[table_idx], 0, 0x1000); /// MAX_FRAMES
		dir->tables_physical[table_idx] = temp | 0x7;
		
		return &directory->tables[table_idx]->pages[address % 1024];
		
	}
	
	return 0;
	
}

void page_fault(struct registers* regs) {
	uint32 faulting_addresses;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	
	int present = !(registers.err_code & 0x1);
	int rw = registers.err_code & 0x2;
	int us = registers.err_code & 0x4;
	int reserved = registers.err_code & 0x8;
	int id = registers.err_code & 0x10;
	
	println("A page fault has occured.", 0x06);
	if (present) println("\tpresent", 0x06);
	if (rw) println("\tread only", 0x06);
	if (us) println("\tuser mode", 0x06);
	if (reserved) println("\treserved", 0x06);
	
	print("\tat 0x", 0x06);
	print_hex(faulting_addresses, 0x06);
	println("", 0x06);
	
}
