/*
 * ata_driver.h
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


#ifndef ATA_DRIVER_H
	#define ATA_DRIVER_H
	
	#include "../types.h"
	
	#include "../screen.h"
	
	#include "../system/system.h"
	
	#define BYTES_PER_SECTOR 512
	
	void ata_setup(uint8 _master, uint16 port_base);
	
	void identify(void);
	char* read28(uint32 sector, int count); // count = 512 by default
	void write28(uint32 sector, char* data, int count);
	void flush(void);
	
#endif
