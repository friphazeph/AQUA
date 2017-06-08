/*
 * PIT_driver.c
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


#include "PIT_driver.h"

int timer_ticks = 0;
boolean second_passed = FALSE;
boolean buffer_blit = TRUE;

void timer_handler(struct registers* r) {
	timer_ticks++;
	
	if (timer_ticks % 18 == 0) {
		second_passed = TRUE;
		
	}
	
	if (buffer_blit) {
		GFX_update_framebuffer();
		update_mouse_cursor();
		
	}
	
}

boolean get_second_passed(void) {
	if (second_passed) {
		second_passed = FALSE;
		return TRUE;
	
	} else {
		return FALSE;
		
	}
	
}

void reset_timer(void) {
	timer_ticks = 0;
	
}

void timer_phase(int hertz) {
	int divisor = 1193180 / hertz;
	outportb(0x43, 0x36);
	outportb(0x40, divisor & 0xFF);
	outportb(0x40, divisor >> 8);
	
}

void timer_disable(void) {
	timer_phase(1);
	
}

void timer_install(boolean _buffer_blit) {
	buffer_blit = _buffer_blit;
	
	timer_ticks = 0;
	irq_add_handler(0, timer_handler);
	
}
