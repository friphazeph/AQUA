/*
 * AQUA_fs.h
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


#ifndef AQUA_FS_H
	#define AQUA_FS_H
	
	#include "../types.h"
	#include "../drivers/ATA_driver.h"
	
	#include "../utils/math.h"
	#include "../utils/string.h"
	#include "../universal/lang.h"
	
	#define ALLOCATION_INDEX_END 4096 * 2 // makes room for 4096 files
	
	boolean init_fs(void);
	void FS_reformat(void);
	
	string FS_get_name(void);
	void FS_set_name(string name);
	
	void FS_save_settings(boolean* settings, int len);
	boolean* FS_load_settings(int len);
	
	void FS_write_program(int sector_id, string code);
	string FS_read_program(int sector_id);
	
	string FS_write_number_u32(uint32 number);
	uint32 FS_read_number_u32(string sector);
	
	int FS_find_file(string name);
	boolean FS_in(int index, int* list, int length);
	string get_avalible_sectors(int length);
	
	string FS_read_file(string sector);
	void FS_write_file(string sector, string content);
	void FS_new_file(string name, string content);
	
#endif
