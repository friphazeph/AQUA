/*
 * heap.h
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


#ifndef HEAP_H
	#define HEAP_H
	
	#include "../types.h"
	#include "../screen.h"
	
	#include "../utils/utils.h"
	#include "../utils/string.h"
	
	#define MAX_HEAP_SPACE 0x100000
	#define MAX_FRAMES 0xFFFFFF // 0x2000
	
	typedef struct frame {
		uint32 num;
		uint32 set;
		uint32 pos;
		
	} frame;
	
	typedef struct frame_set {
		frame start;
		uint32 len;
		
	} frame_set;
	
	void init_heap(void);
	
	frame first_available_frame(void);
	frame falloc(void);
	
	void ffree(frame _frame);
	
	frame_set first_available_frameset(uint32 len);
	frame get_frame(uint32 id);
	frame_set fsalloc(uint32 len);
	
	void* kmalloc(uint32 len);
	void kfree(void* __ptr, uint32 len);
	void strfree(string str);
	
#endif
