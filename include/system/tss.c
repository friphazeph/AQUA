/*
 * tss.c
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


#include "tss.h"

static tss_entry_t tss;

void write_tss(GDT_entry_bits* g) {
	uint32 base = (uint32) &tss;
	uint32 limit = sizeof(tss);
	
	g->limit_low = limit & 0xFFFF;
	g->base_low = base & 0xFFFFFF;
	
	g->accessed = TRUE;
	g->read_write = FALSE;
	g->conforming_expand_down = FALSE;
	g->code = TRUE;
	g->always_1 = FALSE;
	g->DPL = 3;
	
	g->present = TRUE;
	g->limit_high = (limit & 0xF0000) >> 16;
	g->available = FALSE;
	g->always_0 = FALSE;
	
	g->big = FALSE;
	g->gran = FALSE;
	g->base_high = (base & 0xFF000000) >> 24;
	
	memset(&tss, 0, sizeof(tss));
	
	tss.ss0 = REPLACE_KERNEL_DATA_SEGMENT;
	tss.esp0 = REPLACE_KERNEL_STACK_ADDRESS;
	
}

void set_kernel_stack(uint32 stack) {
	tss.esp0 = stack;
	
}

void install_tss(GDT_entry* entry) {
	tss_install();
	
}

void flush_tss(void) {
	tss_flush();
	
}
