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

static int timer_ticks;

static boolean buffer_blit = TRUE;
static boolean dragging_top;

static int32 mx;
static int32 my;
static uint8 mp;
static uint8 mr;

static uint32 width;
static uint32 height;

void timer_handler(struct registers* r) {
	timer_ticks++;
	
	mx = get_mouse_x();
	my = get_mouse_y();
	
	mp = untouch_mouse_press();
	mr = untouch_mouse_release();
	
	clear_untouch();
	
	if (my <= 0 && mp == 1) dragging_top = TRUE;
	if (dragging_top && mp != 1) dragging_top = FALSE;
	
	if (dragging_top && my > height / 2 && mr == 1) {
		enter_sleep_mode();
		dragging_top = FALSE;
		
	}
	
	if (buffer_blit) {
		GFX_update_framebuffer();
		update_mouse_cursor();
		
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
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
	buffer_blit = _buffer_blit;
	dragging_top = FALSE;
	
	timer_ticks = 0;
	irq_add_handler(0, timer_handler);
	
}
