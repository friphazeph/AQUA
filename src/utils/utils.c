/*
 * utils.c
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


#include "utils.h"

string itoa(int val) {
	if (val == 0) {
		return "0";
		
	}
	
	string result;
	int len = 0;
	
	for (; val > 0; ++len) {
		result[len] = val % 10 + '0';
		val /= 10;
		
	}
	
	result[len] = 0;
	
	int i;
	for (i = 0; i < len / 2; ++i) {
		char c = result[i];
		
		result[i] = result[len - i - 1];
		result[len - i - 1] = c;
		
	}
	
	return result;
	
}

int atoi(string s) {
	int k = 0;
	while (*s) {
		k = (k << 3) + (k << 1) + (*s) - '0';
		s++;
		
	}
	
	return k;
	
}
