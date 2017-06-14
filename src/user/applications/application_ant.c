/*
 * application_ant.c
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

#include "application_ant.h"

static int32 x = -1;
static int32 y = -1;

void launch_application_ant(void) {
	uint32 ant_colour = 0x00dba087;
	uint32 earth_colour = 0x00471c01;
	
	uint32 width = GFX_get_mode_info('w');
	uint32 height = GFX_get_mode_info('h');
	
	uint8* buffer_trace = (uint8*) kmalloc(width * height * 3);
	buffer_trace = GFX_compile_section_to_ptr8(buffer_trace, 0, 0, width, height);
	
	if (x < 0) x = width / 2;
	if (y < 0) y = height / 2;
	
	hide_cursor();
	GFX_wash_hex(earth_colour);
	
	uint8 key;
	uint8 dir = 1;
	
	boolean running = TRUE;
	while (running) {
		key = poll_key();
		
		//sleep(1);
		if (GFX_get_pixel_hex(x, y) == earth_colour) {
			dir--;
			if (dir == 0) dir = 4;
			
		} else {
			dir++;
			if (dir == 5) dir = 1;
			
		}
		
		if (GFX_get_pixel_hex(x, y) == ant_colour) GFX_put_pixel_hex(x, y, earth_colour);
		else GFX_put_pixel_hex(x, y, ant_colour);
		
		if (dir == 1) x--;
		if (dir == 2) y++;
		if (dir == 3) x++;
		if (dir == 4) y--;
		
		if (key == 28) {
			running = FALSE;
			break;
			
		}
		
	}
	
	GFX_blit_image(0, 0, width, height, buffer_trace);
	show_cursor();
	
}
