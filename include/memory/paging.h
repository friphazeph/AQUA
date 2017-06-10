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


#ifndef PAGING_H
	#define PAGING_H
	
	#include "../types.h"
	#include "../screen.h"
	#include "../interrupts/isr.h"
	
	extern long kstart;
	extern long kend;
	extern void load_page_dir(unsigned int*);
	
	void setup_paging(void);
	void page_fault_handler(struct registers* r);
	void exec(uint8* program);
	
#endif
