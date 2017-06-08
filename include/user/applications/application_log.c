/*
 * application_log.c
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

#include "application_log.h"

void launch_application_log(void) {
	uint8* _api_icon_back = api_get_icon("back");
	uint8* _api_font_mono_15px = api_get_font("monospace", 15);
	
	uint32 width = GFX_get_mode_info('w');
	uint32 height = GFX_get_mode_info('h');
	uint32 buffer_size = width * height * 3;
	
	uint8* buffer = (uint8*) kmalloc(buffer_size);
	uint8* buffer_trace = (uint8*) kmalloc(buffer_size);
	uint8* buffer_final = (uint8*) kmalloc(buffer_size);
	
	buffer_trace = GFX_compile_section_to_ptr8(buffer_trace, 0, 0, width, height);
	buffer = GFX_blit_image_to_ptr8(buffer, width, 0, 0, width, height, buffer_trace);
	
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 0, 0, width, height, 0x00000000, 120);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, height - 30, 20, 20, _api_icon_back, 0x0000FF00);
	
	string line = "hey!";
	char* log = get_log('l');
	
	int ll = 0;
	int nl = 0;
	
	int l;
	for (l = 0; l < get_log_length(); l++) {
		if (nl * 20 + 10 > height - 60) {
			break;
			
		} if (ll > width / 8 || log[l] == '\n') {
			line[ll] = 0;
			ll = 0;
			buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 10, nl * 20 + 10, 0, 130, _api_font_mono_15px, line, 0x0000FF00);
			nl++;
			
		} else {
			if (log[l] == '\t') {
				line[ll] = ' ';
				line[ll + 1] = ' ';
				line[ll + 2] = ' ';
				line[ll + 3] = ' ';
				
				ll += 4;
				
			} else {
				line[ll] = log[l];
				ll++;
				
			}
			
		}
		
	}
	
	hide_cursor();
	
	uint16 a;
	for (a = 0; a < 255; a += 30) {
		buffer_final = GFX_blit_image_to_ptr8(buffer_final, width, 0, 0, width, height, buffer);
		buffer_final = GFX_blit_image_to_ptr8_wa(buffer_final, width, 0, 0, width, height, buffer_trace, 255 - a);
		
		GFX_blit_image(0, 0, width, height, buffer_final);
		
	}
	
	GFX_blit_image(0, 0, width, height, buffer);
	show_cursor();
	
	boolean apping = TRUE;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint8 key;
	
	while (apping) {
		key = poll_key();
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (key == 1) {
			apping = FALSE;
			break;
			
		}
		
		if (my > height - 40 && mr == 1) {
			if (mx > width - 40) {
				apping = FALSE;
				break;
				
			}
			
		}
		
	}
	
	hide_cursor();
	
	for (a = 0; a < 255; a += 30) {
		buffer_final = GFX_blit_image_to_ptr8(buffer_final, width, 0, 0, width, height, buffer);
		buffer_final = GFX_blit_image_to_ptr8_wa(buffer_final, width, 0, 0, width, height, buffer_trace, a);
		
		GFX_blit_image(0, 0, width, height, buffer_final);
		
	}
	
	GFX_blit_image(0, 0, width, height, buffer_trace);
	
	kfree(buffer, buffer_size);
	
	kfree(buffer_trace, buffer_size);
	kfree(buffer_final, buffer_size);
	
}
