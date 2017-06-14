/*
 * ATA_driver.c
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


#include "ATA_driver.h"

static uint8 master;

static uint16 data_port; // 16 bit port
static uint16 error_port; // 8 bit port (same with all under)
static uint16 sector_count_port;

static uint16 LBA_low_port;
static uint16 LBA_mid_port;
static uint16 LBA_hi_port;

static uint16 device_port;
static uint16 command_port;
static uint16 control_port;

void ata_setup(uint8 _master, uint16 port_base) {
	data_port = port_base;
	error_port = port_base + 0x1;
	sector_count_port = port_base + 0x2;
	
	LBA_low_port = port_base + 0x3;
	LBA_mid_port = port_base + 0x4;
	LBA_hi_port = port_base + 0x5;
	
	device_port = port_base + 0x6;
	command_port = port_base + 0x7;
	control_port = port_base + 0x206;
	
	master = _master;
	
}

void identify(void) {
	outportb(device_port, master ? 0xA0 : 0xB0);
	outportb(control_port, 0);
	
	outportb(device_port, 0xA0);
	uint8 status = inportb(command_port);
	
	if (status == 0xFF) {
		print("Device BUS does not exist.\n", 0x06);
		return;
		
	}
	
	outportb(device_port, master ? 0xA0 : 0xB0);
	outportb(sector_count_port, 0);
	
	outportb(LBA_low_port, 0);
	outportb(LBA_mid_port, 0);
	outportb(LBA_hi_port, 0);
	
	outportb(command_port, 0xEC);
	
	status = inportb(command_port);
	
	if (status == 0x00) {
		print("Device does not exist.\n", 0x06);
		return;
		
	}
	
	while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = inportb(command_port);
		
	}
	
	if(status & 0x01) {
		print("Error detecting device identity.\n", 0x06);
		return;
		
	}
	
	int i;
	for (i = 0; i < 256; i++) {
		uint16 data = inportw(data_port);
		char* text = "  \0";
		
		text[1] = (data >> 8) & 0x00FF;
		text[0] = data & 0x00FF;
		
		print(text, 0x0f);
		
	}
	
}

char* read28(uint32 sector, int count) {
	char* data;
	
	if (sector & 0xF0000000) {
		print("The first four bytes that you are reading are not zeros.\n", 0x06);
		return 0;
		
	}
	
	if (count > BYTES_PER_SECTOR) {
		print("You are trying to read more data than the sector (defined in BYTES_PER_SECTOR). Use the fs_read_all function from file_system to read more than a sector.\n", 0x06);
		return 0;
		
	}
	
	outportb(device_port, (master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	outportb(error_port, 0);
	outportb(sector_count_port, 1);
	
	outportb(LBA_low_port, sector & 0x000000FF);
	outportb(LBA_mid_port, (sector & 0x0000FF00) >> 8);
	outportb(LBA_hi_port, (sector & 0x00FF0000) >> 16);
	outportb(command_port, 0x20);
	
	uint8 status = inportb(command_port);
	
	while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = inportb(command_port);
		
	}
	
	if(status & 0x01) {
		print("Error detecting device identity.\n", 0x06);
		return 0;
		
	}
	
	print("Reading from ATA: ", 0x0f);
	
	int16 i;
	for (i = 0; i < count; i += 2) {
		uint16 wdata = inportw(data_port);
		
		char* text = "  \0";
		
		text[1] = (wdata >> 8) & 0x00FF;
		text[0] = wdata & 0x00FF;
		
		print(text, 0x0f);
		
		data[i] = text[0];
		
		if (i + 1 < count) {
			data[i + 1] = text[1];
			
		}
		
	}
	
	for (i = count + (count % 2); i < BYTES_PER_SECTOR; i += 2) {
		inportw(data_port);
		
	}
	
	data[i] = '\0';
	return data;
	
}

void write28(uint32 sector, char* data, int count) {
	if (sector & 0xF0000000) {
		print("The first four bytes that you are writing are not zeros.\n", 0x06);
		return;
		
	}
	
	if (count > BYTES_PER_SECTOR) {
		print("You are trying to write more data than the sector (defined in BYTES_PER_SECTOR). Use the fs_write_all function from file_system to write more than a sector.\n", 0x06);
		return;
		
	}
	
	outportb(device_port, (master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	outportb(error_port, 0);
	outportb(sector_count_port, 1);
	
	outportb(LBA_low_port, sector & 0x000000FF);
	outportb(LBA_mid_port, (sector & 0x0000FF00) >> 8);
	outportb(LBA_hi_port, (sector & 0x00FF0000) >> 16);
	outportb(command_port, 0x30);
	
	print("Writing to ATA: ", 0x0f);
	
	uint16 i;
	for (i = 0; i < count; i += 2) {
		uint16 wdata = data[i];
		
		if (i + 1 < count) {
			wdata |= ((uint16) data[i + 1]) << 8;
			
		}
		
		char* text = "  \0";
		
		text[1] = (wdata >> 8) & 0x00FF;
		text[0] = wdata & 0x00FF;
		
		print(text, 0x0f);
		outportw(data_port, wdata);
		
	}
	
	for (i = count + (count % 2); i < BYTES_PER_SECTOR; i += 2) {
		outportw(data_port, 0x0000);
		
	}
	
}

void flush(void) {
	outportb(device_port, master ? 0xE0 : 0xF0);
	outportb(command_port, 0xE7);
	
	uint8 status = inportb(command_port);
	
	while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = inportb(command_port);
		
	}
	
	if(status & 0x01) {
		print("Error detecting device identity.\n", 0x06);
		return;
		
	}
	
}
