/*
 * heap.c
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


#include "heap.h"

static uint8 heap_space[MAX_HEAP_SPACE];
static uint8 frames[MAX_FRAMES];

static frame frame_buffer;
static frame_set frame_set_buffer;

static uint32 kfree_buffer;

void init_heap(void) {
	int f;
	for (f = 0; f < MAX_FRAMES; f++) {
		frames[f] = 0;
		
	}
	
}

frame first_available_frame(void) {
	int s;
	int f;
	for (s = 0; s < MAX_FRAMES; s++) {
		for (f = 0; f < 8; f++) {
			if (!(frames[s] >> f & 1)) {
				frame_buffer.num = s * 8 + f;
				frame_buffer.set = s;
				frame_buffer.pos = f;
				
				return frame_buffer;
				
			}
			
		}
		
	}
	
	println("The heap ran out of memory.", 0x06);
	
}

frame_set first_available_frameset(uint32 len) {
	uint32 num_frames = 0;
	
	int s;
	int f;
	for (s = 0; s < MAX_FRAMES; s++) {
		for (f = 0; f < 8; f++) {
			if (!(frames[s] >> f & 1)) {
				if (num_frames == 0) {
					frame_buffer.num = s * 8 + f;
					frame_buffer.set = s;
					frame_buffer.pos = f;
					
				}
				
				num_frames++;
				
			} else {
				num_frames = 0;
				
			}
			
			if (num_frames == len) {
				frame_set_buffer.start = frame_buffer;
				frame_set_buffer.len = num_frames;
				
				return frame_set_buffer;
				
			}
			
		}
		
	}
	
	println("The heap doesn't have enough consecutive frames to fit a variable", 0x06);
	print("Variable size: ~", 0x06);
	print_hex(len * 16, 0x06);
	println("bytes", 0x06);
	
	while (TRUE);
	
}

frame_set fsalloc(uint32 len) {
	first_available_frameset(len);
	frame buffer;
	
	int f;
	for (f = 0; f < frame_set_buffer.len; f++) {
		buffer = get_frame(f + frame_set_buffer.start.num);
		frames[buffer.set] |= 1 << buffer.pos;
		
	}
	
	frame_set _frame_set;
	_frame_set.start = get_frame(frame_set_buffer.start.num);
	_frame_set.len = len;
	
	return _frame_set;
	
}

frame get_frame(uint32 id) {
	frame _frame;
	
	_frame.set = id / 8;
	_frame.pos = id % 8;
	_frame.num = id;
	
	return _frame;
	
}

frame falloc(void) {
	first_available_frame();
	frames[frame_buffer.set] = frames[frame_buffer.set] | (1 << frame_buffer.pos);
	
	frame _frame;
	
	_frame.set = frame_buffer.set;
	_frame.pos = frame_buffer.pos;
	_frame.num = frame_buffer.num;
	
	return _frame;
	
}

void ffree(frame _frame) {
	frames[_frame.set] &= ~(1 << _frame.pos);
	
}

void* kmalloc(uint32 len) {
	frame_set_buffer = fsalloc(len / 0x10 + 1);
	return &heap_space[frame_set_buffer.start.num * 0x10];
	
}

void kfree(void* __ptr, uint32 len) {
	kfree_buffer = (uint32) __ptr - (uint32) &heap_space;
	kfree_buffer /= 0x10;
	
	int b;
	for (b = 0; b < len / 0x10 + 1; b++) {
		frame_buffer = get_frame(b + kfree_buffer);
		ffree(frame_buffer);
		
	}
	
}

void strfree(string str) {
	kfree(str, str_len(str) + 1);
	
}

void get_frames_for_paging(void) {
	return (uint32*) frames;
	
}
