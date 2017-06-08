/*
 * api.c
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

#include "api.h"

static icon default_icon;
static colour default_colour;

static uint8* api_icon_off;
static uint8* api_icon_back;
static uint8* api_icon_save;
static uint8* api_icon_load;
static uint8* api_icon_next;

static uint8** api_font_mono_15px;
static uint8** api_font_aqua_50px;
static uint8** api_font_aqua_20px;

void api_rescorces_init(\
	uint8* _api_icon_off, \
	uint8* _api_icon_back, \
	uint8* _api_icon_save, \
	uint8* _api_icon_load, \
	uint8* _api_icon_next, \
	\
	uint8** _api_font_mono_15px, \
	uint8** _api_font_aqua_50px, \
	uint8** _api_font_aqua_20px \
	\
) {
	api_icon_off = _api_icon_off;
	api_icon_back = _api_icon_back;
	api_icon_save = _api_icon_save;
	api_icon_load = _api_icon_load;
	api_icon_next = _api_icon_next;
	
	api_font_mono_15px = _api_font_mono_15px;
	api_font_aqua_50px = _api_font_aqua_50px;
	api_font_aqua_20px = _api_font_aqua_20px;
	
}

uint8* api_get_icon(string name) {
	if (str_eql(name, "off")) return api_icon_off;
	if (str_eql(name, "back")) return api_icon_back;
	if (str_eql(name, "save")) return api_icon_save;
	if (str_eql(name, "load")) return api_icon_load;
	if (str_eql(name, "next")) return api_icon_next;
	
}

uint8** api_get_font(string name, uint8 _size) {
	if (str_eql(name, "monospace") && _size == 15) return api_font_mono_15px;
	if (str_eql(name, "aqua") && _size == 50) return api_font_aqua_50px;
	if (str_eql(name, "aqua") && _size == 20) return api_font_aqua_20px;
	
}

//uint16 font_aqua_20px_l,
//uint8 font_aqua_20px[94][font_aqua_20px_l],
//uint16 font_aqua_50px_l,
//uint8 font_aqua_50px[94][font_aqua_50px_l],

/*
void api_init(uint8* _default_icon, uint32 _colour, uint32 _default_colour) {
	default_icon = lib2d_create_icon(_default_icon, _colour);
	default_colour = lib2d_create_colour(_default_colour);
	
}

application create_application(string name, uint16 version, string description) {
	application _application;
	
	_application.name = name;
	_application.version = version;
	_application.description = description;
	
	_application.tab = 0;
	_application._icon = default_icon;
	_application._colour = default_colour;
	
	_application.code = "lib2d.text 0,0,255,255,255,\"Hello world!\";";
	_application = compile_code_to_commands(_application);
	
	return _application;
	
}

application set_tab_look(application _application, uint8 tab, icon _icon, colour _colour) {
	_application.tab = tab;
	_application._icon = _icon;
	_application._colour = _colour;
	
	return _application;
	
}

application compile_code_to_commands(application _appliction) {
	char* commands;
	uint32 len;
	
	_application.commands = commands;
	_application.commands_len = len;
	
	return _application;
	
}

application set_code(application _application, string code) {
	_application.code = code;
	return _application;
	
}

void run_application(application _application) {
	uint c;
	for (c = 0; c < _appliction.commands_len; c++) {
		//if 
		
	}
	
}
*/
