/*
 * types.h
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


#ifndef TYPES_H
	#define TYPES_H
	
	#define TRUE 1
	#define FALSE 0
	#define NULL ((void*) 0)
	
	#define HIGHER_HALF 0xC0000000
	
	typedef signed char int8;
	typedef unsigned char uint8;
	typedef uint8 boolean;
	
	typedef signed short int16;
	typedef unsigned short uint16;
	
	typedef signed int int32;
	typedef unsigned int uint32;
	typedef unsigned int uint;
	
	typedef signed long long int64;
	typedef unsigned long long uint64;
	
	typedef char* string;
	
	typedef signed int* intptr;
	typedef unsigned int* uintptr_t;
	typedef uint16 size;
	
	#define low_16(address) (uint16) ((address) & 0xFFFF)            
	#define high_16(address) (uint16) (((address) >> 16) & 0xFFFF)
	
	struct __attribute__((packed)) registers {
		unsigned int gs, fs, es, ds;
		unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
		unsigned int num, err_code;
		unsigned int eip, cs, eflags, useresp, ss;
		
	};
	
#endif
