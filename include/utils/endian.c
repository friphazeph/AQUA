/*
 * endian.c
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

#include "endian.h"

uint16 switch_endian16(uint16 nb) {
	return (nb >> 8) | (nb << 8);
	
}

uint32 switch_endian32(uint32 nb) {
	return ((nb >> 24) & 0xff) |
		((nb << 8) & 0xff0000) |
		((nb >> 8) & 0xff00) |
		((nb << 24) & 0xff000000);
	
}
