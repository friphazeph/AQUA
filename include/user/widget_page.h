/*
 * widget_page.h
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

#ifndef WIDGET_PAGE_H
	#define WIDGET_PAGE_H
	
	#include "../types.h"
	#include "../memory/memory.h"
	
	#include "../universal/lang.h"
	#include "../utils/string.h"
	
	#include "cursor_task.h"
	
	#include "../specs/RAM_count.h"
	
	#include "../graphics/UI.h"
	#include "../graphics/GFX.h"
	#include "../graphics/effects/graphics_blurring.h"
	
	void init_widget_page( \
		uint32 _width, \
		uint32 _height, \
		\
		uint8* _wallpaper, \
		\
		uint8* _icon_off, \
		uint8* _icon_back, \
		uint8* _icon_edit, \
		\
		uint8** _font_aqua_20px, \
		uint8** _font_aqua_50px \
		\
	);
	
	void widget_set_desktop(void);
	void enter_widget_page(void);
	
#endif
