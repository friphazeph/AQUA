/*
 * application_about.c
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

#include "application_about.h"

static uint8 icon_credits[12000]={0,0,0,0,255,255,0,255,255,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,255,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,0,0,255,255,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,255,255,255,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,255,255,255,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,255,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,255,255,255,255,0,0,255,255,255,255,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,255,255,255,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

void launch_application_about(void) {
	uint8* _api_icon_back = api_get_icon("back");
	uint8** _api_font_aqua_50px = api_get_font("aqua", 50);
	uint8** _api_font_aqua_20px = api_get_font("aqua", 20);
	
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
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, 50, height - 30, 20, 20, icon_credits, 0x00FEF199);
	
	string temp = "About the AQUA OS";
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, (width / 2) - (GFX_get_text_potential_length(temp, 0, 2681, _api_font_aqua_50px) / 2), 10, 0, 2681, _api_font_aqua_50px, temp, 0x0000CDCD);
	
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 50, 80, 0, 439, _api_font_aqua_20px, "This page is all about the AQUA OS", 0x00FFD700);
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 10, 120, 0, 439, _api_font_aqua_20px, "I'll add more here when I find something interesting to put down.", 0x00FFFFFF);
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 10, 150, 0, 439, _api_font_aqua_20px, "But for now, welcome!", 0x00FFFFFF);
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 10, 180, 0, 439, _api_font_aqua_20px, "And thank you for trying out the developement build of AQUA OS SVE 1.4!", 0x00FFFFFF);
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 85, height - 30, 0, 439, _api_font_aqua_20px, "Credits", 0x00FFFFFF);
	
	uint8* credits_buffer = (uint8*) kmalloc(buffer_size);
	credits_buffer = GFX_fill_rect_hex_to_ptr8(credits_buffer, width, 0, 0, width, height, 0x00000000);
	credits_buffer = GFX_blit_icon_hex_to_ptr8(credits_buffer, width, width - 30, height - 30, 20, 20, _api_icon_back, 0x0000FF00);
	
	temp = "Credits";
	credits_buffer = GFX_blit_text_hex_to_ptr8(credits_buffer, width, (width / 2) - (GFX_get_text_potential_length(temp, 0, 2681, _api_font_aqua_50px) / 2), 10, 0, 2681, _api_font_aqua_50px, temp, 0x00FFFFFF);
	
	hide_cursor();
	
	UI_animate_desktop("fade", 30, buffer);
	
	uint16 a;
	/*for (a = 0; a < 255; a += 30) {
		buffer_final = GFX_blit_image_to_ptr8(buffer_final, width, 0, 0, width, height, buffer);
		buffer_final = GFX_blit_image_to_ptr8_wa(buffer_final, width, 0, 0, width, height, buffer_trace, 255 - a);
		
		GFX_blit_image(0, 0, width, height, buffer_final);
		
	}
	*/
	GFX_blit_image(0, 0, width, height, buffer);
	show_cursor();
	
	boolean apping = TRUE;
	boolean crediting = FALSE;
	
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
				
			} else {
				hide_cursor();
				UI_animate_desktop("slide_up", 1, credits_buffer);
				
				crediting = TRUE;
				show_cursor();
				
				while (crediting) {
					update_mouse_cursor();
					
					mx = get_mouse_x();
					my = get_mouse_y();
					
					mp = poll_mouse_press();
					mr = poll_mouse_release();
					
					if (mx > width - 40 && my > height - 40 && mr == 1) {
						hide_cursor();
						
						for (a = 0; a < 255; a += 30) {
							buffer_final = GFX_blit_image_to_ptr8(buffer_final, width, 0, 0, width, height, buffer);
							buffer_final = GFX_blit_image_to_ptr8_wa(buffer_final, width, 0, 0, width, height, credits_buffer, 255 - a);
							
							GFX_blit_image(0, 0, width, height, buffer_final);
							
						}
						
						GFX_blit_image(0, 0, width, height, buffer);
						show_cursor();
						
						crediting = FALSE;
						break;
						
					}
					
				}
				
			}
			
		}
		
	}
	
	hide_cursor();
	UI_animate_desktop("fade", 30, buffer_trace);
	
	/*for (a = 0; a < 255; a += 30) {
		buffer_final = GFX_blit_image_to_ptr8(buffer_final, width, 0, 0, width, height, buffer);
		buffer_final = GFX_blit_image_to_ptr8_wa(buffer_final, width, 0, 0, width, height, buffer_trace, a);
		
		GFX_blit_image(0, 0, width, height, buffer_final);
		
	}
	
	GFX_blit_image(0, 0, width, height, buffer_trace);
	*/
	kfree(buffer, buffer_size);
	kfree(credits_buffer, buffer_size);
	
	kfree(buffer_trace, buffer_size);
	kfree(buffer_final, buffer_size);
	
}
