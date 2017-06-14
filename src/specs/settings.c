/*
 * settings.c
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

#include "settings.h"

static char* settings;

void load_settings(void) {
	strcpy(settings, read28(SETTINGS_SECTOR, SETTINGS));
	invert_cursor(settings[2]);
	
}

void save_settings(void) {
	settings[SETTINGS] = 0;
	write28(3, settings, 512);
	flush();
	
}

void settings_reset(void) {
	set_setting(0, 'f');
	set_setting(1, 'f');
	set_setting(2, 'f');
	
}

void set_setting(int id, char value) {
	if (id == 2) invert_cursor(value);
	settings[id] = value;
	
}

char get_setting(int id) {
	return settings[id];
	
}
