/*
 * AQUA_fs.c
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


#include "AQUA_fs.h"

static string DISK_SIGNATURE = "AQUA OS 1.5 file system signature. DO NOT ATTEMPT TO MODIFY THIS SECTOR AT THE RISK OF LOSING ALL DATA ON THE DISK.";

static boolean FS_str_eql(string a, string b) {
	boolean result = TRUE;
	
	int i;
	for (i = 0; i <= str_len(b); i++) {
		if (a[i] != b[i]) {
			result = FALSE;
			
		}
		
	}
	
	return result;
	
}

boolean init_fs(void) {
	string lang;
	
	if (FS_str_eql(read28(1, 512), DISK_SIGNATURE)) {
		lang = read28(2, 512);
		
		if (FS_str_eql(lang, "en-uk")) set_lang("en-uk");
		if (FS_str_eql(lang, "fr-be")) set_lang("fr-be");
		if (FS_str_eql(lang, "ge")) set_lang("ge");
		if (FS_str_eql(lang, "hu")) set_lang("hu");
		
		return FALSE;
		
	} else {
		return TRUE;
		
	}
	
}

string FS_get_name(void) {
	return read28(4, 512);
	
} void FS_set_name(string name) {
	int len = str_len(name);
	
	write28(4, name, len + 1);
	flush();
	
}

void FS_save_settings(boolean* settings, int len) {
	string str;
	
	int b;
	for (b = 0; b < len; b++) {
		if (settings[b] == TRUE) str[b] = 't';
		if (settings[b] == FALSE) str[b] = 'f';
		
	}
	
	write28(3, str, 512);
	flush();
	
}

boolean* FS_load_settings(int len) {
	boolean* result;
	string str = read28(3, 512);
	
	int b;
	for (b = 0; b < len; b++) {
		if (str[b] == 't') result[b] = TRUE;
		if (str[b] == 'f') result[b] = FALSE;
		
	}
	
}

void FS_reformat(void) {
	write28(1, DISK_SIGNATURE, 512);
	flush();
	write28(2, get_lang(), 512);
	flush();
	write28(3, "fff", 512);
	flush();
	write28(4, "Jeff", 512);
	flush();
	
	write28(19, FS_write_number_u32(0), 4);
	flush();
	
}

void FS_write_program(int sector_id, string code) {
	char sector[512];
	
	int i;
	for (i = 0; i < str_len(code); i++) {
		sector[i % 512] = code[i];
		
		if (!((i + 1) % 512)) {
			write28((i / 512) + sector_id, sector, 512);
			flush();
			
		}
		
	}
	
	write28((i / 512) + sector_id, sector, i % 512);
	flush();
	
}

string FS_read_program(int sector_id) {
	string result;
	char sector[512];
	
	int i;
	int g = 0;
	
	boolean getting = TRUE;
	while (getting) {
		strcpy(sector, read28(g + sector_id, 512));
		
		for (i = 0; i < 512; i++) {
			result[512 * g + i] = sector[i];
			
			if (sector[i] == '\0') {
				getting = FALSE;
				break;
				
			}
			
		}
		
		if (!getting) break;
		g++;
		
	}
	
	return result;
	
}

string FS_write_number_u32(uint32 number) {
	string sector;
	
	sector[3] = number & 0xff;
	sector[2] = (number >> 8) & 0xff;
	sector[1] = (number >> 16) & 0xff;
	sector[0] = (number >> 24) & 0xff;
	
	sector[4] = '\0';
	return sector;
	
}

uint32 FS_read_number_u32(string sector) {
	uint8 a = sector[0] & 0xff;
	uint8 b = sector[1] & 0xff;
	uint8 c = sector[2] & 0xff;
	uint8 d = sector[3] & 0xff;
	
	return (a << 24) + \
		(b << 16) + \
		(c << 8) + \
		d;
	
}

int FS_find_file(string name) {
	int f;
	for (f = 20; f < 20 + FS_read_number_u32(read28(19, 4)) * 2; f += 2) {
		if (FS_str_eql(read28(f, 512), name)) return f;
		
	}
	
	return -1;
	
}

boolean FS_in(int index, int* list, int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (list[i] == index) return TRUE;
		
	}
	
	return FALSE;
	
}

string get_avalible_sectors(int length) {
	uint32* sector_list;
	int sectors = 0;
	string sector;
	
	int f;
	int max_files = FS_read_number_u32(read28(19, 4));
	
	char command;
	int number;
	
	char packet[4];
	int packet_offset = 0;
	
	int t;
	int c;
	
	int s = 0;
	for (f = 0; f <= max_files; f++) {
		sector = read28(s + 20, 512);
		s += 2;
		
		for (c = 0; c < 512; c++) {
			if (packet_offset > 4) {
				number = FS_read_number_u32(packet);
				println("{ over here", 0x0f);
				println(itoa(number), 0x0f);
				println("} not here", 0x0f);
				command = sector[packet_offset];
				
				if (command == ',') {
					sector_list[sectors] = number;
					sectors++;
					
				} else if (command == '-') {
					for (t = sector_list[sectors - 1]; t < number; t++) {
						sector_list[sectors] = t;
						sectors++;
						
					}
					
				} else {
					sector_list[sectors] = number;
					sectors++;
					break;
					
				}
				
				packet_offset = 0;
				
			} else {
				packet[packet_offset] = c;
				
			}
			
			packet_offset++;
			
		}
		
	}
	
	uint32* good_sector_list;
	int good_sectors = 0;
	
	for (s = 0; s < ALLOCATION_INDEX_END; s++) {
		if (!FS_in(s, (int*) sector_list, sectors)) {
			good_sector_list[good_sectors] = s;
			
			good_sectors++;
			if (good_sectors > length) break;
			
		}
		
	}
	
	string result;
	
	for (s = 0; s < length; s++) {
		str_append(result, FS_write_number_u32(good_sector_list[s]));
		str_append(result, ",");
		
	}
	
	str_append(result, "e");
	return result;
	
}

// sector = 0000,0002-0005 is what we refer to as 0000-0002,0005

string FS_read_file(string sector) {
	uint32* sector_list;
	int sectors = 0;
	
	char command;
	int number;
	
	char packet[4];
	int packet_offset = 0;
	
	int t;
	int c;
	for (c = 0; c < 512; c++) {
		if (packet_offset > 4) {
			number = FS_read_number_u32(packet);
			command = sector[packet_offset];
			
			if (command == ',') {
				sector_list[sectors] = number;
				sectors++;
				
			} else if (command == '-') {
				for (t = sector_list[sectors - 1]; t < number; t++) {
					sector_list[sectors] = t;
					sectors++;
					
				}
				
			} else {
				sector_list[sectors] = number;
				sectors++;
				break;
				
			}
			
			packet_offset = 0;
			
		} else {
			packet[packet_offset] = c;
			
		}
		
		packet_offset++;
		
	}
	
	string result;
	char temp[512];
	
	int i;
	int g = 0;
	
	boolean getting = TRUE;
	while (getting) {
		strcpy(temp, read28(sector_list[g] + 20 + ALLOCATION_INDEX_END, 512));
		println(itoa(sector_list[g]), 0x0f);
		
		for (i = 0; i < 512; i++) {
			result[512 * g + i] = temp[i];
			
			if (sector[i] == '\0') {
				getting = FALSE;
				break;
				
			}
			
		}
		
		if (!getting) break;
		g++;
		
	}
	
	return result;
	
}

void FS_write_file(string sector, string content) {
	uint32* sector_list;
	int sectors = 0;
	
	char command;
	int number;
	
	char packet[4];
	int packet_offset = 0;
	
	int t;
	int c;
	for (c = 0; c < 512; c++) {
		if (packet_offset > 4) {
			number = FS_read_number_u32(packet);
			command = sector[packet_offset];
			
			if (command == ',') {
				sector_list[sectors] = number;
				sectors++;
				
			} else if (command == '-') {
				for (t = sector_list[sectors - 1]; t < number; t++) {
					sector_list[sectors] = t;
					sectors++;
					
				}
				
			} else {
				sector_list[sectors] = number;
				sectors++;
				break;
				
			}
			
			packet_offset = 0;
			
		} else {
			packet[packet_offset] = c;
			
		}
		
		packet_offset++;
		
	}
	
	char sector_chunk[512];
	
	int i;
	for (i = 0; i < str_len(content); i++) {
		sector_chunk[i % 512] = content[i];
		
		if (!((i + 1) % 512)) {
			write28(sector_list[i / 512] + 20 + ALLOCATION_INDEX_END, sector_chunk, 512);
			flush();
			
		}
		
	}
	
	write28(sector_list[sectors - 1] + 20 + ALLOCATION_INDEX_END, sector_chunk, i % 512);
	flush();
	
}

void FS_new_file(string name, string content) {
	uint32 pos = FS_read_number_u32(read28(19, 4)) * 2;
	
	write28(pos + 20, name, 512);
	flush();
	
	int length = str_len(content) / 512;
	string avalible_sectors = get_avalible_sectors(length);
	
	write28(pos + 21, avalible_sectors, 512);
	flush();
	
	FS_write_file(avalible_sectors, content);
	
	pos += 1;
	write28(19, FS_write_number_u32(pos), 4);
	flush();
	
}
