/*
 * app_launcher.h
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

#ifndef APP_LAUNCHER_H
	#define APP_LAUNCHER_H
	
	#include "../types.h"
	
	#include "../file_system/AQUA_fs.h"
	#include "interpreters/basic_interpreter.h"
	
	#include "applications/application_draw.h"
	#include "applications/application_shell.h"
	#include "applications/application_utils.h"
	
	#include "applications/application_lasergame.h"
	#include "applications/application_ant.h"
	
	#include "applications/application_log.h"
	#include "applications/application_about.h"
	
	#include "applications/application_my_notes.h"
	
	void launch_application(string name);
	
#endif
