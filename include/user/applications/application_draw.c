/*
 * application_draw.c
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

#include "application_draw.h"

static uint32 width;
static uint32 height;

static uint8 brush_colour_r = 255;
static uint8 brush_colour_g = 255;
static uint8 brush_colour_b = 255;

static void display_side_bar(void) {
	uint16 time = 1;
	uint32 side_bar_buffer_size = (width / 5) * height * 3;
	
	uint8* side_bar_buffer = (uint8*) kmalloc(side_bar_buffer_size);
	uint8* side_bar_buffer_trace = (uint8*) kmalloc(side_bar_buffer_size);
	
	side_bar_buffer_trace = GFX_compile_section_to_ptr8(side_bar_buffer_trace, 0, 0, width / 5, height);
	
	float s = 0;
	uint32 x;
	
	uint f;
	for (f = 0; f < 70 * time; f++) {
		s += ((s / 10.0f) + 0.2f) / time;
		x = (width / 5) - ((uint32) ((float) (width / 5) / (s + 1.0f)));
		
		side_bar_buffer = GFX_blit_image_to_ptr8(side_bar_buffer, width / 5, 0, 0, width / 5, height, side_bar_buffer_trace);
		side_bar_buffer = GFX_fill_rect_hex_to_ptr8_wa(side_bar_buffer, width / 5, 0, 0, x, height, 0x00000000, 40);
		
		GFX_blit_image(0, 0, width / 5, height, side_bar_buffer);
		
	}
	
	float g = 255;
	for (f = 0; f < 70 * time; f++) {
		s += ((s / 10.0f) + 0.2f) / time;
		g -= 255.0f / ((float) (70 * time));
		
		GFX_fill_rect_hex_wa(0, 0, width / 15, (uint32) (g * ((float) height / 255.0f)), 0x00FF0000, 1);
		GFX_fill_rect_hex_wa(width / 15, 0, width / 15, (uint32) (g * ((float) height / 255.0f)), 0x0000FF00, 1);
		GFX_fill_rect_hex_wa((width / 15) * 2, 0, width / 15, (uint32) (g * ((float) height / 255.0f)), 0x000000FF, 1);
		
	}
	
	kfree(side_bar_buffer, side_bar_buffer_size);
	kfree(side_bar_buffer_trace, side_bar_buffer_size);
	
}

void launch_application_draw(void) {
	uint8 brush_colour_amount;
	uint8* _api_icon_back = api_get_icon("back");
	
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	uint32 buffer_size = width * height * 3;
	
	uint8* buffer = (uint8*) kmalloc(buffer_size);
	uint8* buffer_trace = (uint8*) kmalloc(buffer_size);
	uint8* buffer_final = (uint8*) kmalloc(buffer_size);
	
	buffer_trace = GFX_compile_section_to_ptr8(buffer_trace, 0, 0, width, height);
	buffer = GFX_blit_image_to_ptr8(buffer, width, 0, 0, width, height, buffer_trace);
	
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 0, 0, width, height, 0x00000000, 120);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, height - 30, 20, 20, _api_icon_back, 0x0000FF00);
	buffer = GFX_fill_rect_hex_to_ptr8(buffer, width, width - 30, 10, 20, 20, GFX_compile_rgb_to_hex(brush_colour_r, brush_colour_g, brush_colour_b));
	
	hide_cursor();
	
	uint16 a;
	for (a = 0; a < 255; a += 30) {
		buffer_final = GFX_blit_image_to_ptr8(buffer_final, width, 0, 0, width, height, buffer);
		buffer_final = GFX_blit_image_to_ptr8_wa(buffer_final, width, 0, 0, width, height, buffer_trace, 255 - a);
		
		GFX_blit_image(0, 0, width, height, buffer_final);
		
	}
	
	GFX_blit_image(0, 0, width, height, buffer);
	display_side_bar();
	
	show_cursor();
	boolean apping = TRUE;
	
	int32 mx;
	int32 my;
	int32 omx = width / 2;
	int32 omy = height / 2;
	
	uint8 mp;
	uint8 mr;
	
	uint8 key;
	
	boolean clicking = FALSE;
	
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
		
		if (mp == 1) {
			clicking = TRUE;
			hide_cursor();
			
		} if (mr == 1) {
			clicking = FALSE;
			show_cursor();
			
		}
		
		if (mx > width - 40 && my > height - 40 && mr == 1) {
			apping = FALSE;
			break;
			
		} else if (clicking && mx < width / 5) {
			brush_colour_amount = 255 - ((uint8) (((float) my / (float) height) * 255.0f));
			
			if (mx < width / 15) brush_colour_r = brush_colour_amount;
			else if (mx < (width / 15) * 2) brush_colour_g = brush_colour_amount;
			else brush_colour_b = brush_colour_amount;
			
			GFX_fill_rect_hex(width - 30, 10, 20, 20, GFX_compile_rgb_to_hex(brush_colour_r, brush_colour_g, brush_colour_b));
			
		} else if (clicking) {
			GFX_draw_line(omx, omy, mx, my, GFX_compile_rgb_to_hex(brush_colour_r, brush_colour_g, brush_colour_b));
			
		}
		
		omx = mx;
		omy = my;
		
	}
	
	hide_cursor();
	buffer = GFX_compile_section_to_ptr8(buffer, 0, 0, width, height);
	
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
