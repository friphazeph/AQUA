/*
 * application_lasergame.c
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

#include "application_lasergame.h"

void launch_application_lasergame(void) {
	uint8* _api_icon_back = api_get_icon("back");
	uint8** _api_font_aqua_50px = api_get_font("aqua", 50);
	
	uint32 WIDTH = GFX_get_mode_info('w');
	uint32 HEIGHT = GFX_get_mode_info('h');
	
	uint8* old = (uint8*) kmalloc(WIDTH * HEIGHT * 3);
	old = GFX_compile_section_to_ptr8(old, 0, 0, WIDTH, HEIGHT);
	
	uint8 cr = 0;
	uint8 cg = 0;
	uint8 cb = 0;
	
	int g = rand_int(0, 2);
	int stars = rand_int(5000, 10000);
	
	gl_init(12 * stars + 128);
	gl_clear_colour(0x00000000);
	
	gl_enable(GL_BILLBOARDS);
	gl_enable(GL_DEPTH_TEST);
	
	gl_billboard_alpha(127);
	gl_billboard_size(300 / (800.0 / WIDTH));
	gl_bloom_radius(2);
	
	int s;
	for (s = 0; s < stars; s++) {
		cr = 0;
		cg = 0;
		cb = 0;
		
		if (g == 0) {
			cr = rand_int(0, 0xFF);
			cg = rand_int(0, cr);
			cb = rand_int(0, cg);
			
		} if (g == 1) {
			cg = rand_int(0, 0xFF);
			cb = rand_int(0, cg);
			cr = rand_int(0, cb);
			
		} if (g == 2) {
			cr = rand_int(0, 0xFF);
			cb = rand_int(0, cr);
			cg = rand_int(0, cb);
			
		}
		
		gl_point(gl_pos(rand_int(-40, 40), rand_int(-40, 40), rand_int(-40, 40)), GFX_compile_rgb_to_hex(cr, cg, cb));
		
	}
	
	gl_edit_point(0, gl_pos(0, 0, 0), rand_int(0, 0xFF00FF00));
	hide_cursor();
	
	char letter;
	char poll;
	
	float pos_x = 0;
	float pos_y = 0;
	float pos_z = -5;
	gl_rot_t rot = gl_rot(0, 0);
	
	gl_pos_t ai_pos;
	float ai_speed = 0.01;
	
	ai_pos.x = 0;
	ai_pos.y = 0;
	ai_pos.z = 0;
	
	float sx = 0;
	float sy = 0;
	float sz = 0;
	
	gl_2d_pos_t _2d_pos;
	gl_2d_pos_t _2d_ai;
	
	boolean lost = FALSE;
	
	while (TRUE) {
		/*update_mouse_cursor();
		rot.x = (get_mouse_y() - 300) / 2;
		rot.y = -(get_mouse_x() - 400) / 2;
		*/
		poll = poll_key();
		letter = get_letter_from_map(poll, FALSE);
		
		if (!lost) ai_speed += 0.001;
		
		if (letter == 'w') sz += 0.3;
		if (letter == 's') sz -= 0.3;
		if (letter == 'a') sx -= 0.3;
		if (letter == 'd') sx += 0.3;
		if (letter == 'u') sy -= 0.3;
		if (letter == 'j') sy += 0.3;
		
		if (letter == 'e') {
			gl_destroy();
			break;
			
		}
		
		if (letter == 'z') gl_enable(GL_BLOOM);
		if (letter == 'x') gl_disable(GL_BLOOM);
		if (letter == 'c') gl_billboard_alpha(127);
		if (letter == 'v') gl_billboard_alpha(255);
		
		pos_x += sx;
		pos_y += sy;
		pos_z += sz;
		
		if (pos_x > 40) pos_x = 40;
		if (pos_x < -40) pos_x = -40;
		if (pos_y > 40) pos_y = 40;
		if (pos_y < -40) pos_y = -40;
		if (pos_z > 40) pos_z = 40;
		if (pos_z < -40) pos_z = -40;
		
		sx /= 1.1;
		sy /= 1.1;
		sz /= 1.1;
		
		if (pos_x < ai_pos.x) ai_pos.x -= ai_speed;
		if (pos_x > ai_pos.x) ai_pos.x += ai_speed;
		if (pos_y < ai_pos.y) ai_pos.y -= ai_speed;
		if (pos_y > ai_pos.y) ai_pos.y += ai_speed;
		if (pos_z < ai_pos.z) ai_pos.z -= ai_speed;
		if (pos_z > ai_pos.z) ai_pos.z += ai_speed;
		
		if (pos_x > ai_pos.x - ai_speed && pos_y > ai_pos.y - ai_speed && pos_z > ai_pos.z - ai_speed &&
			pos_x < ai_pos.x + ai_speed && pos_y < ai_pos.y + ai_speed && pos_z < ai_pos.z + ai_speed) {
			if (g == 0) gl_clear_colour(0x00FF7700);
			if (g == 1) gl_clear_colour(0x0000FF77);
			if (g == 2) gl_clear_colour(0x00FF0077);
			
			lost = TRUE;
			
		}
		
		gl_update_camera(gl_pos(pos_x, pos_y, pos_z), rot);
		
		if (g == 0) gl_edit_point(stars - 1, ai_pos, 0x000077FF);
		if (g == 1) gl_edit_point(stars - 1, ai_pos, 0x00FF0077);
		if (g == 2) gl_edit_point(stars - 1, ai_pos, 0x0000FF77);
		
		_2d_pos = gl_point_pos(0);
		_2d_ai = gl_point_pos(stars - 1);
		
		gl_render();
		
		if (lost) {
			GFX_blit_text_hex_to_ptr8(gl_get_screen(), WIDTH, 50, 100, 0, 2681, _api_font_aqua_50px, "You lose. Your score was", 0x00FFFFFF);
			GFX_blit_text_hex_to_ptr8(gl_get_screen(), WIDTH, 100, 200, 0, 2681, _api_font_aqua_50px, itoa((int) (ai_speed * 100)), 0x00FFFFFF);
			
		}
		
		//if (g == 0) GFX_draw_line_to_ptr8(gl_get_screen(), WIDTH, (uint32) _2d_ai.x, (uint32) _2d_ai.y, (uint32) _2d_pos.x, (uint32) _2d_pos.y, 0x00FF7700);
		//if (g == 1) GFX_draw_line_to_ptr8(gl_get_screen(), WIDTH, (uint32) _2d_ai.x, (uint32) _2d_ai.y, (uint32) _2d_pos.x, (uint32) _2d_pos.y, 0x0000FF77);
		//if (g == 2) GFX_draw_line_to_ptr8(gl_get_screen(), WIDTH, (uint32) _2d_ai.x, (uint32) _2d_ai.y, (uint32) _2d_pos.x, (uint32) _2d_pos.y, 0x00FF0077);
		
		gl_bloom();
		
		if (lost) {
			GFX_blit_text_hex_to_ptr8(gl_get_screen(), WIDTH, 50, 100, 0, 2681, _api_font_aqua_50px, "You lose. Your score was", 0x00FFFFFF);
			GFX_blit_text_hex_to_ptr8(gl_get_screen(), WIDTH, 100, 200, 0, 2681, _api_font_aqua_50px, itoa((int) (ai_speed * 1000)), 0x00FFFFFF);
			
		}
		
		gl_display();
		
	}
	
	GFX_blit_image(0, 0, WIDTH, HEIGHT, old);
	kfree(old, WIDTH * HEIGHT * 3);
	
}
