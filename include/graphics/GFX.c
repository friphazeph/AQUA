/*
 * GFX.c
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


#include "GFX.h"

static uint64 framebuffer_addr;
static uint8* video_RAM;
static uint8* screen;

static uint32 mode_pitch;
static uint32 width;
static uint32 height;

static uint8 bpp;
static uint8 cpc;

void GFX_init(uint32 _width, uint32 _height, uint32 _bpp, uint64 _framebuffer_addr, boolean _buffer_blit) {
	cpc = _bpp / 8;
	mode_pitch = cpc * _width;
	
	width = _width;
	height = _height;
	bpp = _bpp;
	
	framebuffer_addr = _framebuffer_addr;
	screen = (uint8*) _framebuffer_addr;
	
	if (_buffer_blit) video_RAM = (uint8*) kmalloc(width * height * cpc);
	else video_RAM = screen;
	
}

void GFX_update_framebuffer(void) {
	int i;
	for (i = 0; i < width * height * cpc; i++) {
		screen[i] = video_RAM[i];
		
	}
	
}

uint8* GFX_get_video_RAM_ptr(void) {
	return video_RAM;
	
}

uint32 GFX_compile_rgb_to_hex(uint8 r, uint8 g, uint8 b) {
	uint8 a = 0;
	return (uint32) ((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	
}

void GFX_basic(void) {
	GFX_wash_hex(0x00000000);
	
}

void GFX_flip_buffer(uint8* buffer) {
	int b;
	for (b = 0; b < width * height * 4; b += 4) {
		video_RAM[b] = buffer[b];
		video_RAM[b + 1] = buffer[b + 1];
		video_RAM[b + 2] = buffer[b + 2];
		
	}
	
}

uint32 GFX_get_mode_info(char info_byte) {
	if (info_byte == 'w') return width;
	else if (info_byte == 'h') return height;
	else if (info_byte == 'b') return (uint32) bpp;
	else if (info_byte == 'c') return (uint32) cpc;
	else if (info_byte == 'p') return mode_pitch;
	else return 0;
	
}

void GFX_put_pixel_hex(uint32 x, uint32 y, uint32 colour) {
	uint32 where = x * cpc + y * mode_pitch;
	
	video_RAM[where] = colour & 255;
	video_RAM[where + 1] = (colour >> 8) & 255;
	video_RAM[where + 2] = (colour >> 16) & 255;
	
}

void GFX_put_pixel_rgb(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b) {
	uint32 where = x * cpc + y * mode_pitch;
	
	video_RAM[where] = r;
	video_RAM[where + 1] = g;
	video_RAM[where + 2] = b;
	
}

uint32 GFX_get_pixel_hex(uint32 x, uint32 y) {
	uint32 where = (width - x) * cpc + (height - y) * mode_pitch;
	uint32 result = 0x00000000;
	
	result += video_RAM[where];
	result += video_RAM[where + 1] << 8;
	result += video_RAM[where + 2] << 16;
	
	return result;
	
}

uint8 GFX_get_pixel_component(uint32 x, uint32 y, char c) {
	uint32 where = x * cpc + y * mode_pitch;
	
	if (c == 'r') return video_RAM[where];
	else if (c == 'g') return video_RAM[where + 1];
	else if (c == 'b') return video_RAM[where + 2];
	else return 0;
	
}

void GFX_draw_line(uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 colour) {
	int delta_x = x2 - x1;
	int delta_y = y2 - y1;
	int steps;
	
	if (abs(delta_x) > abs(delta_y)) steps = abs(delta_x);
	else steps = abs(delta_y);
	
	float x_inc = delta_x / (float) steps;
	float y_inc = delta_y / (float) steps;
	
	float x = x1;
	float y = y1;
	
	int v;
	for (v = 0; v < steps; v++) {
		x += x_inc;
		y += y_inc;
		
		if ((uint32) x > 0 && (uint32) x < width) GFX_put_pixel_hex((uint32) x, (uint32) y, colour);
		
	}
	
}

uint8* GFX_draw_line_to_ptr8(uint8* surf, uint32 sw, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 colour) {
	int delta_x = x2 - x1;
	int delta_y = y2 - y1;
	int steps;
	
	if (abs(delta_x) > abs(delta_y)) steps = abs(delta_x);
	else steps = abs(delta_y);
	
	float x_inc = delta_x / (float) steps;
	float y_inc = delta_y / (float) steps;
	
	float x = x1;
	float y = y1;
	
	int v;
	for (v = 0; v < steps; v++) {
		x += x_inc;
		y += y_inc;
		
		if (x < width && x > 0 && y < height && y > 0) surf = GFX_put_pixel_hex_to_ptr8(surf, sw, (uint32) x, (uint32) y, colour);
		
	}
	
	return surf;
	
}

void GFX_fill_rect_hex(uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour) {
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			where[_y * cpc] = colour & 255;
			where[_y * cpc + 1] = (colour >> 8) & 255;
			where[_y * cpc + 2] = (colour >> 16) & 255;
			
		}
		
		where += mode_pitch;
		
	}
	
}

void GFX_fill_rect_hex_wa(uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour, uint8 a) {
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	int _r;
	int _g;
	int _b;
	
	uint8 r = colour & 255;
	uint8 g = (colour >> 8) & 255;
	uint8 b = (colour >> 16) & 255;
	
	float mc = (float) a / 255.0f;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			_r = mc * r + (1.0f - mc) * where[_y * cpc];
			_g = mc * g + (1.0f - mc) * where[_y * cpc + 1];
			_b = mc * b + (1.0f - mc) * where[_y * cpc + 2];
			
			if (_r < 0) _r = 0;
			if (_g < 0) _g = 0;
			if (_b < 0) _b = 0;
			
			if (_r > 255) _r = 255;
			if (_g > 255) _g = 255;
			if (_b > 255) _b = 255;
			
			where[_y * cpc] = (uint8) _r;
			where[_y * cpc + 1] = (uint8) _g;
			where[_y * cpc + 2] = (uint8) _b;
			
		}
		
		where += mode_pitch;
		
	}
	
}

void GFX_wash_hex(uint32 colour) {
	GFX_fill_rect_hex(0, 0, width, height, colour);
	
}

void GFX_wash_hex_wa(uint32 colour, uint8 a) {
	GFX_fill_rect_hex_wa(0, 0, width, height, colour, a);
	
}

void GFX_wash_rgb(uint8 r, uint8 g, uint8 b) {
	int p;
	for (p = 0; p < width * height * 4; p += 4) {
		video_RAM[p] = r;
		video_RAM[p + 1] = g;
		video_RAM[p + 2] = b;
		
	}
	
}

uint8* GFX_scale_image(uint8* surf, uint32 w, uint32 factor) {
	
}

void GFX_blit_image(uint32 x, int y, uint32 w, uint32 h, uint8 image_data[]) {
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	uint64 i = 0;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			if (_x + y >= 0 && _x + y < height) {
				where[_y * cpc] = image_data[i];
				where[_y * cpc + 1] = image_data[i + 1];
				where[_y * cpc + 2] = image_data[i + 2];
				
			}
			
			i += 3;
			
		}
		
		where += mode_pitch;
		
	}
	
}

void GFX_blit_image_part(uint32 x, uint32 y, uint32 part_x, uint32 part_y, uint32 part_w, uint32 part_h, uint32 w, uint32 h, uint8 image_data[]) {
	//x -= part_x;
	//y -= part_y;
	
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	uint32 i = 0;
	
	int _y;
	int _x;
	for (_y = 0; _y < h; _y++) {
		for (_x = x; _x < w + x; _x++) {
			if (_x <= part_w && _x >= part_x) {
				where[_x * cpc] = image_data[i];
				where[_x * cpc + 1] = image_data[i + 1];
				where[_x * cpc + 2] = image_data[i + 2];
				
			}
			
			i += 3;
			
		}
		
		where += mode_pitch;
		
	}
	
}

void GFX_blit_image_wa(uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[], uint8 a) {
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	int _r;
	int _g;
	int _b;
	
	int i = 0;
	float mc = (float) a / 255.0f;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			_r = mc * image_data[i] + (1.0f - mc) * where[y * cpc];
			_g = mc * image_data[i + 1] + (1.0f - mc) * where[y * cpc + 1];
			_b = mc * image_data[i + 2] + (1.0f - mc) * where[y * cpc + 2];
			
			if (_r < 0) _r = 0;
			if (_g < 0) _g = 0;
			if (_b < 0) _b = 0;
			
			if (_r > 255) _r = 255;
			if (_g > 255) _g = 255;
			if (_b > 255) _b = 255;
			
			where[_y * cpc] = image_data[i];
			where[_y * cpc + 1] = image_data[i + 1];
			where[_y * cpc + 2] = image_data[i + 2];
			
			i += 3;
			
		}
		
		where += mode_pitch;
		
	}
	
}

void GFX_blit_32bit_image(uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[]) {
	uint8* where = screen;
	where += mode_pitch * y;
	
	int i = 0;
	float mc;
	
	int _r;
	int _g;
	int _b;
	
	uint8 r;
	uint8 g;
	uint8 b;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			if (_y < GFX_get_mode_info('w')) {
				r = image_data[i];
				g = image_data[i + 1];
				b = image_data[i + 2];
				mc = (float) image_data[i + 3] / 255.0f;
				
				_r = mc * r + (1.0f - mc) * GFX_get_pixel_component(_y, _x + y, 'r');
				_g = mc * g + (1.0f - mc) * GFX_get_pixel_component(_y, _x + y, 'g');
				_b = mc * b + (1.0f - mc) * GFX_get_pixel_component(_y, _x + y, 'b');
				
				if (_r < 0) _r = 0;
				if (_g < 0) _g = 0;
				if (_b < 0) _b = 0;
				
				if (_r > 255) _r = 255;
				if (_g > 255) _g = 255;
				if (_b > 255) _b = 255;
				
				where[_y * cpc] = (uint8) _r;
				where[_y * cpc + 1] = (uint8) _g;
				where[_y * cpc + 2] = (uint8) _b;
				
				i += 4;
				
			}
			
		}
		
		where += mode_pitch;
		
	}
	
}

void GFX_blit_icon_hex(uint32 x, uint32 y, uint8 w, uint8 h, uint8 icon_data[], uint32 colour) {
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	int i = 0;
	float mc;
	
	int _r;
	int _g;
	int _b;
	
	uint8 r = colour & 255;
	uint8 g = (colour >> 8) & 255;
	uint8 b = (colour >> 16) & 255;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			mc = (float) icon_data[i] / 255.0f;
			
			_r = mc * r + (1.0f - mc) * GFX_get_pixel_component(_y, _x + y, 'r');
			_g = mc * g + (1.0f - mc) * GFX_get_pixel_component(_y, _x + y, 'g');
			_b = mc * b + (1.0f - mc) * GFX_get_pixel_component(_y, _x + y, 'b');
			
			if (_r < 0) _r = 0;
			if (_g < 0) _g = 0;
			if (_b < 0) _b = 0;
			
			if (_r > 255) _r = 255;
			if (_g > 255) _g = 255;
			if (_b > 255) _b = 255;
			
			where[_y * cpc] = (uint8) _r;
			where[_y * cpc + 1] = (uint8) _g;
			where[_y * cpc + 2] = (uint8) _b;
			
			i++;
			
		}
		
		where += mode_pitch;
		
	}
	
}

uint32 GFX_get_text_potential_length(string text, uint8 s, uint16 fdl, uint8 font_data[94][fdl]) {
	int x = 0;
	
	int c;
	for (c = 0; c < str_len(text); c++) {
		x += (s * (x / width)) + 1 + font_data[text[c] - 32][0];
		
	}
	
	return (uint32) x;
	
}

void GFX_blit_text_hex(uint32 x, uint32 y, uint8 s, uint16 l, uint8 font_data[94][l], string text, uint32 colour) {
	char _c;
	
	uint8* temp_icon_data;
	uint8 icon_data[l];
	
	int _s;
	
	int c;
	for (c = 0; c < str_len(text); c++) {
		_c = text[c] - 32;
		temp_icon_data = (uint8*) font_data[_c];
		
		for (_s = 0; _s < temp_icon_data[0] * temp_icon_data[1]; _s++) {
			icon_data[_s] = temp_icon_data[_s + 2];
			
		}
		
		GFX_blit_icon_hex(x, y, temp_icon_data[0], temp_icon_data[1], icon_data, colour);
		x += (s * (x / width)) + 1 + temp_icon_data[0];
		
	}
	
}

uint8* GFX_scale_ptr8(uint8* _ptr, uint32 w1, uint32 h1, uint32 w2, uint32 h2) {
	uint8* temp = kmalloc(w1 * h1 * 3);
	
	int x_ratio = (int) ((w1 << 16) / w2) + 1;
	int y_ratio = (int) ((h1 << 16) / h2) + 1;
	
	int x;
	int y;
	
	int i;
	int j;
	for (i = 0; i < h2; i++) {
		for (j = 0; j < w2; j++) {
			x = (j * x_ratio) >> 16;
			y = (i * y_ratio) >> 16;
			
			temp[((i * w2) + j) * 3] = _ptr[((y * w1) + x) * 3];
			temp[(((i * w2) + j) * 3) + 1] = _ptr[(((y * w1) + x) * 3) + 1];
			temp[(((i * w2) + j) * 3) + 2] = _ptr[(((y * w1) + x) * 3) + 2];
			
		}
		
	}
	
	return temp;
	
}

uint8* GFX_copy_ptr8(uint8* surf, uint32 w, uint32 h) {
	uint8* copy;
	
	int i;
	for (i = 0; i < w * h * 3; i += 3) {
		copy[i] = surf[i];
		copy[i + 1] = surf[i + 1];
		copy[i + 2] = surf[i + 2];
		
	} 
	
	return copy;
	
}

uint8* GFX_compile_section_to_ptr8(uint8* surf, uint32 x, uint32 y, uint32 w, uint32 h) {
	uint8* where = video_RAM;
	where += mode_pitch * y;
	
	int i = 0;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			surf[i] = where[_y * cpc];
			surf[i + 1] = where[_y * cpc + 1];
			surf[i + 2] = where[_y * cpc + 2];
			
			i += 3;
			
		}
		
		where += mode_pitch;
		
	}
	
	return surf;
	
}

uint8 GFX_get_ptr8_pixel_component(uint8* surf, uint32 w, uint32 x, uint32 y, char c) {
	if (c == 'r') return surf[x * 3 + y * (3 * w)];
	else if (c == 'g') return surf[x * 3 + y * (3 * w) + 1];
	else if (c == 'b') return surf[x * 3 + y * (3 * w) + 2];
	else return 0;
	
}

uint8* GFX_put_pixel_hex_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 colour) {
	uint32 where = x * 3 + y * (3 * sw);
	
	surf[where] = colour & 255;
	surf[where + 1] = (colour >> 8) & 255;
	surf[where + 2] = (colour >> 16) & 255;
	
	return surf;
	
}

uint8* GFX_fill_rect_hex_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour) {
	uint32 pitch = sw * 3;
	uint32 add_pitch = pitch * y;
	
	uint8 r = colour & 255;
	uint8 g = (colour >> 8) & 255;
	uint8 b = (colour >> 16) & 255;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			if (_x + y < height) {
				surf[_y * 3 + add_pitch] = r;
				surf[_y * 3 + 1 + add_pitch] = g;
				surf[_y * 3 + 2 + add_pitch] = b;
				
			}
			
		}
		
		add_pitch += pitch;
		
	}
	
	return surf;
	
}

uint8* GFX_fill_rect_hex_to_ptr8_wa(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour, uint8 a) {
	uint32 pitch = sw * 3;
	uint32 add_pitch = pitch * y;
	
	int _r;
	int _g;
	int _b;
	
	uint8 r = colour & 255;
	uint8 g = (colour >> 8) & 255;
	uint8 b = (colour >> 16) & 255;
	
	float mc = (float) a / 255.0f;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			if (_x + y < height) {
				_r = mc * r + (1.0f - mc) * surf[_y * 3 + add_pitch];
				_g = mc * g + (1.0f - mc) * surf[_y * 3 + 1 + add_pitch];
				_b = mc * b + (1.0f - mc) * surf[_y * 3 + 2 + add_pitch];
				
				if (_r < 0) _r = 0;
				if (_g < 0) _g = 0;
				if (_b < 0) _b = 0;
				
				if (_r > 255) _r = 255;
				if (_g > 255) _g = 255;
				if (_b > 255) _b = 255;
				
				surf[_y * 3 + add_pitch] = (uint8) _r;
				surf[_y * 3 + 1 + add_pitch] = (uint8) _g;
				surf[_y * 3 + 2 + add_pitch] = (uint8) _b;
				
			}
			
		}
		
		add_pitch += pitch;
		
	}
	
	return surf;
	
}

uint8* GFX_blit_image_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[]) {
	uint32 pitch = sw * 3;
	uint32 add_pitch = pitch * y;
	
	int i = 0;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			surf[_y * 3 + add_pitch] = image_data[i];
			surf[_y * 3 + 1 + add_pitch] = image_data[i + 1];
			surf[_y * 3 + 2 + add_pitch] = image_data[i + 2];
			
			i += 3;
			
		}
		
		add_pitch += pitch;
		
	}
	
	return surf;
	
}

uint8* GFX_blit_image_to_ptr8_wa(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[], uint8 a) {
	uint32 pitch = sw * 3;
	uint32 add_pitch = pitch * y;
	
	int _r;
	int _g;
	int _b;
	
	int i = 0;
	float mc = (float) a / 255.0f;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			_r = mc * image_data[i] + (1.0f - mc) * surf[_y * 3 + add_pitch];
			_g = mc * image_data[i + 1] + (1.0f - mc) * surf[_y * 3 + 1 + add_pitch];
			_b = mc * image_data[i + 2] + (1.0f - mc) * surf[_y * 3 + 2 + add_pitch];
			
			if (_r < 0) _r = 0;
			if (_g < 0) _g = 0;
			if (_b < 0) _b = 0;
			
			if (_r > 255) _r = 255;
			if (_g > 255) _g = 255;
			if (_b > 255) _b = 255;
			
			surf[_y * 3 + add_pitch] = (uint8) _r;
			surf[_y * 3 + 1 + add_pitch] = (uint8) _g;
			surf[_y * 3 + 2 + add_pitch] = (uint8) _b;
			
			i += 3;
			
		}
		
		add_pitch += pitch;
		
	}
	
	return surf;
	
}

uint8* GFX_blit_icon_hex_to_ptr8(uint8* surf, uint32 sw, uint32 x, int y, uint8 w, uint8 h, uint8 icon_data[], uint32 colour) {
	uint32 pitch = sw * 3;
	uint32 add_pitch = pitch * y;
	
	int i = 0;
	float mc;
	
	int _r;
	int _g;
	int _b;
	
	uint8 r = colour & 255;
	uint8 g = (colour >> 8) & 255;
	uint8 b = (colour >> 16) & 255;
	
	int _x;
	int _y;
	for (_x = 0; _x < h; _x++) {
		for (_y = x; _y < w + x; _y++) {
			if (_x + y > 0) {
				mc = (float) icon_data[i] / 255.0f;
				
				_r = mc * r + (1.0f - mc) * GFX_get_ptr8_pixel_component(surf, sw, _y, _x + y, 'r');
				_g = mc * g + (1.0f - mc) * GFX_get_ptr8_pixel_component(surf, sw, _y, _x + y, 'g');
				_b = mc * b + (1.0f - mc) * GFX_get_ptr8_pixel_component(surf, sw, _y, _x + y, 'b');
				
				if (_r < 0) _r = 0;
				if (_g < 0) _g = 0;
				if (_b < 0) _b = 0;
				
				if (_r > 255) _r = 255;
				if (_g > 255) _g = 255;
				if (_b > 255) _b = 255;
				
				surf[_y * 3 + add_pitch] = (uint8) _r;
				surf[_y * 3 + 1 + add_pitch] = (uint8) _g;
				surf[_y * 3 + 2 + add_pitch] = (uint8) _b;
				
				i++;
				
			}
			
		}
		
		add_pitch += pitch;
		
	}
	
	return surf;
	
}

static uint8* GFX_reverse_icon(uint8* icon_data, boolean vertical, uint32 w, uint32 h) {
	uint8 new_icon_data[w * h];
	int _w = 0;
	int l = 0;
	
	int i;
	for (i = 0; i < w * h; i++) {
		if (!(i % w)) {
			_w = 0;
			l++;
			
		}
		
		new_icon_data[i] = icon_data[l * w - _w];
		_w++;
		
	}
	
	for (i = 0; i < w * h; i++) {
		icon_data[i] = new_icon_data[i];
		
	}
	
	return icon_data;
	
}

uint8* GFX_blit_text_hex_to_ptr8(uint8* surf, uint32 sw, int32 x, int32 y, uint8 s, uint16 l, uint8 font_data[94][l], string text, uint32 colour) {
	char _c;
	
	uint8* temp_icon_data;
	uint8* temp_accent_data;
	
	uint8 icon_data[l];
	uint8 accent_data[l];
	
	boolean stop = FALSE;
	boolean reverse = FALSE;
	boolean _double = FALSE;
	int slide;
	
	int _s;
	
	int c;
	for (c = 0; c < str_len(text); c++) {
		_c = text[c] - 32;
		
		temp_icon_data = (uint8*) font_data[_c];
		
		for (_s = 0; _s < temp_icon_data[0] * temp_icon_data[1]; _s++) {
			icon_data[_s] = temp_icon_data[_s + 2];
			
		}
		
		surf = GFX_blit_icon_hex_to_ptr8(surf, sw, x, y, temp_icon_data[0], temp_icon_data[1], icon_data, colour);
		
		if (text[c + 1] == '`') {
			reverse = FALSE;
			_double = FALSE;
			stop = FALSE;
			
			if (text[c + 2] == 'g') { temp_accent_data = (uint8*) font_data['`' - 32]; slide = 0; }
			else if (text[c + 2] == 'a') { temp_accent_data = (uint8*) font_data['`' - 32]; slide = 0; reverse = TRUE; }
			else if (text[c + 2] == '^') { temp_accent_data = (uint8*) font_data['^' - 32]; slide = temp_icon_data[1] / (20.0f / 4); }
			else if (text[c + 2] == '~' || text[c + 2] == 'v') { temp_accent_data = (uint8*) font_data['-' - 32]; slide = temp_icon_data[1] / (20.0f / 8); }
			else if (text[c + 2] == ':' || text[c + 2] == 't') { temp_accent_data = (uint8*) font_data['.' - 32]; slide = temp_icon_data[1] / (20.0f / 12); _double = TRUE; }
			else stop = TRUE;
			
			if (!stop) {
				for (_s = 0; _s < temp_accent_data[0] * temp_accent_data[1]; _s++) {
					accent_data[_s] = temp_accent_data[_s + 2];
					
				}
				
				if (reverse) {
					GFX_reverse_icon(accent_data, FALSE, temp_accent_data[0], temp_accent_data[1]);
					
				}
				
				if (_double) {
					surf = GFX_blit_icon_hex_to_ptr8(surf, sw, x - (temp_accent_data[0] / 2) + (temp_icon_data[0] / 2) - 3, y - slide, temp_accent_data[0], temp_accent_data[1], accent_data, colour);
					surf = GFX_blit_icon_hex_to_ptr8(surf, sw, x - (temp_accent_data[0] / 2) + (temp_icon_data[0] / 2) + 2, y - slide, temp_accent_data[0], temp_accent_data[1], accent_data, colour);
					
				} else {
					surf = GFX_blit_icon_hex_to_ptr8(surf, sw, x - (temp_accent_data[0] / 2) + (temp_icon_data[0] / 2), y - slide, temp_accent_data[0], temp_accent_data[1], accent_data, colour);
					
				}
				
				c += 2;
				
			} else c++;
			
		}
		
		x += (s * (x / width)) + 1 + temp_icon_data[0];
		
	}
	
	return surf;
	
}
