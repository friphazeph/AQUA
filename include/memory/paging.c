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
	uint32 frame = frame_address / 0x1000;
	uint32 idx = INDEX_FROM_BIT(frame);
	uint32 off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
	
}

void enable_paging(void) {
	
	
}

void switch_page_directory(page_directory* new) {
	
	
}

page* get_page(uint32 address, int make, page_directory* directory) {
	
	
}

void page_fault(struct registers regs) {
	
	
}
