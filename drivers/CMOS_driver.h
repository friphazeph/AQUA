/*
 * CMOS_driver.h
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


#ifndef CMOS_DRIVER_H
	#define CMOS_DRIVER_H
	
	#include "../types.h"
	#include "../system/system.h"
	
	#define CMOS_PORT_ADDRESS 0x70
	#define CMOS_PORT_DATA 0x71
	
	#define CURRENT_YEAR 2017 /// !!!: Change this every year
	
	int CMOS_get_update_in_progress_flag(void);
	uint8 CMOS_get_RTC_register(int reg);
	void CMOS_read_RTC(void);
	
	uint32 CMOS_get_time(char type);
	
#endif
