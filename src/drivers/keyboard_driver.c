/*
 * keyboard_driver.c
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


#include "keyboard_driver.h"

static boolean shift;

static uint8 press;
static uint8 release;

static boolean irq = FALSE;

void keyboard_handler(struct registers* r) {
	uint8 scancode;
	scancode = inportb(0x60);
	
	if (scancode & 0x80) release = scancode;
	else press = scancode;
	
}

void keyboard_install(boolean IRQ) {
	irq = IRQ;
	
	if (irq) {
		while (inportb(0x64) & 0x1) {
			inportb(0x60);
			
		}
		
		outportb(0x64, 0xae);
		outportb(0x64, 0x20);
		
		shift = FALSE;
		uint8 status = (inportb(0x60) | 1) & ~0x10;
		
		outportb(0x64, 0x60);
		outportb(0x60, status);
		outportb(0x60, 0xf4);
		
		irq_add_handler(1, keyboard_handler);
		
	}
	
}

boolean get_shift(void) {
	return shift;
	
}

uint8 poll_key(void) {
	uint8 scancode = 127;
	
	if (!irq) {
		while (!(inportb(0x64) & 0x1));
		scancode = inportb(0x60);
		
		if (scancode & 0x80) return 127;
		else return scancode;
		
	} else {
		uint8 temp = press;
		if (temp == 42) shift = TRUE;
		
		press = 127;
		return temp;
		
	}
	
}

uint8 poll_key_release(void) {
	uint8 temp = release;
	if (temp == 42) shift = FALSE;
	
	release = 127;
	return temp;
	
}
