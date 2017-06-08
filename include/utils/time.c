/*
 * time.c
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


#include "time.h"

static string month_names[12] = {
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
	
};

int compile_time_string(void) {
	int time_string = 0;
	char* types = "smhdMy";
	
	int i;
	for (i = 0; i < 6; i++) {
		time_string += CMOS_get_time(types[i]);
		
	}
	
	return time_string;
	
}

string format_time(void) {
	string str = (char*) kmalloc(6);
	int len;
	
	strcpy(str, itoa(CMOS_get_time('h')));
	len = str_len(str);
	
	str[len] = ':';
	str[len + 1] = itoa(CMOS_get_time('m'))[0];
	str[len + 2] = itoa(CMOS_get_time('m'))[1];
	
	if (str[len + 2] != '\0') {
		str[len + 3] = '\0';
		
	}
	
	return str;
	
}

string get_day_name(void) {
	int day = CMOS_get_time('d');
	int month = CMOS_get_time('M');
	
	int y = CMOS_get_time('y');
	
	string ys;
	string cs;
	
	cs[0] = itoa(y)[0];
	cs[1] = itoa(y)[1];
	
	ys[0] = itoa(y)[2];
	ys[1] = itoa(y)[3];
	
	y = atoi(ys);
	int c = atoi(cs);
	
	day = fmod((day + floor(2.6 * month - 0.2) + y + floor((float) y / 4) + floor((float) c / 4) - 2 * c), 7);
	return itoa((int) day);
	
	if (day == 0) return "Monday";
	if (day == 1) return "Tuesday";
	if (day == 2) return "Wensday";
	if (day == 3) return "Thursday";
	if (day == 4) return "Friday";
	if (day == 5) return "Saturday";
	if (day == 6) return "Sunday";
	
}

string get_month_name(void) {
	return month_names[CMOS_get_time('M')];
	
}

void sleep(int seconds) {
	int iter = 0;
	
	int old_time = CMOS_get_time('s');
	int new_time;
	
	while (iter < seconds) {
		new_time = CMOS_get_time('s');
		
		if (new_time != old_time) {
			iter++;
			
		}
		
		old_time = new_time;
		
	}
	
}
