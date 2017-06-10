/*
 * aqua3d.c
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


#include "aqua3d.h"

static uint32 width;
static uint32 height;

static uint32 cx;
static uint32 cy;

static int allocated_bytes;

static gl_point_t* points;
static int point_count;

static gl_edge_t* edges;
static int edge_count;

static uint8* screen;
static gl_colour_t clear_colour;

static gl_camera_t camera;

static boolean render_points;
static boolean render_edges;
static boolean render_faces;

static boolean render_fog;
static boolean render_bloom;
static boolean render_billboards;

static boolean depth_test;

static gl_float_t fog_start;
static gl_float_t fog_end;
static uint32 fog_colour;

static uint32 bloom_radius;

static uint32 billboard_size;
static uint8 billboard_alpha;

void gl_init(int bytes) { // one point is 12 bytes
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
	cx = width / 2;
	cy = height / 2;
	
	allocated_bytes = bytes;
	//points = (gl_point_t*) kmalloc(bytes);
	
	point_count = 0;
	edge_count = 0;
	
	gl_update_camera(gl_pos(0, 0, -5), gl_rot(0, 0));
	gl_fov(cx);
	
	screen = (uint8*) kmalloc(width * height * 3);
	clear_colour = 0x00000000;
	
	render_points = TRUE;
	render_edges = TRUE;
	render_faces = TRUE;
	
	render_fog = FALSE;
	render_bloom = FALSE;
	render_billboards = FALSE;
	
	depth_test = FALSE;
	
	fog_start = 0;
	fog_end = 0;
	fog_colour = clear_colour;
	
	bloom_radius = 2;
	
	billboard_size = 300;
	billboard_alpha = 255;
	
}

void gl_destroy(void) {
	kfree(screen, width * height * 3);
	//kfree(points, allocated_bytes);
	
}

gl_rot_t gl_rotate_2d(gl_2d_pos_t pos, gl_float_t rad) {
	gl_float_t s = (gl_float_t) sin(rad);
	gl_float_t c = (gl_float_t) cos(rad);
	
	gl_rot_t rot;
	
	rot.x = pos.x * c - pos.y * s;
	rot.y = pos.y * c + pos.x * s;
	
	return rot;
	
}

gl_float_t gl_get_distance(gl_pos_t pos1, gl_pos_t pos2) {
	gl_float_t delta_x = pos2.x - pos1.x;
	gl_float_t delta_y = pos2.y - pos1.y;
	gl_float_t delta_z = pos2.z - pos1.z;
	
	return sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
	
}

void gl_update_camera(gl_pos_t pos, gl_rot_t rot) {
	camera.pos = pos;
	camera.rot = rot;
	
}

void gl_fov(uint32 fov) {
	camera.fov = fov;
	
}

gl_pos_t gl_pos(gl_float_t x, gl_float_t y, gl_float_t z) {
	gl_pos_t pos;
	
	pos.x = x;
	pos.y = y;
	pos.z = z;
	
	return pos;
	
}

gl_rot_t gl_rot(gl_float_t x, gl_float_t y) {
	gl_rot_t rot;
	
	rot.x = x;
	rot.y = y;
	
	return rot;
	
}

gl_2d_pos_t gl_point_pos(int id) {
	gl_2d_pos_t _2d_pos;
	gl_float_t f;
	
	gl_pos_t v;
	gl_2d_pos_t _2d;
	gl_rot_t r;
	
	v.x = points[id].pos.x;
	v.y = points[id].pos.y;
	v.z = points[id].pos.z;
	
	v.x -= camera.pos.x;
	v.y -= camera.pos.y;
	v.z -= camera.pos.z;
	
	_2d.x = v.x;
	_2d.y = v.z;
	r = gl_rotate_2d(_2d, camera.rot.y);
	
	v.x = r.x;
	v.z = r.y;
	
	_2d.x = v.y;
	_2d.y = v.z;
	r = gl_rotate_2d(_2d, camera.rot.x);
	
	v.y = r.x;
	v.z = r.y;
	
	f = camera.fov / v.z;
	
	v.x *= f;
	v.y *= f;
	
	_2d_pos.x = cx + (int) v.x;
	_2d_pos.y = cy + (int) v.y;
	
	return _2d_pos;
	
}

int gl_point(gl_pos_t pos, gl_colour_t colour) {
	gl_point_t point;
	
	point.pos = pos;
	point.colour = colour;
	
	points[point_count] = point;
	point_count++;
	
	return point_count - 1;
	
}

void gl_edit_point(int id, gl_pos_t pos, gl_colour_t colour) {
	points[id].pos = pos;
	points[id].colour = colour;
	
}

int gl_line(gl_pos_t pos1, gl_pos_t pos2, gl_colour_t colour) {
	gl_edge_t edge;
	
	edge.pos1 = pos1;
	edge.pos2 = pos2;
	edge.colour = colour;
	
	edges[edge_count] = edge;
	edge_count++;
	
	return edge_count - 1;
	
}

void gl_delete_lines(void) {
	edge_count = 0;
	
}

void gl_enable(int type) {
	if (type == GL_POINT) render_points = TRUE;
	if (type == GL_EDGE) render_edges = TRUE;
	if (type == GL_FACE) render_faces = TRUE;
	
	if (type == GL_FOG) render_fog = TRUE;
	if (type == GL_BLOOM) render_bloom = TRUE;
	if (type == GL_BILLBOARDS) render_billboards = TRUE;
	
	if (type == GL_DEPTH_TEST) depth_test = TRUE;
	
}

void gl_disable(int type) {
	if (type == GL_POINT) render_points = FALSE;
	if (type == GL_EDGE) render_edges = FALSE;
	if (type == GL_FACE) render_faces = FALSE;
	
	if (type == GL_FOG) render_fog = FALSE;
	if (type == GL_BLOOM) render_bloom = FALSE;
	if (type == GL_BILLBOARDS) render_billboards = FALSE;
	
	if (type == GL_DEPTH_TEST) depth_test = FALSE;
	
}

void gl_fog_start(gl_float_t start) { fog_start = start; }
void gl_fog_end(gl_float_t end) { fog_end = end; }
void gl_fog_colour(gl_colour_t colour) { fog_colour = colour; }

void gl_bloom_radius(uint32 radius) { bloom_radius = radius; }

void gl_billboard_size(uint32 _size) { billboard_size = _size; }
void gl_billboard_alpha(uint8 alpha) { billboard_alpha = alpha; }

uint8* gl_get_screen(void) {
	return screen;
	
}

void gl_set_screen(uint8* _screen) {
	screen = _screen;
	
}

void gl_clear_colour(gl_colour_t colour) {
	clear_colour = colour;
	
}

gl_point_t* gl_bubble_swap(gl_point_t* list, int index1, int index2) {
	gl_point_t temp = list[index1];
	list[index1] = list[index2];
	list[index2] = temp;
	
	return list;
	
}

gl_point_t* gl_bubble_sort(int len, gl_point_t* list) {
	int c;
	int d;
	
	for (c = 0; c < (len - 1); c++) {
		for (d = 0; d < len - c - 1; d++) {
			if (list[d].distance > list[d + 1].distance) {
				gl_bubble_swap(list, d, d + 1);
				
			}
			
		}
		
	}
	
	return list;
	
}


void gl_render_alpha(void) {
	gl_pos_t v;
	gl_2d_pos_t _2d;
	gl_rot_t r;
	
	gl_float_t f;
	
	uint32 fx;
	uint32 fy;
	
	uint32 s;
	
	//gl_point_t* new_points = points;
	
	int p;
	/*for (p = 0; p < point_count; p++) {
		points[p].distance = gl_get_distance(points[p].pos, camera.pos);
		
	}
	
	bubble_sort(point_count, points);*/
	
	/*if (depth_test) {
		for (p = 0; p < point_count; p++) {
			points[p].distance = gl_get_distance(points[p].pos, camera.pos);
			
		}
		
		bubble_sort(point_count, points);
		
	} else new_points = points;
	*/
	for (p = point_count; p > 0; p--) {
		v.x = points[p].pos.x;
		v.y = points[p].pos.y;
		v.z = points[p].pos.z;
		
		if (v.z > camera.pos.z) {
			v.x -= camera.pos.x;
			v.y -= camera.pos.y;
			v.z -= camera.pos.z;
			
			/*_2d.x = v.x;
			_2d.y = v.z;
			r = gl_rotate_2d(_2d, camera.rot.y);
			
			v.x = r.x;
			v.z = r.y;
			
			_2d.x = v.y;
			_2d.y = v.z;
			r = gl_rotate_2d(_2d, camera.rot.x);
			
			v.y = r.x;
			v.z = r.y;*/
			
			f = camera.fov / v.z;
			
			v.x *= f;
			v.y *= f;
			
			fx = cx + (int) v.x;
			fy = cy + (int) v.y;
			
			if (fx < width && fx > 0 && fy > 0 && fy < height) {
				if (render_billboards) {
					s = (uint32) (billboard_size * (1 / gl_get_distance(camera.pos, points[p].pos)));
					
					if (billboard_alpha == 255) screen = GFX_fill_rect_hex_to_ptr8(screen, width, fx - s / 2, fy - s / 2, s, s, points[p].colour);
					else screen = GFX_fill_rect_hex_to_ptr8_wa(screen, width, fx - s / 2, fy - s / 2, s, s, points[p].colour, billboard_alpha);
					
				} else GFX_put_pixel_hex_to_ptr8(screen, width, fx, fy, points[p].colour);
				
			}
			
		}
		
	}
	
	/*uint32 fx2;
	uint32 fy2;
	
	int e;
	for (e = 0; e < edge_count; e++) {
		for (p = 0; p < 2; p++) {
			if (p) {
				v.x = edges[e].pos1.x;
				v.y = edges[e].pos1.y;
				v.z = edges[e].pos1.z;
				
			} else {
				v.x = edges[e].pos2.x;
				v.y = edges[e].pos2.y;
				v.z = edges[e].pos2.z;
				
			}
			
			v.x -= camera.pos.x;
			v.y -= camera.pos.y;
			v.z -= camera.pos.z;
			
			_2d.x = v.x;
			_2d.y = v.z;
			r = gl_rotate_2d(_2d, camera.rot.y);
			
			v.x = r.x;
			v.z = r.y;
			
			_2d.x = v.y;
			r = gl_rotate_2d(_2d, camera.rot.x);
			
			v.y = r.x;
			v.z = r.y;
			
			f = camera.fov / v.z;
			
			v.x *= f;
			v.y *= f;
			
			if (p) {
				fx = cx + (int) v.x;
				fy = cy + (int) v.y;
				
			} else {
				fx2 = cx + (int) v.x;
				fy2 = cy + (int) v.y;
				
			}
			
		}
		
		screen = GFX_draw_line_to_ptr8(screen, width, fx, fy, fx2, fy2, edges[e].colour);
		
	}*/
	
}

void gl_bloom(void) {
	if (render_bloom) {
		blur(screen, width, height, bloom_radius);
		gl_render_alpha();
		
	}
	
}

void gl_render(void) {
	screen = GFX_fill_rect_hex_to_ptr8(screen, width, 0, 0, width, height, clear_colour);
	gl_render_alpha();
	
}

void gl_display(void) {
	GFX_blit_image(0, 0, width, height, screen);
	
}
