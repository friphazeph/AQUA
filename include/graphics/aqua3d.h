/*
 * aqua3d.h
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


#ifndef AQUA3D_H
	#define AQUA3D_H
	
	#include "../types.h"
	#include "../utils/math.h"
	
	#define GL_POINT 1
	#define GL_EDGE 2
	#define GL_FACE 3
	
	#define GL_FOG 4
	#define GL_BLOOM 5
	#define GL_BILLBOARDS 6
	#define GL_DEPTH_TEST 7
	
	typedef uint32 gl_colour_t;
	typedef float gl_float_t;
	
	typedef struct {
		gl_float_t x;
		gl_float_t y;
		gl_float_t z;
		
	} gl_pos_t;

	typedef struct {
		gl_float_t x;
		gl_float_t y;
		
	} gl_2d_pos_t;

	typedef struct {
		gl_float_t x;
		gl_float_t y;
		
	} gl_rot_t;

	typedef struct {
		gl_pos_t pos;
		gl_rot_t rot;
		uint32 fov;
		
	} gl_camera_t;

	typedef struct {
		gl_pos_t pos;
		gl_colour_t colour;
		gl_float_t distance;
		
	} gl_point_t;
	
	typedef struct {
		gl_pos_t pos1;
		gl_pos_t pos2;
		gl_colour_t colour;
		
	} gl_edge_t;
	
	void gl_init(int bytes);
	void gl_destroy(void);
	
	gl_rot_t gl_rotate_2d(gl_2d_pos_t pos, gl_float_t rad);
	gl_float_t gl_get_distance(gl_pos_t pos1, gl_pos_t pos2);
	
	void gl_update_camera(gl_pos_t pos, gl_rot_t rot);
	void gl_fov(uint32 fov);
	
	gl_pos_t gl_pos(gl_float_t x, gl_float_t y, gl_float_t z);
	gl_rot_t gl_rot(gl_float_t x, gl_float_t y);
	gl_2d_pos_t gl_point_pos(int id);
	
	int gl_point(gl_pos_t pos, gl_colour_t colour);
	void gl_edit_point(int id, gl_pos_t pos, gl_colour_t colour);
	
	int gl_line(gl_pos_t pos1, gl_pos_t pos2, gl_colour_t colour);
	void gl_delete_lines(void);
	
	void gl_enable(int type);
	void gl_disable(int type);
	
	void gl_fog_start(gl_float_t start);
	void gl_fog_end(gl_float_t end);
	void gl_fog_colour(gl_colour_t colour);
	
	void gl_bloom_radius(uint32 radius);
	
	void gl_billboard_size(uint32 _size);
	void gl_billboard_alpha(uint8 alpha);
	
	uint8* gl_get_screen(void);
	void gl_set_screen(uint8*);
	
	void gl_clear_colour(gl_colour_t colour);
	void gl_render_alpha(void);
	void gl_bloom(void);
	void gl_render(void);
	void gl_display(void);
	
#endif
