/*
 * pci.h
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

#ifndef PCI_H
	#define PCI_H
	
	#include "../types.h"
	#include "../screen.h"
	#include "../system/system.h"
	
	#define PCI_DATA_PORT 0xCF8 // PCI_CONFIG_ADDRESS
	#define PCI_COMMAND_PORT 0xCFC // PCI_CONFIG_DATA
	
	typedef struct PCI_device {
		uint8 bus;
		uint8 slot;
		uint8 flags;
		
	} PCI_device;
	
	uint16 PCI_read_word(uint8 bus, uint8 slot, uint8 func, uint8 offset);
	uint16 get_PCI_vendor(uint8 bus, uint8 slot, uint8 function);
	
	PCI_device get_PCI_device(uint8 tclass, uint8 tsub_class, uint8 tprog_IF);
	void PCI_debug(void);
	void print_PCI_class_code(uint8 class_code, uint8 sub_class, uint8 prog_IF);
	
#endif
