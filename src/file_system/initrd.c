/*
 * initrd.c
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


#include "initrd.h"

static initrd_header* _initrd_header;
static initrd_file_header* file_headers;

static fs_node_t* initrd_root;
static fs_node_t* initrd_dev;
static fs_node_t* root_nodes;

static int root_node_count;
static struct dirent _dirent;

static uint32 initrd_read(fs_node_t* node, uint32 offset, uint32 _size, uint8* buffer) {
	initrd_file_header header = file_headers[node->inode];
	
	if (offset > header.length) return 0;
	if (offset + _size > header.length) _size = header.length - offset;
	
	memcpy(buffer, (uint8*) (header.offset + offset), _size);
	return _size;
	
}

static struct dirent* initrd_readdir(fs_node_t* node, uint32 index) {
	if (node == initrd_root && index == 0) {
		strcpy(_dirent.name, "dev");
		_dirent.name[3] = 0;
		_dirent.inode_number = 0;
		return &_dirent;
		
	} if (index - 1 >= root_node_count) {
		return 0;
		
	}
	
	strcpy(_dirent.name, root_nodes[index - 1].name);
	_dirent.name[strlen(root_nodes[index - 1].name)] = 0;
	_dirent.inode_number = root_nodes[index - 1].inode;
	return &_dirent;
	
}

static fs_node_t* initrd_finddir(fs_node_t* node, char* name) {
	if (node == initrd_root && !strcmp(name, "dev")) {
		return initrd_dev;
		
	}
	
	int i;
	for (i = 0;	i < root_node_count; i++) {
		if (!strcmp(name, root_nodes[i].name)) {
			return &root_nodes[i];
			
		}
		
	}
	
	return 0;
	
}

fs_node_t* init_initrd(uint32 location) {
	_initrd_header = (initrd_header*) location;
	file_headers = (initrd_file_header*) (location + sizeof(initrd_header));
	
	initrd_root = (fs_node_t*) kmalloc(sizeof(fs_node_t));
	strcpy(initrd_root->name, "initrd");
	
	initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
	initrd_root->flags = FS_DIRECTORY;
	
	initrd_root->read = 0;
	initrd_root->write = 0;
	initrd_root->open = 0;
	initrd_root->close = 0;
	
	initrd_root->readdir = &initrd_readdir;
	initrd_root->finddir = &initrd_finddir;
	
	initrd_root->pointer = 0;
	initrd_root->implementation = 0;
	
	initrd_dev = (fs_node_t*) kmalloc(sizeof(fs_node_t));
	strcpy(initrd_dev->name, "dev");
	
	initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;
	initrd_dev->flags = FS_DIRECTORY;
	
	initrd_dev->read = 0;
	initrd_dev->write = 0;
	initrd_dev->open = 0;
	initrd_dev->close = 0;
	
	initrd_dev->readdir = &initrd_readdir;
	initrd_dev->finddir = &initrd_finddir;
	
	initrd_dev->pointer = 0;
	initrd_dev->implementation = 0;
	
	root_nodes = (fs_node_t*) kmalloc(sizeof(fs_node_t) * _initrd_header->file_count);
	root_node_count = _initrd_header->file_count;
	
	int i;
	for (i = 0; i < _initrd_header->file_count; i++) {
		file_headers[i].offset += location;
		strcpy(root_nodes[i].name, &file_headers[i].name);
		
		root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
		root_nodes[i].length = file_headers[i].length;
		
		root_nodes[i].inode = i;
		root_nodes[i].flags = FS_FILE;
		root_nodes[i].read = &initrd_read;
		
		root_nodes[i].write = 0;
		root_nodes[i].readdir = 0;
		root_nodes[i].finddir = 0;
		root_nodes[i].open = 0;
		root_nodes[i].close = 0;
		root_nodes[i].implementation = 0;
		
	}
	
	return initrd_root;
	
}
