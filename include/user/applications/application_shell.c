/*
 * application_shell.c
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

#include "application_shell.h"

static uint32 width;
static uint32 height;

static uint32 entry_count;

static uint8* _api_icon_off;
static uint8* _api_icon_back;
static uint8* _api_icon_save;
static uint8* _api_icon_load;
static uint8* _api_icon_next;

static uint8** _api_font_aqua_20px;
static uint8** _api_font_aqua_50px;

static boolean saved;
static boolean loaded = FALSE;

uint8* add_entry(char check, uint8* next, uint8** font, uint8* buffer, string text, uint32 colour) {
	if (check != 'n') buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 10, 90 + (40 * entry_count), 20, 20, colour, 40);
	if (check == 't') buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 15, 95 + (40 * entry_count), 10, 10, colour, 90);
	
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 40, 90 + (40 * entry_count), 0, 439, font, translate(text), colour);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, 90 + (40 * entry_count), 20, 20, next, colour);
	
	entry_count++;
	return buffer;
	
}

void update_entries(uint8* buffer_overlay, uint8* buffer) {
	entry_count = 0;
	buffer_overlay = GFX_blit_image_to_ptr8(buffer_overlay, width, 0, 0, width, height, buffer);
	
	if (saved) buffer_overlay = GFX_blit_icon_hex_to_ptr8(buffer_overlay, width, 50, height - 30, 20, 20, _api_icon_save, 0x00292929);
	else buffer_overlay = GFX_blit_icon_hex_to_ptr8(buffer_overlay, width, 50, height - 30, 20, 20, _api_icon_save, 0x00C40E6C);
	
	buffer_overlay = add_entry('n', _api_icon_next, _api_font_aqua_20px, buffer_overlay, "Reformat disk", 0x00FF0000);
	buffer_overlay = add_entry('n', _api_icon_next, _api_font_aqua_20px, buffer_overlay, "Reset settings", 0x00FF0000);
	buffer_overlay = add_entry('n', _api_icon_next, _api_font_aqua_20px, buffer_overlay, "Startup assistant", 0x00FFFFFF);
	
	buffer_overlay = add_entry(get_setting(0), _api_icon_next, _api_font_aqua_20px, buffer_overlay, "Disable warnings", 0x00FFFFFF);
	buffer_overlay = add_entry(get_setting(1), _api_icon_next, _api_font_aqua_20px, buffer_overlay, "Fast transitions", 0x00FFFFFF);
	
	hide_cursor();
	UI_animate_desktop("fade", 30, buffer_overlay);
	show_cursor();
	
}

void launch_application_shell(void) {
	if (!loaded) {
		_api_icon_off = api_get_icon("off");
		_api_icon_back = api_get_icon("back");
		_api_icon_next = api_get_icon("next");
		_api_icon_save = api_get_icon("save");
		_api_icon_load = api_get_icon("load");
		
		_api_font_aqua_20px = api_get_font("aqua", 20);
		_api_font_aqua_50px = api_get_font("aqua", 50);
		
		loaded = TRUE;
		
	}
	
	load_settings();
	saved = TRUE;
	
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
	uint32 buffer_size = width * height * 3;
	
	uint8* buffer = (uint8*) kmalloc(buffer_size);
	uint8* buffer_overlay = (uint8*) kmalloc(buffer_size);
	uint8* buffer_trace = (uint8*) kmalloc(buffer_size);
	
	buffer_trace = GFX_compile_section_to_ptr8(buffer_trace, 0, 0, width, height);
	
	buffer = GFX_compile_section_to_ptr8(buffer, 0, 0, width, height);
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 0, 0, width, height, 0x00000000, 120);
	
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, height - 30, 20, 20, _api_icon_back, 0x0000FF00);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, 90, height - 30, 20, 20, _api_icon_load, 0x00C48F0E);
	
	string temp = translate("Settings");
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, (width / 2) - (GFX_get_text_potential_length(temp, 0, 2681, _api_font_aqua_50px) / 2), 10, 0, 2681, _api_font_aqua_50px, temp, 0x00FFFFFF);
	update_entries(buffer_overlay, buffer);
	
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
		
		if (mr == 1) {
			if (mx < 80 && my > height - 40) {
				if (!saved) {
					saved = TRUE;
					save_settings();
					update_entries(buffer_overlay, buffer);
					
				}
				
			} else if (mx < 120 && my > height - 40) {
				saved = TRUE;
				load_settings();
				update_entries(buffer_overlay, buffer);
				
			} else if (mx > width - 40 && my > height - 40) {
				if (!saved) {
					if (UI_dialog(439, _api_font_aqua_20px, translate("You have unsaved changes. Are you sure you want to quit?"), _api_icon_save, 0x00C40E6C, "TRUE", _api_icon_next, 0x00FFFF00, "FALSE")) {
						saved = TRUE;
						save_settings();
						
					}
					
				}
				
				apping = FALSE;
				break;
				
			} else {
				saved = FALSE;
				
				my -= 90;
				my /= 40;
				
				if (my == 0) {
					if (UI_dialog(439, _api_font_aqua_20px, translate("Are you sure you want to reformat the disk?"), _api_icon_back, 0x0000FF00, "FALSE", _api_icon_next, 0x00FFFF00, "TRUE")) {
						FS_reformat();
						
						UI_dialog(439, _api_font_aqua_20px, translate("Your disk has now been formatted. (Shutdown is recomended)"), _api_icon_off, 0x00FF0000, "poweroff", _api_icon_next, 0x00FFFF00, "NULL");
						UI_dialog(439, _api_font_aqua_20px, translate("Are you really sure? Disk may cause corruption."), _api_icon_off, 0x00FF0000, "poweroff", _api_icon_next, 0x00FFFF00, "NULL");
						
					}
					
				} else if (my == 1) {
					if (UI_dialog(439, _api_font_aqua_20px, translate("Are you sure you want to reset your settings?"), _api_icon_next, 0x00FFFF00, "TRUE", _api_icon_back, 0x0000FF00, "NULL")) {
						settings_reset();
						saved = FALSE;
						update_entries(buffer_overlay, buffer);
						
					}
					
				} else if (my == 2) {
					enter_start_wizard();
					update_entries(buffer_overlay, buffer);
					
				} else if (my == 3) {
					if (get_setting(0) == 'f') set_setting(0, 't');
					else if (get_setting(0) == 't') set_setting(0, 'f');
					
					update_entries(buffer_overlay, buffer);
					
				} else if (my == 4) {
					if (get_setting(1) == 'f') set_setting(1, 't');
					else if (get_setting(1) == 't') set_setting(1, 'f');
					
					update_entries(buffer_overlay, buffer);
					
				}
				
			}
			
		}
		
	}
	
	hide_cursor();
	UI_animate_desktop("fade", 30, buffer_trace);
	
	kfree(buffer, buffer_size);
	kfree(buffer_overlay, buffer_size);
	kfree(buffer_trace, buffer_size);
	
}
