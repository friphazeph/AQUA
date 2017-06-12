/*
 * kheap.c
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


#include "kheap.h"

extern uint32 end;
uint32 placement_address = (uint32) &end;

uint32 page_kmalloc_int(uint32 sz, int align, uint32 *phys) {
	if (align == 1 && (placement_address & 0xFFFFF000)) {
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
		
	}
	
	if (phys) {
		*phys = placement_address;
		
	}
	
	uint32 temp = placement_address;
	placement_address += sz;
	
	return temp;
	
}

uint32 page_kmalloc_a(uint32 sz) {
	return page_kmalloc_int(sz, 1, 0);
	
}

uint32 page_kmalloc_p(uint32 sz, uint32 *phys) {
	return page_kmalloc_int(sz, 0, phys);
	
}

uint32 page_kmalloc_ap(uint32 sz, uint32 *phys) {
	return page_kmalloc_int(sz, 1, phys);
	
}

uint32 page_kmalloc(uint32 sz) {
	return page_kmalloc_int(sz, 0, 0);
	
}
