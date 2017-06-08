/*
 * string.c
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


#include "string.h"

string str_append(string base, string add) {
	int index = str_len(base);
	
	int i;
	for (i = 0; i < str_len(add); i++) {
		base[index + i] = add[i];
		
	}
	
	base[index + i] = 0;
	return base;
	
}

uint32 str_len(string ch) {
	uint32 i = 1;
	while (ch[i++]);
	
	return --i;
	
}

uint8 str_eql(string a, string b) {
	uint8 result = 1;
	int _size = str_len(a);
	
	if (_size != str_len(b)) {
		result = 0;
		
	} else {
		int i;
		for (i = 0; i <= _size; i++) {
			if (a[i] != b[i]) {
				result = 0;
				
			}
			
		}
		
	}
	
	return result;
	
}

string str_splice(string str, int a, int b) {
	string result;
	
	int i;
	for (i = a; i < str_len(str) - b; i++) {
		result[i - a] = str[i];
		
	}
	
	result[i + 1] = 0;
	
	return result;
	
}

string str_rev(string str) {
	string result;
	int n;
	int c;
	int d;
	
	n = str_len(str);
	
	for (c = n - 1, d = 0; c >= 0; c--, d++) {
		result[d] = str[c];
		
	}
	
	result[d] = '\0';
	
	return result;
	
}

char* strcpy(char* __dest, const char* __src) {
	int i = 0;
	
	while (1) {
		__dest[i] = __src[i];
		if (__dest[i] == '\0') break;
		i++;
		
	}
	
	return __dest;
	
}

int str_cmp(const char* s1, const char* s2, size n) {
	while (n && *s1 && (*s1 == *s2)) {
		++s1;
		++s2;
		--n;
		
	}
	
	if (n == 0) {
		return 0;
		
	}
	
	return (*(unsigned char*) s1 - *(unsigned char*) s2);
	
}

int str_cmp_ext2(string str1, string str2) {
	int i = 0;
	boolean flag = FALSE;
	
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			flag = TRUE;
			break;
			
		}
		
		i++;
 		
	}
	
	return flag == FALSE && str1[i] != '\0' && str2[i] != '\0';
	
}

int strcmp(const char* __s1, const char* __s2) {
	while(*__s1 && (*__s1 == *__s2)) {
		__s1++;
		__s2++;
		
	}
	
    return *(const unsigned char*) __s1 - *(const unsigned char*) __s2;
	
}

int index_of(char c, char* str) {
	int i = 0;
	
	while (str[i] != '\0') {
		if (str[i] == c) {
			return i;
			
		}
		
		i++;
		
	}
	
	return str_len(str);
	
}

void substrr(int s, int e, char* src, char* dest) {
	memcpy_ext2(dest, &src, str_len(src));
	dest[str_len(src)] = '\0';
	
}
