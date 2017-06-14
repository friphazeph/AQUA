/*
 * fs.h
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


#ifndef FS_H
	#define FS_H
	
	#include "../types.h"
	
	#define FS_FILE 0x01
	#define FS_DIRECTORY 0x02
	
	#define FS_CHARDEVICE 0x03
	#define FS_BLOCKDEVICE 0x04
	
	#define FS_PIPE 0x05
	#define FS_SYMLINK 0x06
	#define FS_MOUNTPOINT 0x08
	
	struct fs_node;
	
	typedef uint32 (*read_type) (struct fs_node*, uint32, uint32, uint8*);
	typedef uint32 (*write_type) (struct fs_node*, uint32, uint32, uint8*);
	
	typedef void (*open_type) (struct fs_node*);
	typedef void (*close_type) (struct fs_node*);
	
	typedef struct dirent* (*readdir_type) (struct fs_node*, uint32);
	typedef struct fs_node* (*finddir_type) (struct fs_node*, uint32);
	
	typedef struct fs_node {
		char name[128];
		uint32 mask;
		
		uint32 uid;
		uint32 gid;
		
		uint32 flags;
		uint32 inode;
		uint32 length;
		uint32 implementation;
		
		read_type read;
		write_type write;
		
		open_type open;
		close_type close;
		
		readdir_type readdir;
		finddir_type finddir;
		
		struct fs_node* pointer;
		
	} fs_node_t;
	
	struct dirent {
		char name[128];
		uint32 inode_number;
		
	};
	
	extern fs_node_t* fs_root;
	
	uint32 read_fs(fs_node_t* node, uint32 offset, uint32 _size, uint8* buffer);
	uint32 write_fs(fs_node_t* node, uint32 offset, uint32 _size, uint8* buffer);
	
	uint32 open_fs(fs_node_t* node, uint8 read, uint8 write);
	uint32 close_fs(fs_node_t* node);
	
	struct dirent* readdir_fs(fs_node_t* node, uint32 index);
	fs_node_t* finddir_fs(fs_node_t* node, char* name);
	
#endif
