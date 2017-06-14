/*
 * widget_page.c
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

#include "widget_page.h"

static uint8* icon_off;
static uint8* icon_back;
static uint8* icon_edit;

static uint32 buffer_size;
static uint8* wallpaper;

static uint8** font_aqua_20px;
static uint8** font_aqua_50px;

static uint32 width;
static uint32 height;

static uint8* buffer;
static uint8* desktop;

void init_widget_page( \
	uint32 _width, \
	uint32 _height, \
	\
	uint8* _wallpaper, \
	\
	uint8* _icon_off, \
	uint8* _icon_back, \
	uint8* _icon_edit, \
	\
	uint8** _font_aqua_20px, \
	uint8** _font_aqua_50px \
	\
) {
	width = _width;
	height = _height;
	
	buffer_size = width * height * 3;
	wallpaper = _wallpaper;
	
	icon_off = _icon_off;
	icon_back = _icon_back;
	icon_edit = _icon_edit;
	
	font_aqua_20px = _font_aqua_20px;
	font_aqua_50px = _font_aqua_50px;
	
}

void widget_set_desktop(void) {
	desktop = (uint8*) kmalloc(buffer_size);
	desktop = GFX_compile_section_to_ptr8(desktop, 0, 0, width, height);
	
}

static void render_widget_page(void) {
	buffer = (uint8*) kmalloc(buffer_size);
	buffer = GFX_blit_image_to_ptr8(buffer, width, 0, 0, width, height, wallpaper);
	
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, 10, height - 30, 20, 20, icon_off, 0x00FF0000);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, height - 30, 20, 20, icon_back, 0x0000FF00);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, 10, 20, 20, icon_edit, 0x00E0861B);
	
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 10, 10, 0, 2681, font_aqua_50px, format_time(), 0x00FFFFFF);
	
	hide_cursor();
	UI_animate_desktop("fade", 30, buffer);
	show_cursor();
	
	kfree(buffer, buffer_size);
	
}

void enter_widget_page(void) {
	render_widget_page();
	
	int old_minute = CMOS_get_time('m');
	int minute_update;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint8 key;
	
	boolean widgeting = TRUE;
	while (widgeting) {
		minute_update = CMOS_get_time('m');
		if (minute_update != old_minute) render_widget_page();
		old_minute = minute_update;
		
		key = poll_key();
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (mx < 40 && my > height - 40 && mr == 1) {
			UI_dialog(439, font_aqua_20px, translate("Are you sure you want to shutdown the computer?"), icon_off, 0x00FF0000, "poweroff", icon_back, 0x0000FF00, "NULL");
			
		} else if ((mx > width - 40 && my > height - 40 && mr == 1) || key == 1) {
			widgeting = FALSE;
			break;
			
		}
		
	}
	
	hide_cursor();
	UI_animate_desktop("fade", 30, desktop);
	show_cursor();
	
}
