/*
 * mouse_driver.h
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


#ifndef MOUSE_DRIVER_H
	#define MOUSE_DRIVER_H
	
	#include "../types.h"
	#include "../screen.h"
	
	#include "../graphics/GFX.h"
	#include "../system/system.h"
	#include "../interrupts/irq.h"
	
	void mouse_move(int8 x, int8 y);
	void mouse_press(uint8 button);
	void mouse_release(uint8 button);
	
	void mouse_handler(struct registers* r);
	void mouse_install(void);
	
	int32 get_mouse_x(void);
	int32 get_mouse_y(void);
	
	uint8 untouch_mouse_press(void);
	uint8 untouch_mouse_release(void);
	void clear_untouch(void);
	
	uint8 poll_mouse_press(void);
	uint8 poll_mouse_release(void);
	
#endif
