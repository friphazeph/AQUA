/*
 * bubble_sort.c
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

#include "bubble_sort.h"

float* bubble_swap(float* list, int index1, int index2) {
	float temp = list[index1];
	list[index1] = list[index2];
	list[index2] = temp;
	
	return list;
	
}

float* bubble_sort(int len, float* list) {
	int newn;
	int i;
	
	while (len > 0) {
		newn = 0;
		
		for (i = 1; i < len - 1; i++) {
			if (list[i - 1] > list[i]) {
				list = bubble_swap(list, i - 1, i);
				newn = i;
				
			}
			
		}
		
		len = newn;
		
	}
	
}
