/*
 * system.c
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

#include "system.h"

uint8 inportb(uint16 _port) {
	uint8 rv;
	__asm__ __volatile__("inb %1, %0" : "=a" (rv) : "dN" (_port));
	
	return rv;
	
}

void outportb(uint16 _port, uint8 _data) {
	__asm__ __volatile__("outb %1, %0" : : "dN" (_port), "a" (_data));
	
}

uint16 inportw(uint16 _port) {
	uint16 rv;
	__asm__ __volatile__("inw %1, %0" : "=a" (rv) : "dN" (_port));
	
	return rv;
	
}

void outportw(uint16 _port, uint16 _data) {
	__asm__ __volatile__("outw %1, %0" : : "dN" (_port), "a" (_data));
	
}

uint32 inportl(uint16 _port) {
	uint32 rv;
	__asm__ __volatile__("inl %1, %0" : "=a" (rv) : "dN" (_port));
	
	return rv;
	
}

void outportl(uint16 _port, uint32 _data) {
	__asm__ __volatile__("outl %1, %0" : : "dN" (_port), "a" (_data));
	
}

void syscall(uint callid) {
	asm volatile("int $0x80" : : "a" (callid));
	
}
