/*
 * ACPI_driver.h
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


#ifndef ACPI_DRIVER_H
	#define ACPI_DRIVER_H
	
	#include "../types.h"
	#include "../utils/time.h"
	#include "../screen.h"
	
	#include "../system/system.h"
	#include "../memory/memory.h"
	
	typedef uint32 dword;
	typedef uint16 word;
	typedef uint8 byte;
	
	struct RSD_ptr {
		byte signature[8];
		byte check_sum;
		
		byte OEM_ID[6];
		byte revision;
		
		dword* RSDT_address;
		
	};
	
	struct FACP {
		byte signature[4];
		dword length;
		
		byte unneded1[40 - 8];
		dword* DSDT;
		
		byte unneded2[48 - 44];
		dword* SMI_CMD;
		
		byte ACPI_ENABLE;
		byte ACPI_DISABLE;
		byte unneded3[64 - 54];
		
		dword* PM1a_CNT_BLK;
		dword* PM1b_CNT_BLK;
		
		byte unneded4[89 - 72];
		byte PM1_CNT_LEN;
		
	};
	
	uint* ACPI_check_RSD_ptr(uint* ptr);
	uint* ACPI_get_RSD_ptr(void);
	int ACPI_check_header(uint* ptr, char* sig);
	
	int ACPI_enable(void);
	int ACPI_init(void);
	
	void ACPI_poweroff(void);
	
#endif
