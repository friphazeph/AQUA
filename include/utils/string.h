/*
 * string.h
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


#ifndef STRING_H
	#define STRING_H
	
	#include "../types.h"
	#include "../memory/memory.h"
	
	string str_append(string base, string add);
	
	uint32 str_len(string ch);
	uint8 str_eql(string a, string b);
	
	string str_splice(string str, int a, int b);
	string str_rev(string str);
	
	char* strcpy(char* __dest, const char* __src);
	int str_cmp(const char* s1, const char* s2, size n);
	int str_cmp_ext2(string str1, string str2);
	int strcmp(const char* __s1, const char* __s2);
	
	int index_of(char c, char* str);
	void substrr(int s, int e, char* src, char* dest);
	
#endif
