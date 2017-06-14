/*
 * application_utils.c
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

#include "application_utils.h"

static boolean apping;

static string itoa_util_wtf(int val) {
	if (val == 0) {
		return "0";
		
	}
	
	string result;
	int len = 0;
	
	for (; val > 0; ++len) {
		result[len] = val % 10 + '0';
		val /= 10;
		
	}
	
	result[len] = 0;
	return result;
	
}

static void update_notifications(uint8* buffer, string notification_name, int notification_hour, int notification_minute) {
	
	
}

static void update_launch(void) {
	int minute = CMOS_get_time('m');
	int old_minute = minute;
	
	uint8* _api_icon_back = api_get_icon("back");
	uint8* _api_icon_next = api_get_icon("next");
	
	uint8** _api_font_aqua_50px = api_get_font("aqua", 50);
	uint8** _api_font_aqua_20px = api_get_font("aqua", 20);
	
	uint32 width = GFX_get_mode_info('w');
	uint32 height = GFX_get_mode_info('h');
	uint32 buffer_size = width * height * 3;
	
	uint8* buffer = (uint8*) kmalloc(buffer_size);
	uint8* buffer_trace = (uint8*) kmalloc(buffer_size);
	
	buffer_trace = GFX_compile_section_to_ptr8(buffer_trace, 0, 0, width, height);
	
	string time_string;
	string week_day;
	string day_string;
	
	time_string = format_time();
	day_string = itoa_util_wtf(CMOS_get_time('d'));
	
	if (day_string[str_len(day_string) - 1] == '1') day_string = str_append(day_string, translate("st of "));
	else if (day_string[str_len(day_string) - 1] == '2') day_string = str_append(day_string, translate("nd of "));
	else if (day_string[str_len(day_string) - 1] == '3') day_string = str_append(day_string, translate("rd of "));
	else day_string = str_append(day_string, translate("th of "));
	
	day_string = str_append(day_string, translate(get_month_name()));
	
	buffer = GFX_blit_image_to_ptr8(buffer, width, 0, 0, width, height, buffer_trace);
	
	int hour = CMOS_get_time('h');
	uint32 text_colour = 0x00FFFFFF;
	
	if (hour < 4 || hour > 22) {
		buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 0, 0, width, height, 0x00FFEEAA, 90);
		text_colour = 0x00292929;
		
	} else {
		buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 0, 0, width, height, 0x00000000, 120);
		
	}
	
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, height - 30, 20, 20, _api_icon_back, 0x0000FF00);
	
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 20, 20, 0, 2681, _api_font_aqua_50px, time_string, text_colour);
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 20, 80, 0, 439, _api_font_aqua_20px, "Day of the week", text_colour);
	buffer = GFX_blit_text_hex_to_ptr8(buffer, width, 20, 110, 0, 439, _api_font_aqua_20px, day_string, text_colour);
	
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, width - 21, 150, 2, 20, text_colour, 90);
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, width - 30, 159, 9, 2, text_colour, 90);
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, width - 19, 159, 9, 2, text_colour, 90);
	
	hide_cursor();
	if (apping) UI_animate_desktop("fade", 60, buffer);
	else UI_animate_desktop("fade", 30, buffer);
	show_cursor();
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint8 key;
	
	apping = TRUE;
	while (apping) {
		minute = CMOS_get_time('m');
		if (minute != old_minute);
		old_minute = minute;
		
		key = poll_key();
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (key == 1) {
			apping = FALSE;
			break;
			
		} if (mx > width - 40 && my > height - 40 && mr == 1) {
			apping = FALSE;
			break;
			
		}
		
		if (mr == 1 && mx > width - 40 && my > 140 && my < 180) {
			string notification_name = UI_entry(439, _api_font_aqua_20px, translate("Enter your notification:"), _api_icon_next, 0x00FFFF00);
			int notification_hour = atoi(UI_entry(439, _api_font_aqua_20px, translate("Enter the hour of your notification:"), _api_icon_next, 0x00FFFF00));
			int notification_minute = atoi(UI_entry(439, _api_font_aqua_20px, translate("Enter the minute of your notification:"), _api_icon_next, 0x00FFFF00));
			
			//update_notifications(notification_name, notification_hour, notification_minute);
			
		}
		
	}
	
	kfree(buffer, buffer_size);
	
	hide_cursor();
	if (apping) UI_animate_desktop("fade", 60, buffer_trace);
	else UI_animate_desktop("fade", 30, buffer_trace);
	
	kfree(buffer_trace, buffer_size);
	show_cursor();
	
}

void launch_application_utils(void) {
	apping = FALSE;
	
	boolean maining = TRUE;
	while (maining) {
		update_launch();
		if (!apping) break;
		
	}
	
}
