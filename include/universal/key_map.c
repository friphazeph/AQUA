/*
 * key_map.c
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

#include "key_map.h"

static string layout = "azerty";

static char qwerty_map[] = "``1234567890-=\b\tqwertyuiop[]\n`asdfghjkl;'`#\\zxcvbnm,./";
static char shift_qwerty_map[] = "``!\"#$%^&*()_+\b\tQWERTYUIOP{}\n`ASDFGHJKL:@~~|ZXCVBNM<>?";

static char azerty_map[] = "@@&|\"'($\\!<>)-\b\tazertyuiop^$\n`qsdfghjklm%`#<wxcvbn,;:=";
static char shift_azerty_map[] = "##1234567890'_\b\tAZERTYUIOP:*\n`QSDFGHJKLM%`#>WXCVBN?./+";

void set_layout(string _layout) {
	strcpy(layout, _layout);
	
}

string get_layout(void) {
	return layout;
	
}

char get_letter_from_map(char stroke, boolean shift) {
	if (shift) {
		if (stroke == 57) {
			return ' ';
			
		}
		
		if (str_eql(layout, "qwerty")) return shift_qwerty_map[stroke];
		if (str_eql(layout, "azerty")) return shift_azerty_map[stroke];
		
	} else {
		if (stroke == 57) {
			return ' ';
			
		}
		
		if (str_eql(layout, "qwerty")) return qwerty_map[stroke];
		if (str_eql(layout, "azerty")) return azerty_map[stroke];
		
	}
	
}
