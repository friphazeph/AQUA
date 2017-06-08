/*
 * start_wizard.c
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

#include "start_wizard.h"

static uint8* wallpaper;
static uint8* icon_next;

static uint8** font_aqua_20px;

static uint32 width;
static uint32 height;

void init_start_wizard(
	uint8* _wallpaper, \
	uint8* _icon_next, \
	\
	uint8** _font_aqua_20px \
	\
) {
	wallpaper = _wallpaper;
	icon_next = _icon_next;
	
	font_aqua_20px = _font_aqua_20px;
	
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
}

static string set_page(int page, string animation, boolean next) {
	uint8* wizard_buffer = (uint8*) kmalloc(width * height * 3);
	wizard_buffer = GFX_blit_image_to_ptr8(wizard_buffer, width, 0, 0, width, height, wallpaper);
	
	if (next) wizard_buffer = GFX_blit_icon_hex_to_ptr8(wizard_buffer, width, width - 30, height - 30, 20, 20, icon_next, 0x00FFFF00);
	else wizard_buffer = GFX_blit_icon_hex_to_ptr8(wizard_buffer, width, width - 30, height - 30, 20, 20, icon_next, 0x00292929);
	
	if (page == 0) {
		wizard_buffer = GFX_blit_image_to_ptr8(wizard_buffer, width, width / 3 - 20, 50, 40, 20, flag_en_uk);
		wizard_buffer = GFX_blit_image_to_ptr8(wizard_buffer, width, width / 1.5 - 20, 50, 40, 20, flag_fr_be);
		wizard_buffer = GFX_blit_image_to_ptr8(wizard_buffer, width, width / 3 - 20, 100, 40, 20, flag_hu);
		wizard_buffer = GFX_blit_image_to_ptr8(wizard_buffer, width, width / 1.5 - 20, 100, 40, 20, flag_ge);
		
	}
	
	hide_cursor();
	if (str_eql(animation, "fade")) UI_animate_desktop(animation, 30, wizard_buffer);
	else UI_animate_desktop(animation, 1, wizard_buffer);
	show_cursor();
	
	string result = "NULL";
	boolean b = FALSE;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint8 key;
		
	boolean paging = TRUE;
	while (paging) {
		key = poll_key();
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (!next && mr == 1 && page == 0) {
			if (my > 40 && my < 80) {
				if (mx > width / 3 - 30 && mx < width / 3 + 30) { strcpy(result, "en-uk"); b = TRUE; }
				else if (mx > width / 1.5 - 30 && mx < width / 1.5 + 30) { strcpy(result, "fr-be"); b = TRUE; }
				
			} else if (my > 90 && my < 130) {
				if (mx > width / 3 - 30 && mx < width / 3 + 30) { strcpy(result, "hu"); b = TRUE; }
				else if (mx > width / 1.5 - 30 && mx < width / 1.5 + 30) { strcpy(result, "ge"); b = TRUE; }
				
			}
			
		}
		
		if (b || (((mr == 1 && mx > width - 40 && my > height - 40) || key == 28) && next)) {
			break;
			
		}
		
	}
	
	kfree(wizard_buffer, width * height * 3);
	return result;
	
}

void enter_start_wizard(void) {
	string lang = set_page(0, "slide_up", FALSE);
	lang = set_page(0, "fade", TRUE);
	
	set_lang(lang);
	poll_key();
	string name = UI_entry(439, font_aqua_20px, translate("Hello! What is your name?"), icon_next, 0x00FFFF00);
	
	FS_reformat();
	FS_set_name(name);
	
}
