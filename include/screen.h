/*
 * screen.h
 * 
 * Copyright 2016 Obiwac
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


#ifndef SCREEN_H
	#define SCREEN_H
	
	#include "types.h"
	#include "utils/string.h"
	
	#include "system/system.h"
	#include "drivers/serial_driver.h"
	
	int cursor_x;
	int cursor_y;
	
	const uint8 screen_width;
	const uint8 screen_height;
	const uint8 screen_depth;
	
	void clear_line(uint8 from, uint8 to);
	void update_cursor(void);
	void clear_screen(void);
	
	void scroll_up(uint8 line_number);
	void new_line_check(void);
	
	void printch(char c, char colour);
	void print(string ch, char colour);
	void println(string str, char colour);
	void print_hex(uint8 key, char colour);
	void print_no_lang(string ch, char colour);
	
	int get_log_length(void);
	char* get_log(char type);
	
#endif
