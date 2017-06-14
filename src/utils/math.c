/*
 * math.c
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


#include "math.h"

static uint32 next = 0;

static int32 rand(void) {
	next = next * 1103515245 + 12345;
	return (uint32) (next / 65536) % RAND_MAX;
	
}

void rand_seed(int seed) {
	next = seed;
	
}

int32 rand_int(uint32 min, uint32 max) {
	max -= min - 1;
	return (int32) ((float) rand() / ((float) RAND_MAX / (float) max)) + min;
	
}

int rand_choose(int list[]) {
	return list[rand_int(0, (sizeof(list) / sizeof(list[0])) - 1)];
	
}

int ipow(int val, int exp) {
	int i;
	for (i = 0; i < exp; i++) {
		val *= val;
		
	}
	
	return val;
	
}

float pow(float val, int exp) {
	int i;
	for (i = 0; i < exp; i++) {
		val *= val;
		
	}
	
	return val;
	
}

float nrt(float val, int rot) {
	if (rot == 0) {
		return 1.0f;
		
	} else if (rot == 1) {
		return val;
		
	} else if (rot == 2) {
		uint i = *(uint*) &val;
		i += 127 << 23;
		i >>= 1;
		
		return *(float*) &i;
		
	} else {
		return pow(val, 1.0 / (float) rot);
		
	}
	
}

float sqrt(float val) { return nrt(val, 2); }
float cbrt(float val) { return nrt(val, 3); }

float fabs(float x) {
	if (x >= 0.0) return x;
	else return -x;
	
}

int abs(int x) {
	if (x >= 0) return x;
	else return -x;
	
}

int round(float x) { return (int) (x + 0.5f); }
int floor(float x) { return (int) x; }
int ceil(float x) { return (int) x + 1.0f; }

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int exp(int x) { return pow(E, x); }

int average(int length, int array[length]) {
	int sum = 0;
	
	int i;
	for (i = 0; i < length; i++) {
		sum += array[i];
		
	}
	
	return sum / length;
	
}

float fmod(float x, float y) {
	return (x - y * floor(x / y));
	
}

/*
float fac(float x) {
	if (x == 3) return 6;
	if (x == 5) return 120;
	if (x == 7) return 5040;
	
}

float sin(float x) {
	return x - pow(x, 3) / fac(3) + pow(x, 5) / fac(5) - pow(x, 7) / fac(7);
	
}*/

float sin(float x) {
	/*if (x > 0) x = fmod((x + PI), (_2PI) - PI);
	else x = fmod((x - PI), (_2PI) + PI);
	
	if (x > 0) return _4DPI * x + _M4DPISQ * x * x;
	else return _4DPI * x - _M4DPISQ * x * x;*/
	
	x += 180;
	int flx = floor(x);
	
	float c1 = table_sin[flx];
	float c2 = table_sin[flx + 1];
	
	return c1 + ((c2 - c1) / (1 / (x - flx)));
	
}

float cos(float x) {
	/*x = PID2 - x;
	
	if (x > 0) x = fmod((x + PI), (_2PI) - PI);
	else x = fmod((x - PI), (_2PI) + PI);
	
	if (x > 0) return _4DPI * x + _M4DPISQ * x * x;
	else return _4DPI * x - _M4DPISQ * x * x;*/
	
	x += 180;
	int flx = floor(x);
	
	float c1 = table_cos[flx];
	float c2 = table_cos[flx + 1];
	
	return c1 + ((c2 - c1) / (1 / (x - flx)));
	
}
