/*
 * serial_driver.h
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


#ifndef SERIAL_DRIVER_H
	#define SERIAL_DRIVER_H
	
	#include "../types.h"
	#include "../system/system.h"
	
	#define COM1 0x3F8
	#define COM2 0x2F8
	#define COM3 0x3E8
	#define COM4 0x2E8
	
	void set_serial_port(uint16 _port);
	uint16 get_serial_port(void);
	
	void init_serial(void);
	
	boolean serial_received(void);
	char read_serial(void);
	
	boolean is_transmit_empty(void);
	void write_serial(char c);
	
#endif
