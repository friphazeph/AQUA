/*
 * math.h
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


#ifndef MATH_H
	#define MATH_H
	
	#include "../types.h"
	#include "../trig_tables/cos.h"
	#include "../trig_tables/sin.h"
	
	#define RAND_MAX 32768
	
	#define PI 3.14159265
	#define E 2.71828182
	
	#define _2PI 2.0 * PI
	#define PID2 PI / 2.0
	#define _4DPI 4.0 / PI
	#define _M4DPISQ -4.0 / (PI * PI)
	
	#define EXTRA_PRESISION FALSE
	
	static int32 rand(void);
	void rand_seed(int seed);
	
	int32 rand_int(uint32 min, uint32 max);
	int rand_choose(int list[]);
	
	int ipow(int val, int exp);
	float pow(float val, int exp);
	
	float nrt(float val, int rot);
	float sqrt(float val);
	float cbrt(float val);
	
	float fabs(float x);
	int abs(int x);
	
	int round(float x);
	int floor(float x);
	int ceil(float x);
	
	int min(int a, int b);
	int max(int a, int b);
	
	int exp(int x);
	
	int average(int length, int array[length]);
	
	float fmod(float x, float y);
	float cos(float x);
	float sin(float x);
	
#endif
