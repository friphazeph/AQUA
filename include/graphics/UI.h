/*
 * UI.h
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


#ifndef UI_H
	#define UI_H
	
	#include "../types.h"
	#include "../memory/heap.h"
	
	#include "../utils/time.h"
	#include "../utils/string.h"
	#include "../universal/lang.h"
	
	#include "../user/cursor_task.h"
	#include "../drivers/ACPI_driver.h"
	#include "../drivers/keyboard_driver.h"
	
	#include "GFX.h"
	
	#define DIALOG_MAX_WIDTH 800
	#define DIALOG_WIDTH 300
	#define DIALOG_HEIGHT 75
	#define DIALOG_ALPHA 90
	
	#define ANIMATION_STEP 20
	
	typedef struct {
		uint32 id;
		uint32 x;
		uint32 y;
		
		uint32 len;
		string content;
		boolean hidden;
		
	} UI_component_entry;
	
	typedef struct {
		string name;
		uint32 colour;
		uint8* icon;
		
	} tab_entry;
	
	typedef struct {
		uint8 amount;
		tab_entry entries[256];
		
	} tab_entry_list;
	
	void UI_init(uint16 _fdl, uint8 _font_data[94][_fdl]);
	
	/*UI_component_entry UI_new_component_entry(uint32 x, uint32 y, uint32 len, string content, boolean hidden);
	UI_component_entry UI_update_component_entry(UI_component_entry entry);
	uint8* UI_get_component_entry_ptr8(UI_component_entry entry, uint8* buffer);
	*/
	void UI_turn_off(void);
	boolean UI_dialog(uint16 _fdl, uint8 _font_data[94][_fdl], string text, uint8 icon_data0[], uint32 icon_colour0, string command0, uint8 icon_data1[], uint32 icon_colour1, string command1);
	
	void UI_animate_desktop(string type, uint16 time, uint8* image_data);
	void UI_animate_card(string type, uint16 time, uint8* card_data, uint32 _width, uint32 _height);
	void UI_slide_down(uint16 _fdl, uint8 _font_data[94][_fdl], uint8 tab, uint8 entry_count, tab_entry entries[entry_count]);
	
#endif
