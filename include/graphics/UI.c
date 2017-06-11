/*
 * UI.c
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


#include "UI.h"

static uint32 width;
static uint32 height;

static uint8 bpp;
static uint8 cpc;

static uint16 fdl;
static uint8** font_data;

static uint32 UI_components = 0;

void UI_init(uint16 _fdl, uint8 _font_data[94][_fdl]) {
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
	bpp = GFX_get_mode_info('b');
	cpc = bpp / 8;
	
	fdl = _fdl;
	font_data = (uint8**) _font_data;
	
}
/*
UI_component_entry UI_new_component_entry(uint32 x, uint32 y, uint32 len, string content, boolean hidden) {
	UI_component_entry result;
	
	UI_components++;
	result.id = UI_components;
	
	result.x = x;
	result.y = y;
	
	result.len = len;
	result.content = content;
	result.hidden = hidden;
	
	return result;
	
}

UI_component_entry UI_update_component_entry(UI_component_entry entry) {
	_key = poll_key();
	char add = (char) _key + 2;
	
	entry.len++;
	entry.content[entry.len] = add;
	entry.content[entry.len + 1] = 0;
	
	return entry;
	
}

uint8* UI_get_component_entry_ptr8(UI_component_entry entry, uint8* buffer) {
	uint32 c;
	for (c = 0; c < entry.len, c++) {
		
		
	}
	
}*/

void UI_turn_off(void) {
	uint8* double_buffer_trace = (uint8*) kmalloc(width * height * 3);
	double_buffer_trace = GFX_compile_section_to_ptr8(double_buffer_trace, 0, 0, width, height);
	
	uint8* double_buffer = (uint8*) kmalloc(width * height * 3);
	
	uint8 a;
	for (a = 0; a < 255; a += 5) {
		double_buffer = GFX_blit_image_to_ptr8(double_buffer, width, 0, 0, width, height, double_buffer_trace);
		double_buffer = GFX_fill_rect_hex_to_ptr8_wa(double_buffer, width, 0, 0, width, height, 0x00000000, a);
		GFX_blit_image(0, 0, width, height, double_buffer);
		
	}
	
	kfree(double_buffer_trace, width * height * 3);
	kfree(double_buffer, width * height * 3);
	ACPI_poweroff();
	
}

string UI_entry(uint16 _fdl, uint8 _font_data[94][_fdl], string text, uint8* icon_data, uint32 icon_colour) {
	hide_cursor();
	uint32 dialog_buffer_size = width * DIALOG_HEIGHT * 3;
	
	uint8* dialog = (uint8*) kmalloc(dialog_buffer_size);
	uint8* dialog_trace = (uint8*) kmalloc(dialog_buffer_size);
	uint8* dialog_final = (uint8*) kmalloc(dialog_buffer_size);
	uint8* dialog_with_text = (uint8*) kmalloc(dialog_buffer_size);
	
	uint32 y = (height / 2) - (DIALOG_HEIGHT / 2);
	
	dialog_trace = GFX_compile_section_to_ptr8(dialog_trace, 0, y, width, DIALOG_HEIGHT);
	dialog = GFX_compile_section_to_ptr8(dialog, 0, y, width, DIALOG_HEIGHT);
	
	dialog = GFX_fill_rect_hex_to_ptr8_wa(dialog, width, 0, 0, width, DIALOG_HEIGHT, 0x00000000, DIALOG_ALPHA);
	dialog = GFX_blit_text_hex_to_ptr8(dialog, width, 10, 4, 0, fdl, _font_data, text, 0x00FFFFFF);
	
	dialog = GFX_blit_icon_hex_to_ptr8(dialog, width, width - 30, (DIALOG_HEIGHT / 2) + 10, 20, 20, icon_data, icon_colour);
	
	uint8 _key;
	string result = "";
	
	boolean shift = FALSE;
	int q = 0;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint16 a;
	for (a = 0; a < 255; a += ANIMATION_STEP) {
		dialog_final = GFX_blit_image_to_ptr8(dialog_final, width, 0, 0, width, DIALOG_HEIGHT, dialog);
		dialog_final = GFX_blit_image_to_ptr8_wa(dialog_final, width, 0, 0, width, DIALOG_HEIGHT, dialog_trace, 255 - a);
		GFX_blit_image(0, y, width, DIALOG_HEIGHT, dialog_final);
		
	}
	
	show_cursor();
	
	boolean dialoging = TRUE;
	while (dialoging) {
		_key = poll_key();
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (_key == 28 || mr == 1) {
			dialoging = FALSE;
			break;
			
		} else if (_key != 127) {
			if (_key == 42) {
				shift = TRUE;
				
			} else {
				if (_key == 14) {
					q--;
					if (q < 0) q = 0;
					result[q] = 0;
					
				} else {
					//GFX_wash_hex(0x00FF00FF);
					result[q] = get_letter_from_map(_key, shift);
					q++;
					result[q] = 0;
					
				}
				
				dialog_with_text = GFX_blit_image_to_ptr8(dialog_with_text, width, 0, 0, width, DIALOG_HEIGHT, dialog_final);
				dialog_with_text = GFX_blit_text_hex_to_ptr8(dialog_with_text, width, 10, (DIALOG_HEIGHT / 2) + 10, 0, fdl, _font_data, result, 0x00FFFFFF);
				GFX_blit_image(0, y, width, DIALOG_HEIGHT, dialog_with_text);
				
				shift = FALSE;
				
			}
			
		}
		
	}
	
	kfree(dialog_with_text, dialog_buffer_size);
	
	dialog = GFX_blit_text_hex_to_ptr8(dialog, width, 10, (DIALOG_HEIGHT / 2) + 10, 10, fdl, _font_data, result, 0x00FFFFFF);
	hide_cursor();
	
	for (a = 0; a < 255; a += ANIMATION_STEP) {
		dialog_final = GFX_blit_image_to_ptr8(dialog_final, width, 0, 0, width, DIALOG_HEIGHT, dialog);
		dialog_final = GFX_blit_image_to_ptr8_wa(dialog_final, width, 0, 0, width, DIALOG_HEIGHT, dialog_trace, a);
		GFX_blit_image(0, y, width, DIALOG_HEIGHT, dialog_final);
		
	}
	
	GFX_blit_image(0, y, width, DIALOG_HEIGHT, dialog_trace);
	show_cursor();
	
	return result;
	
}

boolean UI_dialog(uint16 _fdl, uint8 _font_data[94][_fdl], string text, uint8 icon_data0[], uint32 icon_colour0, string command0, uint8 icon_data1[], uint32 icon_colour1, string command1) {
	hide_cursor();
	uint32 dialog_buffer_size = DIALOG_MAX_WIDTH * DIALOG_HEIGHT * 3;
	
	uint8* dialog = (uint8*) kmalloc(dialog_buffer_size);
	uint8* dialog_trace = (uint8*) kmalloc(dialog_buffer_size);
	uint8* dialog_final = (uint8*) kmalloc(dialog_buffer_size);
	
	uint32 dw = DIALOG_WIDTH;
	uint32 tpl = GFX_get_text_potential_length(text, 0, _fdl, _font_data);
	
	if (tpl + 40 > dw) {
		dw = tpl + 40;
		
	}
	
	if (dw > DIALOG_MAX_WIDTH) {
		//strcpy(text, "...");
		dw = DIALOG_MAX_WIDTH;
		
	}
	
	uint32 x = (width / 2) - (dw / 2);
	uint32 rx = (DIALOG_MAX_WIDTH / 2) - (dw / 2);
	uint32 sx = (width / 2) - (DIALOG_MAX_WIDTH / 2);
	uint32 y = (height / 2) - (DIALOG_HEIGHT / 2);
	
	dialog_trace = GFX_compile_section_to_ptr8(dialog_trace, 0, y, DIALOG_MAX_WIDTH, DIALOG_HEIGHT);
	dialog = GFX_compile_section_to_ptr8(dialog, 0, y, DIALOG_MAX_WIDTH, DIALOG_HEIGHT);
	
	dialog = GFX_fill_rect_hex_to_ptr8_wa(dialog, DIALOG_MAX_WIDTH, rx, 0, dw, DIALOG_HEIGHT, 0x00000000, DIALOG_ALPHA);
	dialog = GFX_blit_text_hex_to_ptr8(dialog, DIALOG_MAX_WIDTH, (DIALOG_MAX_WIDTH / 2) - (tpl / 2), 4, 0, fdl, _font_data, text, 0x00FFFFFF);
	
	dialog = GFX_blit_icon_hex_to_ptr8(dialog, DIALOG_MAX_WIDTH, (DIALOG_MAX_WIDTH / 2) - 120, (DIALOG_HEIGHT / 2) + 10, 20, 20, icon_data0, icon_colour0);
	dialog = GFX_blit_icon_hex_to_ptr8(dialog, DIALOG_MAX_WIDTH, (DIALOG_MAX_WIDTH / 2) + 100, (DIALOG_HEIGHT / 2) + 10, 20, 20, icon_data1, icon_colour1);
	
	uint16 _continue = 0;
	uint8 _key;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint16 a;
	for (a = 0; a < 255; a += ANIMATION_STEP) {
		_key = poll_key();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (_key == 28 || _key == 15 || _key == 1 || mr == 1) {
			if (mr == 1) _key = mr;
			_continue = a;
			break;
			
		}
		
		dialog_final = GFX_blit_image_to_ptr8(dialog_final, DIALOG_MAX_WIDTH, 0, 0, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog);
		dialog_final = GFX_blit_image_to_ptr8_wa(dialog_final, DIALOG_MAX_WIDTH, 0, 0, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog_trace, 255 - a);
		GFX_blit_image(sx, y, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog_final);
		
	}
	
	if (!_continue) {
		show_cursor();
		GFX_blit_image(0, y, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog);
		
		uint8 dialoging = TRUE;
		
		while (dialoging) {
			_key = poll_key();
			update_mouse_cursor();
			
			mx = get_mouse_x();
			my = get_mouse_y();
			
			mp = poll_mouse_press();
			mr = poll_mouse_release();
			
			if (_key == 28 || _key == 15 || _key == 1 || mr == 1) {
				if (mr == 1) {
					if (mx < (width / 2) - 80 && mx > (width / 2) - 120 && my > height / 2 && my < (height / 2) + 40) _key = 28;
					else if (mx > (width / 2) + 80 && mx < (width / 2) + 120 && my > height / 2 && my < (height / 2) + 40) _key = 15;
					else _key = 1;
					
				}
				
				dialoging = FALSE;
				break;
				
			}
			
		}
		
		hide_cursor();
		
	}
	
	for (a = _continue; a < 255; a += ANIMATION_STEP) {
		dialog_final = GFX_blit_image_to_ptr8(dialog_final, DIALOG_MAX_WIDTH, 0, 0, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog);
		dialog_final = GFX_blit_image_to_ptr8_wa(dialog_final, DIALOG_MAX_WIDTH, 0, 0, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog_trace, a);
		GFX_blit_image(sx, y, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog_final);
		
	}
	
	GFX_blit_image(0, y, DIALOG_MAX_WIDTH, DIALOG_HEIGHT, dialog_trace);
	
	boolean r = NULL;
	
	if (_key == 28) {
		if (str_eql(command0, "poweroff")) UI_turn_off();
		if (str_eql(command0, "TRUE")) r = TRUE;
		if (str_eql(command0, "FALSE")) r = FALSE;
		
	} else if (_key == 15) {
		if (str_eql(command1, "poweroff")) UI_turn_off();
		if (str_eql(command1, "TRUE")) r = TRUE;
		if (str_eql(command1, "FALSE")) r = FALSE;
		
	} 
	
	kfree(dialog, dialog_buffer_size);
	kfree(dialog_trace, dialog_buffer_size);
	kfree(dialog_final, dialog_buffer_size);
	
	show_cursor();
	return r;
	
}

void UI_animate_desktop(string type, uint16 time, uint8* image_data) {
	if (str_eql(type, "slide_up")) {
		float s = 0;
		
		uint f;
		for (f = 0; f < 70 * time; f++) {
			s += (s / 10.0f) + 0.2f;
			GFX_blit_image(0, (uint) (((float) height / (s + 1.0f))), width, height, image_data);
			
		}
		
		GFX_blit_image(0, 0, width, height, image_data);
		
	} else if (str_eql(type, "slide_down")) {
		float s = 0;
		
		uint f;
		for (f = 0; f < 70 * time; f++) {
			s += (s / 10.0f) + 0.2f;
			GFX_blit_image(0, (height - ((int) ((float) height / (s + 1.0f))) - height), width, height, image_data);
			
		}
		
		GFX_blit_image(0, 0, width, height, image_data);
		
	} else if (str_eql(type, "reveal")) {
		float s = 0;
		
		uint32 x;
		uint32 y;
		
		uint32 w;
		uint32 h;
		
		uint f;
		for (f = 0; f < 70 * time; f++) {
			s += (s / 10.0f) + 0.2f;
			
			x = width / 4 / s;
			y = height / 2 / s;
			
			w = width / 2 - x;
			h = height - y;
			
			GFX_blit_image_part(x, y, 0, 0, w * 2, h, width, height, image_data);
			sleep(1);
			
		}
		
	} else if (str_eql(type, "fade")) {
		uint8* old = (uint8*) kmalloc(width * height * 3);
		uint8* blitter = (uint8*) kmalloc(width * height * 3);
		
		old = GFX_compile_section_to_ptr8(old, 0, 0, width, height);
		
		uint16 a;
		for (a = 0; a < 255; a += time) {
			blitter = GFX_blit_image_to_ptr8(blitter, width, 0, 0, width, height, old);
			blitter = GFX_blit_image_to_ptr8_wa(blitter, width, 0, 0, width, height, image_data, (uint8) a);
			
			GFX_blit_image(0, 0, width, height, blitter);
			
		}
		
		GFX_blit_image(0, 0, width, height, image_data);
		
		kfree(old, width * height * 3);
		kfree(blitter, width * height * 3);
		
	}
	
}

void UI_animate_card(string type, uint16 time, uint8* card_data, uint32 _width, uint32 _height) {
	hide_cursor();
	
	if (str_eql(type, "slide_up")) {
		float s = 0;
		uint32 x = (width / 2) - (_width / 2);
		
		uint f;
		for (f = 0; f < 70 * time; f++) {
			s += ((s / 10.0f) + 0.2f) / time;
			GFX_blit_image(x, height - (uint) ((float) _height - ((float) _height / (s + 1.0f))), _width, _height, card_data);
			
		}
		
		GFX_blit_image(x, height - _height, _width, _height, card_data);
		
	}
	
	show_cursor();
	
}

static uint8* UI_blit_slide_down_to_ptr8(uint8* surf, uint16 _fdl, uint8 _font_data[94][_fdl], uint8 entry_count, tab_entry entries[entry_count], uint32 w, uint32 h, float s, boolean after) {
	uint32 y;
	
	if (after) y = (int) ((float) h / (s + 1.0f));
	else y = h - ((int) ((float) h / (s + 1.0f)));
	surf = GFX_fill_rect_hex_to_ptr8_wa(surf, w, 0, 0, w, y, 0x00000000, DIALOG_ALPHA);
	
	int e;
	for (e = 0; e < entry_count; e++) {
		surf = GFX_blit_text_hex_to_ptr8(surf, w, 5, (20 * e) + y - h, 0, _fdl, _font_data, translate(entries[e].name), entries[e].colour);
		//surf = GFX_blit_text_hex_to_ptr8(surf, w, 5, (20 * e), 0, _fdl, _font_data, translate(entries[e].name), entries[e].colour);
		
	}
	
	return surf;
	
}

void UI_slide_down(uint16 _fdl, uint8 _font_data[94][_fdl], uint8 tab, uint8 entry_count, tab_entry entries[entry_count]) {
	hide_cursor();
	float time = 5.0f / entry_count;
	
	uint32 w = (width - 20) / 4;
	uint32 h = entry_count * 20;
	uint32 x = 10 + (w * tab);
	uint32 slide_down_buffer_size = w * h * 3;
	
	//uint8 slide_down = (uint8*) kmalloc(w * h * 3);
	uint8* slide_down_trace = (uint8*) kmalloc(slide_down_buffer_size);
	uint8* slide_down_final = (uint8*) kmalloc(slide_down_buffer_size);
	
	slide_down_trace = GFX_compile_section_to_ptr8(slide_down_trace, x, 31, w, h);
	
	float s = 0;
	float a = 255;
	
	uint f;
	for (f = 0; f < 70 * time; f++) {
		s += ((s / 10.0f) + 0.2f) / time;
		a -= 255.0f / (float) (70 * time);
		
		slide_down_final = GFX_blit_image_to_ptr8(slide_down_final, w, 0, 0, w, h, slide_down_trace);
		slide_down_final = UI_blit_slide_down_to_ptr8(slide_down_final, _fdl, _font_data, entry_count, entries, w, h, s, FALSE);
		slide_down_final = GFX_blit_image_to_ptr8_wa(slide_down_final, w, 0, 0, w, h, slide_down_trace, (uint8) a);
		
		GFX_blit_image(x, 31, w, h, slide_down_final);
				
	}
	
	show_cursor();
	boolean sliding = TRUE;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	while (sliding) {
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (mr == 1) {
			hide_cursor();
			
			if (mx > x && mx < x + w && my > 30 && my < h + 31) {
				launch_application(entries[(my - 31) / 20].name);
				
			}
			
			sliding = FALSE;
			break;
			
		}
		
	}
	
	s = 0;
	for (f = 0; f < 70 * time; f++) {
		s += ((s / 10.0f) + 0.2f) / time;
		a += 255.0f / (float) (70 * time);
		
		if (a > 255) {
			a = 255;
			
		}
		
		slide_down_final = GFX_blit_image_to_ptr8(slide_down_final, w, 0, 0, w, h, slide_down_trace);
		slide_down_final = UI_blit_slide_down_to_ptr8(slide_down_final, _fdl, _font_data, entry_count, entries, w, h, s, TRUE);
		slide_down_final = GFX_blit_image_to_ptr8_wa(slide_down_final, w, 0, 0, w, h, slide_down_trace, (uint8) a);
		
		GFX_blit_image(x, 31, w, h, slide_down_final);
				
	}
	
	GFX_blit_image(x, 31, w, h, slide_down_trace);
	
	kfree(slide_down_final, slide_down_buffer_size);
	kfree(slide_down_trace, slide_down_buffer_size);
	
	show_cursor();
	
}
