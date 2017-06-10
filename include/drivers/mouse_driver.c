/*
 * mouse_driver.c
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


#include "mouse_driver.h"

static uint8 buffer[3];
static uint8 offset;
static uint8 buttons;

static int32 mouse_x = 0;
static int32 mouse_y = 0;
static uint8 press = 127;
static uint8 release = 127;

static uint8 utpress = 127;
static uint8 utrelease = 127;

static int32 max_width = 800;
static int32 max_height = 600;

void mouse_move(int8 x, int8 y) {
	mouse_x += x;
	mouse_y += y;
	
	if (mouse_x > max_width) mouse_x = max_width;
	if (mouse_y > max_height) mouse_y = max_height;
	if (mouse_x < 0) mouse_x = 0;
	if (mouse_y < 0) mouse_y = 0;
	
}

void mouse_press(uint8 button) {
	press = button;
	utpress = press;
	
}

void mouse_release(uint8 button) {
	release = button;
	utrelease = release;
	
}

void mouse_handler(struct registers* r) {
	uint8 status = inportb(0x64);
	
	if (!(status & 0x20)) {
		return;
		
	}
	
	buffer[offset] = inportb(0x60);
	offset = (offset + 1) % 3;
	
	if (offset == 0) {
		if (buffer[1] != 0 || buffer[2] != 0) {
			mouse_move((int8) buffer[1], -((int8) buffer[2]));
			
		}
		
		uint8 i;
		for (i = 0; i < 3; i++) {
			if ((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))) {
				if (buttons & (0x1 << i)) mouse_release(i + 1);
				else mouse_press(i + 1);
				
			}
			
		}
		
		buttons = buffer[0];
		
	}
	
}

void mouse_install(void) {
	offset = 0;
	buttons = 0;
	
	max_width = GFX_get_mode_info('w') - 2;
	max_height = GFX_get_mode_info('h') - 2;
	
	outportb(0x64, 0xA8);
	outportb(0x64, 0x20);
	
	uint8 status = inportb(0x60) | 2;
	
	outportb(0x64, 0x60);
	outportb(0x60, status);
	
	outportb(0x64, 0xD4);
	outportb(0x60, 0xF4);
	inportb(0x60);
	
	irq_add_handler(12, mouse_handler);
	
}

int32 get_mouse_x(void) {
	return mouse_x;
	
}

int32 get_mouse_y(void) {
	return mouse_y;
	
}

uint8 untouch_mouse_press(void) { return utpress; }
uint8 untouch_mouse_release(void) { return utrelease; }

void clear_untouch(void) {
	utpress = 127;
	utrelease = 127;
	
}

uint8 poll_mouse_press(void) {
	uint8 temp = press;
	press = 127;
	return temp;
	
}

uint8 poll_mouse_release(void) {
	uint8 temp = release;
	release = 127;
	return temp;
	
}
