/*
 * app_launcher.c
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

#include "app_launcher.h"

void launch_application(string name) {
	if (str_eql(name, "Utilities")) launch_application_utils();
	if (str_eql(name, "Draw")) launch_application_draw();
	if (str_eql(name, "Settings")) launch_application_shell();
	
	if (str_eql(name, "Random laser game")) launch_application_lasergame();
	if (str_eql(name, "Ant")) launch_application_ant();
	
	if (str_eql(name, "Log")) launch_application_log();
	if (str_eql(name, "About")) launch_application_about();
	
	if (str_eql(name, "My notepad")) launch_application_my_notes();
	
	if (str_eql(name, "My application")) {
		uint32 width = GFX_get_mode_info('w');
		uint32 height = GFX_get_mode_info('h');
		
		uint8* desktop = (uint8*) kmalloc(width * height * 3);
		desktop = GFX_compile_section_to_ptr8(desktop, 0, 0, width, height);
		basic_run(FS_read_program(20));
		
		GFX_blit_image(0, 0, width, height, desktop);
		kfree(desktop, width * height * 3);
		
	}
	
}
