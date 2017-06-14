/*
 * memory.h
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


#ifndef MEMORY_H
	#define MEMORY_H
	
	#include "../types.h"
	#include "heap.h"
	
	typedef struct free_block {
		size _size;
		struct free_block* next;
		
	} free_block;
	
	uintptr_t current_break;
	
	int memcmp(const void* buf1, const void* buf2, size count);
	void memcpy(char* source, char* dest, int nbytes);
	void* memcpy_ext2(void* dest, const void* src, size count);
	void memset(uint8* dest, uint8 val, uint32 len);
	
	void* sbrk(intptr incr);
	void* alloc(size __size);
	void free(void* __ptr);
	
#endif
