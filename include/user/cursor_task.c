/*
 * cursor_task.c
 * 
 * Copyright 2017 Obiwac
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

#include "cursor_task.h"
//#include "../graphics/GFX_loader_files/images/cursor.h"

static uint32 width;
static uint32 height;

static uint32 _cursor_x;
static uint32 _cursor_y;

static uint32 _cursor_ox;
static uint32 _cursor_oy;

static boolean hidden = FALSE;
static boolean buffer_blit = TRUE;
static boolean black = FALSE;

static uint8* _cursor_trace;
static uint8 graphical_mouse_cursor[1024]={0,0,0,210,18,18,18,192,23,23,23,78,0,0,0,36,0,0,0,7,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,13,13,13,197,59,59,59,248,21,21,21,243,26,26,26,211,14,14,14,99,0,0,0,36,0,0,0,7,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,12,12,12,116,30,30,30,239,185,185,185,254,116,116,116,250,32,32,32,239,26,26,26,211,14,14,14,99,0,0,0,36,0,0,0,7,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,7,7,7,37,18,18,18,223,148,148,148,249,240,240,240,255,219,219,219,255,116,116,116,250,32,32,32,239,17,17,17,224,14,14,14,99,0,0,0,36,0,0,0,7,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,8,14,14,14,110,33,33,33,239,220,220,220,255,254,254,254,255,248,248,248,255,219,219,219,255,158,158,158,249,32,32,32,239,17,17,17,224,7,7,7,140,0,0,0,36,0,0,0,7,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,1,7,7,7,36,18,18,18,223,148,148,148,249,248,248,248,255,255,255,255,255,255,255,255,255,248,248,248,255,219,219,219,255,158,158,158,249,47,47,47,233,14,14,14,228,7,7,7,140,0,0,0,36,0,0,0,7,255,255,255,0,255,255,255,0,0,0,0,8,11,11,11,122,39,39,39,237,220,220,220,255,255,255,255,255,249,249,249,255,220,220,220,255,213,213,213,255,206,206,206,255,177,177,177,255,133,133,133,249,19,19,19,233,0,0,0,231,0,0,0,141,0,0,0,43,255,255,255,0,0,0,0,1,7,7,7,36,18,18,18,223,148,148,148,249,248,248,248,255,201,201,201,255,30,30,30,253,27,27,27,235,27,27,27,231,26,26,26,231,22,22,22,231,4,4,4,231,0,0,0,227,0,0,0,227,0,0,0,239,255,255,255,0,255,255,255,0,0,0,0,8,11,11,11,122,39,39,39,237,219,219,219,255,195,195,195,255,31,31,31,232,6,6,6,79,6,6,6,44,6,6,6,43,6,6,6,43,0,0,0,43,0,0,0,43,0,0,0,42,0,0,0,42,255,255,255,0,255,255,255,0,0,0,0,1,7,7,7,36,18,18,18,223,148,148,148,249,188,188,188,255,31,31,31,227,6,6,6,44,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,7,9,9,9,122,39,39,39,237,178,178,178,254,26,26,26,231,6,6,6,43,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,1,0,0,0,36,14,14,14,229,134,134,134,248,22,22,22,231,6,6,6,43,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,7,9,9,9,122,15,15,15,237,4,4,4,230,0,0,0,43,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,1,0,0,0,36,0,0,0,228,0,0,0,228,0,0,0,43,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,7,0,0,0,122,0,0,0,232,0,0,0,42,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,0,0,0,43,0,0,0,237,0,0,0,60,0,0,0,1,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,255,255,255,0,};

void invert_cursor(char value) {
	if ((value == 't' && !black) || (value == 'f' && black)) {
		black = !black;
		
		int i;
		for (i = 0; i < CURSOR_SIZE * CURSOR_SIZE * 4; i += 4) {
			graphical_mouse_cursor[i] = 255 - graphical_mouse_cursor[i];
			graphical_mouse_cursor[i + 1] = 255 - graphical_mouse_cursor[i + 1];
			graphical_mouse_cursor[i + 2] = 255 - graphical_mouse_cursor[i + 2];
			
		}
		
	}
	
}

void init_cursor(boolean _buffer_blit) {
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
	buffer_blit = _buffer_blit;
	if (!buffer_blit) _cursor_trace = (uint8*) kmalloc(CURSOR_SIZE * CURSOR_SIZE * 3);
	
	_cursor_x = get_mouse_x();
	_cursor_y = get_mouse_y();
	
}

void blit_cursor(void) {
	if (!buffer_blit) {
		GFX_blit_image(_cursor_ox, _cursor_oy, CURSOR_SIZE, CURSOR_SIZE, _cursor_trace);
		_cursor_trace = GFX_compile_section_to_ptr8(_cursor_trace, _cursor_x, _cursor_y, CURSOR_SIZE, CURSOR_SIZE);
		
	}
	
	GFX_blit_32bit_image(_cursor_x, _cursor_y, CURSOR_SIZE, CURSOR_SIZE, graphical_mouse_cursor);
	
}

void update_mouse_cursor(void) {
	if (!hidden) {
		_cursor_ox = _cursor_x;
		_cursor_oy = _cursor_y;
		
		_cursor_x = get_mouse_x();
		_cursor_y = get_mouse_y();
		
		if (_cursor_x != _cursor_ox || \
			_cursor_y != _cursor_oy || buffer_blit) {
			blit_cursor();
			
		}
		
	}
	
}

void show_cursor(void) {
	if (!buffer_blit) {
		hidden = FALSE;
		_cursor_trace = GFX_compile_section_to_ptr8(_cursor_trace, _cursor_x, _cursor_y, CURSOR_SIZE, CURSOR_SIZE);
		update_mouse_cursor();
		//blit_cursor();
		
	}
	
}

void hide_cursor(void) {
	if (!buffer_blit) {
		GFX_blit_image(_cursor_ox, _cursor_oy, CURSOR_SIZE, CURSOR_SIZE, _cursor_trace);
		hidden = TRUE;
		
	}
	
}
