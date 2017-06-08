/*
 * screen.c
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


#include "screen.h"

int cursor_x = 0;
int cursor_y = 0;

const uint8 screen_width = 80;
const uint8 screen_height = 25;
const uint8 screen_depth = 2;

static char log[0xFFFF];
static char log_colours[0xFFFF];
static int log_length = 0;

void clear_line(uint8 from, uint8 to) {
	string vidmem = (string) 0xb8000;
	
	uint16 i;
	for (i = screen_width * from * screen_depth; i < (screen_width * (to + 1) * screen_depth); i++) {
		vidmem[i] = 0x0;
		
	}
	
}

void update_cursor(void) {
	unsigned temp;
	temp = cursor_y * screen_width + cursor_x;
	
	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
	
}

void clear_screen(void) {
	clear_line(0, screen_height - 1);
	cursor_x = 0;
	cursor_y = 0;
	update_cursor();
	
}

void scroll_up(uint8 line_number) {
	string vidmem = (string) 0xb8000;
	
	uint16 i;
	for (i = 0; i < screen_width * (screen_height - 1) * 2; i++) {
		vidmem[i] = vidmem[i + screen_width * 2 * line_number];
		
	}
	
	clear_line(screen_height - 1 - line_number, screen_height - 1);
	
	if ((cursor_y - line_number) < 0) {
		cursor_x = 0;
		cursor_y = 0;
		
	} else {
		cursor_y -= line_number;
		
	}
	
	update_cursor();
	
}

void new_line_check(void) {
	if (cursor_y >= screen_height - 1) {
		scroll_up(1);
		
	}
	
}

void printch(char c, char colour) {
	string vidmem = (string) 0xb8000;
	
	//write_serial(c);
	log[log_length] = c;
	log_colours[log_length] = colour;
	log_length++;
	
	switch (c) {
		case (0x08): {
			if (cursor_x > 0) {
				cursor_x--;
				vidmem[(cursor_y * screen_width + cursor_x) * screen_depth] = 0x00;
				
			}
			
			break;
			
		} case (0x09): {
			cursor_x = (cursor_x + 8) & ~(8 - 1);
			break;
			
		} case ('\r'): {
			cursor_x = 0;
			break;
			
		} case ('\n'): {
			cursor_x = 0;
			cursor_y++;
			
			break;
			
		} default: {
			vidmem[(cursor_y * screen_width + cursor_x) * screen_depth] = c;
			vidmem[(cursor_y * screen_width + cursor_x) * screen_depth + 1] = colour;
			cursor_x++;
			
			break;
			
		}
		
	}
	
	if (cursor_x >= screen_width) {
		cursor_x = 0;
		cursor_y++;
		
	}
	
	new_line_check();
	update_cursor();
	
}

void print(string ch, char colour) {
	string tch = ch;
	
	uint16 i;
	for (i = 0; i < str_len(tch); i++) {
		printch(tch[i], colour);
		
	}
	
}

void println(string str, char colour) {
	print(str, colour);
	printch('\n', colour);
	
}

void print_hex(uint8 key, char colour) {
	char* foo = "00";
	char* hex = "0123456789ABCDEF";
	
	foo[0] = hex[(key >> 4) & 0xF];
	foo[1] = hex[key & 0xF];
	
	print(foo, colour);
	
}

void print_no_lang(string ch, char colour) {
	uint16 i;
	for (i = 0; i < str_len(ch); i++) {
		printch(ch[i], colour);
		
	}
	
}

int get_log_length(void) {
	return log_length;
	
}

char* get_log(char type) {
	if (type == 'c') return (char*) log_colours;
	else return (char*) log;
	
}
