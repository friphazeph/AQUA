/*
 * api.h
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

#ifndef API_H
	#define API_H
	
	#include "../../types.h"
	#include "../../memory/memory.h"
	
	#include "../../utils/utils.h"
	#include "../../utils/string.h"
	#include "../../universal/lang.h"
	
	#include "../cursor_task.h"
	//#include "../libs/lib_2d.h"
	
	#include "../../graphics/UI.h"
	#include "../../graphics/GFX.h"
	
	#include "../../file_system/AQUA_fs.h"
	
	#include "../interpreters/basic_interpreter.h"
	
	/*#include "../../graphics/GFX_loader_files/icons/icon_back.h"
	#include "../../graphics/GFX_loader_files/fonts/font_aqua_50px.h"
	#include "../../graphics/GFX_loader_files/fonts/font_aqua_20px.h"*/
	
	/*typedef struct {
		string name;
		uint16 version;
		string description;
		
		uint8 tab;
		icon _icon;
		colour _colour;
		
		string code;
		char* commands;
		uint32 commands_len;
		
	} application;*/
	
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
	);
	
	uint8* api_get_icon(string name);
	uint8** api_get_font(string name, uint8 _size);
	
	//uint16 font_aqua_20px_l,
	//uint8 font_aqua_20px[94][font_aqua_20px_l],
	//uint16 font_aqua_50px_l,
	//uint8 font_aqua_50px[94][font_aqua_50px_l],
	
	/*void api_init(uint8* _default_icon, uint32 _colour, uint32 _default_colour);
	
	application create_application(string name, uint16 version, string description);
	application set_tab_look(application _application, uint8 tab, icon _icon, colour _colour);
	
	application compile_code_to_commands(application _application);
	application set_code(application _application, string code);
	
	void run_application(application _appliction);
	*/
	//void add_application(application _application);
	//void remove_application(string name);
	
#endif
