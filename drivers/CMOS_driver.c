/*
 * CMOS_driver.c
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


#include "CMOS_driver.h"

static int century_register = 0x00;

static uint8 second;
static uint8 minute;
static uint8 hour;

static uint8 day;
static uint8 month;
static uint year;

int CMOS_get_update_in_progress_flag(void) {
	outportb(CMOS_PORT_ADDRESS, 0x0A);
	return inportb(CMOS_PORT_DATA) & 0x80;
	
}

uint8 CMOS_get_RTC_register(int reg) {
	outportb(CMOS_PORT_ADDRESS, reg);
	return inportb(CMOS_PORT_DATA);
	
}

void CMOS_read_RTC(void) {
	uint8 century;
	
	uint8 last_second;
	uint8 last_minute;
	uint8 last_hour;
	
	uint8 last_day;
	uint8 last_month;
	uint8 last_year;
	
	uint8 last_century;
	uint8 register_b;
	
	while (CMOS_get_update_in_progress_flag());
	
	second = CMOS_get_RTC_register(0x00);
	minute = CMOS_get_RTC_register(0x02);
	hour = CMOS_get_RTC_register(0x04);
	
	day = CMOS_get_RTC_register(0x07);
	month = CMOS_get_RTC_register(0x08);
	year = CMOS_get_RTC_register(0x08);
	
	if (century_register != 0) {
		century = CMOS_get_RTC_register(century_register);
		
	}
	
	do {
		last_second = second;
		last_minute = minute;
		last_hour = hour;
		
		last_day = day;
		last_month = month;
		last_year = year;
		last_century = century;
		
		while (CMOS_get_update_in_progress_flag());
		
		second = CMOS_get_RTC_register(0x00);
		minute = CMOS_get_RTC_register(0x02);
		hour = CMOS_get_RTC_register(0x04);
		
		day = CMOS_get_RTC_register(0x07);
		month = CMOS_get_RTC_register(0x08);
		year = CMOS_get_RTC_register(0x08);
		
		if (century_register != 0) {
			century = CMOS_get_RTC_register(century_register);
			
		}
		
	} while ((last_second != second) || (last_minute != minute) || (last_hour != hour) || (last_day != day) || (last_month != month) || (last_year != year) || (last_century != century));
	
	register_b = CMOS_get_RTC_register(0x0B);
	
	if (!(register_b & 0x04)) {
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
		
		month = (month & 0x0F) + ((month / 16) * 10);
		year = (year & 0x0F) + ((year / 16) * 10);
		
		if (century_register != 0) {
			century = (century & 0x0F) + ((century / 16) * 10);
			
		}
		
	}
	
	if (!(register_b & 0x02) && (hour & 0x80)) {
		hour = ((hour & 0x7F) + 12) % 24;
		
	}
	
	if (century_register != 0) {
		year += century * 100;
		
	} else {
		year += (CURRENT_YEAR / 100) * 100;
		
		if (year < CURRENT_YEAR) {
			year += 100;
			
		}
		
	}
	
}

uint CMOS_get_time(char type) {
	CMOS_read_RTC();
	
	if (type == 's') return (uint) second;
	if (type == 'm') return (uint) minute;
	if (type == 'h') return (uint) hour;
	
	if (type == 'd') return (uint) day;
	if (type == 'M') return (uint) month;
	if (type == 'y') return year;
	
}
