/*
 * GFX.h
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


#ifndef GFX_H
	#define GFX_H
	
	#include "../types.h"
	#include "../utils/math.h"
	#include "../utils/utils.h"
	#include "../utils/string.h"
	#include "../system/system.h"
	#include "../user/cursor_task.h"
	
	#define INPUT_STATUS_1 0x03da
	#define VRETRACE 0x08
	
	void GFX_init(uint32 _width, uint32 _height, uint32 _bpp, uint64 _framebuffer_addr, boolean _buffer_blit);
	void GFX_update_framebuffer(void);
	uint8* GFX_get_video_RAM_ptr(void);
	uint32 GFX_compile_rgb_to_hex(uint8 r, uint8 g, uint8 b);
	
	void GFX_basic(void);
	
	void GFX_flip_buffer(uint8* buffer);
	uint32 GFX_get_mode_info(char info_byte);
	
	void GFX_set_filter(char r, char g, char b);
	
	void GFX_put_pixel_hex(uint32 x, uint32 y, uint32 colour);
	void GFX_put_pixel_rgb(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);
	
	uint32 GFX_get_pixel_hex(uint32 x, uint32 y);
	uint8 GFX_get_pixel_component(uint32 x, uint32 y, char c);
	
	void GFX_fill_rect_hex(uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour);
	void GFX_fill_rect_hex_wa(uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour, uint8 a);
	
	void GFX_draw_line(uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 colour);
	uint8* GFX_draw_line_to_ptr8(uint8* surf, uint32 sw, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 colour);
	
	void GFX_wash_hex(uint32 colour);
	void GFX_wash_hex_wa(uint32 colour, uint8 a);
	void GFX_wash_rgb(uint8 r, uint8 g, uint8 b);
	
	void GFX_blit_image(uint32 x, int y, uint32 w, uint32 h, uint8 image_data[]);
	void GFX_blit_image_part(uint32 x, uint32 y, uint32 part_x, uint32 part_y, uint32 part_w, uint32 part_h, uint32 w, uint32 h, uint8 image_data[]);
	void GFX_blit_image_wa(uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[], uint8 a);
	void GFX_blit_32bit_image(uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[]);
	
	void GFX_blit_icon_hex(uint32 x, uint32 y, uint8 w, uint8 h, uint8 icon_data[], uint32 colour);
	
	uint32 GFX_get_text_potential_length(string text, uint8 s, uint16 fdl, uint8 font_data[94][fdl]);
	void GFX_blit_text_hex(uint32 x, uint32 y, uint8 s, uint16 l, uint8 font_data[94][l], string text, uint32 colour);
	
	uint8* GFX_scale_ptr8(uint8* _ptr, uint32 w1, uint32 h1, uint32 w2, uint32 h2);
	
	uint8* GFX_copy_ptr8(uint8* surf, uint32 w, uint32 h);
	uint8* GFX_compile_section_to_ptr8(uint8* surf, uint32 x, uint32 y, uint32 w, uint32 h);
	uint8 GFX_get_ptr8_pixel_component(uint8* surf, uint32 w, uint32 x, uint32 y, char c);
	
	uint8* GFX_put_pixel_hex_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 colour);
	
	uint8* GFX_fill_rect_hex_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour);
	uint8* GFX_fill_rect_hex_to_ptr8_wa(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint32 colour, uint8 a);
	
	uint8* GFX_blit_image_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[]);
	uint8* GFX_blit_image_to_ptr8_wa(uint8* surf, uint32 sw, uint32 x, uint32 y, uint32 w, uint32 h, uint8 image_data[], uint8 alpha);
	
	uint8* GFX_blit_icon_hex_to_ptr8(uint8* surf, uint32 sw, uint32 x, uint32 y, uint8 w, uint8 h, uint8 icon_data[], uint32 colour);
	uint8* GFX_blit_text_hex_to_ptr8(uint8* surf, uint32 sw, int32 x, int32 y, uint8 s, uint16 l, uint8 font_data[94][l], string text, uint32 colour);
	
#endif
