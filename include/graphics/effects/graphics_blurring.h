/*
 * graphics_blurring.h
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


#ifndef GRAPHICS_GRAPHICS_BLURRING_H
	#define GRAPHICS_GRAPHICS_BLURRING_H
	
	#include "../../types.h"
	#include "../../utils/math.h"
	#include "../../memory/memory.h"
	
	uint8* blur(uint8* surface, uint32 width, uint32 height, uint8 radius);
	
	uint8* perfect_gaussian(uint8* surface, uint32 width, uint32 height, uint8 radius);
	uint8* blurring_wrapper(char type, uint8* surface, uint32 width, uint32 height, uint8 radius);
	
	int* boxes_for_gauss(float sigma, float n);
	uint8* gauss_blur(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius);
	uint8* box_blur(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius);
	
	uint8* box_blur_H(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius);
	uint8* box_blur_T(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius);
	
#endif
