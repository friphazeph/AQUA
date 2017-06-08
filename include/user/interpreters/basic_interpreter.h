/*
 * basic_interpreter.h
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

#ifndef BASIC_INTERPRETER_H
	#define BASIC_INTERPRETER_H
	
	#include "../applications/api.h"
	#include "../app_launcher.h"
	
	#include "../../types.h"
	#include "../../screen.h"
	
	#include "../../utils/time.h"
	#include "../../utils/math.h"
	#include "../../utils/utils.h"
	#include "../../utils/string.h"
	
	#include "../../system/system.h"
	#include "../../memory/memory.h"
	
	#include "../../graphics/GFX.h"
	
	#include "../../drivers/ATA_driver.h"
	#include "../../drivers/ACPI_driver.h"
	#include "../../drivers/serial_driver.h"
	
	void basic_run(string code);
	
#endif
