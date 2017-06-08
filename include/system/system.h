/*
 * system.h
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


#ifndef SYSTEM_H
	#define SYSTEM_H
	
	#include "../types.h"
	
	uint8 inportb(uint16 _port);
	void outportb(uint16 _port, uint8 _data);
	
	uint16 inportw(uint16 _port);
	void outportw(uint16 _port, uint16 _data);
	
	uint32 inportl(uint16 _port);
	void outportl(uint16 _port, uint32 _data);
	
	void syscall(uint callid);
	
#endif
