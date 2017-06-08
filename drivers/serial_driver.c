/*
 * serial_driver.c
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



#include "serial_driver.h"

static uint16 port = COM1;

void set_serial_port(uint16 _port) {
	port = _port;
	
} uint16 get_serial_port(void) {
	return port;
	
}

void init_serial(void) {
	outportb(port + 1, 0x00);
	outportb(port + 3, 0x80);
	outportb(port + 0, 0x03);
	outportb(port + 1, 0x00);
	outportb(port + 3, 0x03);
	outportb(port + 2, 0xC7);
	outportb(port + 4, 0x0B);
	
}

boolean serial_received(void) {
	return inportb(port + 5) & 1;
	
}

char read_serial(void) {
	while (serial_received() == 0);
	return inportb(port);
	
}

boolean is_transmit_empty(void) {
	return inportb(port + 5) & 0x20;
	
}

void write_serial(char c) {
	while (is_transmit_empty() == 0);
	outportb(port, c);
	
}
