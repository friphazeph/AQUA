/*
 * memory.c
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


#include "memory.h"

static free_block free_block_list_head = {0, 0};
static const size overhead = sizeof(size);
static const size align_to = 16;

int memcmp(const void* buf1, const void* buf2, size count) {
	if (!count) {
		return 0;
		
	}
	
	while (--count && *(char*) buf1 == *(char*) buf2) {
		buf1 = (char*) buf1 + 1;
		buf2 = (char*) buf2 + 1;
		
	}
	
	return *((uint8*) buf1) - *((uint8*) buf2);
	
}

void memcpy(char* source, char* dest, int nbytes) {
	int i;
	for (i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
		
	}
	
}

void* memcpy_ext2(void* dest, const void* src, size count) {
	const char* sp = (const char*) src;
	char* dp = (char*) dest;
	
	for (; count != 0; count--) {
		*dp++ = *sp++;
		
	}
	
	return dest;
	
}

void memset(uint8* dest, uint8 val, uint32 len) {
	uint8* temp = (uint8*) dest;
	
	for (; len != 0; len--) {
		*temp++ = val;
		
	}
	
}

void* alloc(size __size) {
	__size = (__size + sizeof(size) + (align_to - 1)) & ~ (align_to - 1);
	
	free_block* block = free_block_list_head.next;
	free_block** head = &(free_block_list_head.next);
	
	while (block != 0) {
		if (block->_size >= __size) {
			*head = block->next;
			return ((char*) block) + sizeof(size);
			
		}
		
		head = &(block->next);
		block = block->next;
		
	}
	
	//block = (free_block*) sbrk(__size);
	block->_size = __size;
	
	return ((char*) block) + sizeof(size);
	
}

void free(void* __ptr) {
	free_block* block = (free_block*) (((char*) __ptr) - sizeof(size));
	block->next = free_block_list_head.next;
	free_block_list_head.next = block;
	
}
