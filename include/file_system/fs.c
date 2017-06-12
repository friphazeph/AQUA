/*
 * fs.c
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


#include "fs.h"

fs_node_t* fs_root = 0;

uint32 read_fs(fs_node_t* node, uint32 offset, uint32 _size, uint8* buffer) {
	if (node->read != 0) return node->read(node, offset, _size, buffer);
	else return 0;
	
}

uint32 write_fs(fs_node_t* node, uint32 offset, uint32 _size, uint8* buffer) {
	if (node->read != 0) return node->read(node, offset, _size, buffer);
	else return 0;
	
}

uint32 open_fs(fs_node_t* node, uint8 read, uint8 write) {
	if (node->open != 0) {
		return node->open(node);
		
	}
	
}

uint32 close_fs(fs_node_t* node) {
	if (node->close != 0) {
		return node->close(node);
		
	}
	
}

struct dirent* readdir_fs(fs_node_t* node, uint32 index) {
	if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0) return node->readdir(node, index);
	else return 0;
	
}


fs_node_t* finddir_fs(fs_node_t* node, char* name) {
	if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0) return node->readdir(node, name);
	else return 0;
	
}
