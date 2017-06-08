/*
 * start_wizard.h
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

#ifndef START_WIZARD_H
	#define START_WIZARD_H
	
	#include "../types.h"
	
	#include "app_launcher.h"
	
	#include "../utils/string.h"
	#include "../universal/lang.h"
	
	#include "../graphics/UI.h"
	#include "../graphics/GFX.h"
	
	#include "../universal/lang.h"
	#include "../universal/flags/hu.h"
	#include "../universal/flags/ge.h"
	#include "../universal/flags/en-uk.h"
	#include "../universal/flags/fr-be.h"
	
	void init_start_wizard(
		uint8* _wallpaper, \
		uint8* _icon_next, \
		\
		uint8** _font_aqua_20px \
		\
	);
	
	void enter_start_wizard(void);
	
#endif
