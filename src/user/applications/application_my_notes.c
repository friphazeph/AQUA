/*
 * application_my_notes.c
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

#include "application_my_notes.h"

static uint32 width;
static uint32 height;

static char* lines;

/*"\n\
graphics init;\n\
\n\
new $fric is 0;\n\
new $mouse is;\n\
new $key is;\n\
\n\
\tsleep 1;\n\
\tgraphics wash 15 230 255;\n\
\tgraphics text \"money:\" 1 1 255 255 255;\n\
\n\
\ttrans $fric to int;\n\
\tif $mouse = 1\n\
\t\t++ $fric;\n\
\tevent mouse-click $mouse;\n\
\tevent key $key;\n\
\tevent key-map $key;\n\
\n\
\t\n\
\tif $mouse = 2\n\
\t\tgoto 15;\n\
\n\
\tgraphics text $fric 91 1 255 255 255;\n\
\n\
goto 4;\n\
\n\
\tsleep 1;\n\
\tgraphics wash 15 230 255;\n\
\tgraphics text \"slaves (1):\" 1 1 255 255 255;\n\
\tgraphics text \"better slaves (2):\" 1 15 255 255 255;\n\
\tgraphics text \"Nb of slaves:\" 1 30 255 255 255;\n\
\t\n\
\tevent mouse-click $mouse;\n\
\t\n\
\tif $mouse = 1\n\
\t\tgoto 4;\n\
\t\n\
\tgoto 15;\n\
\n\
exit;\n\
";

/*"\
graphics init;\n\
new $value is 0;\n\
\n\
\t++ $value;\n\
\ttrans $value to str;\n\
\tgraphics wash 255 255 255;\n\
\tgraphics text $value 10 10 0 0 0;\n\
\tsleep 1;\n\
\n\
\tif $value < 5\n\
\t\tgoto 2;\n\
\n\
exit;\n\
";*/

/*"\
graphics init;\n\
new $key is;\n\
new $end_when is;\n\
new $x is;\n\
new $y is;\n\
new $x1 is;\n\
new $y1 is;\n\
new $r is;\n\
new $g is;\n\
new $b is;\n\
graphics wash 0 0 0;\n\
graphics print \"This is a demo\" 350 250 255 255 255;\n\
sleep 3;\n\
graphics wash 255 0 255;\n\
graphics print \"Welcome\" 100 100 0 255 0;\n\
sleep 1;\n\
graphics print \"to\" 100 125 255 255 0;\n\
sleep 1;\n\
graphics print \"AQUA!\" 100 150 0 255 255;\n\
sleep 1;\n\
put 0 to $end_when;\n\
graphics wash 0 0 0;\n\
graphics print \"RANDOM LINES\" 350 250 255 255 255;\n\
\t++ $end_when;\n\
\tif $end_when < 1\n\
\t\tsleep 1;\n\
\t\t? # # # goto 23;\n\
\trandom 0 800 $x;\n\
\trandom 0 800 $x1;\n\
\trandom 0 600 $y;\n\
\trandom 0 600 $y1;\n\
\trandom 0 255 $r;\n\
\trandom 0 255 $g;\n\
\trandom 0 255 $b;\n\
\tgraphics line $x $y $x1 $y1 $r $g $b;\n\
graphics wash 255 255 255;\n\
graphics print \"Press A to restart\" 350 250 0 0 0;\n\
sleep 5;\n\
event key $key;\n\
event key-map false $key;\n\
if $key = 'a'\n\
\tgoto 3;\n\
exit;\n\
";*/
static int temp_ln = 0;
static int temp_nl = 0;

static uint32 background = 0x00292929;
static uint32 foreground = 0x00FFFFFF;

static uint32 syntax[5] = {
	0x00D69CD6, // main
	0x0092BDE8, // sub-main
	0x00EBBF71, // strings
	0x00CFA300, // numbers & macros
	0x00FF939C, // variables
	
};

void blit_lines(uint8* temp_buffer, uint8* buffer, uint8** font) {
	temp_buffer = GFX_blit_image_to_ptr8(temp_buffer, width, 0, 0, width, height, buffer);
	string temp = "\0\0";
	
	int x = 0;
	int xx = 0;
	
	int y;
	for (y = 0; y < str_len(lines); y++) {
		if (lines[y] == '\n') {
			x++;
			xx = 0;
			
		} else if (lines[y] == '\t') {
			xx += 4;
			
		} else {
			temp[0] = lines[y];
			temp_buffer = GFX_blit_text_hex_to_ptr8(temp_buffer, width, 20 + xx * 10, 20 + x * 20, 0, 130, font, temp, 0x00FFFFFF);
			xx++;
			
		}
		
	}
	
}

void launch_application_my_notes(void) {
	uint8* _api_icon_back = api_get_icon("back");
	uint8* _api_icon_save = api_get_icon("save");
	uint8* _api_icon_load = api_get_icon("load");
	uint8** _api_font_mono_15px = api_get_font("monospace", 15);
	
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
	uint8* desktop = (uint8*) kmalloc(width * height * 3);
	desktop = GFX_compile_section_to_ptr8(desktop, 0, 0, width, height);
	
	uint8* buffer = (uint8*) kmalloc(width * height * 3);
	uint8* temp_buffer = (uint8*) kmalloc(width * height * 3);
	
	buffer = GFX_fill_rect_hex_to_ptr8(buffer, width, 0, 0, width, height, 0x00292929); // replcae with background
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, width - 30, height - 30, 20, 20, _api_icon_back, 0x0000FF00);
	
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, 10, height - 30, 20, 20, _api_icon_save, 0x00C40E6C);
	buffer = GFX_blit_icon_hex_to_ptr8(buffer, width, 50, height - 30, 20, 20, _api_icon_load, 0x00C48F0E);
	
	blit_lines(temp_buffer, buffer, _api_font_mono_15px);
	buffer = GFX_blit_image_to_ptr8(buffer, width, 0, 0, width, height, temp_buffer);
	
	hide_cursor();
	UI_animate_desktop("fade", 30, buffer);
	show_cursor();
	
	lines = (char*) kmalloc(32768);
	lines[0] = '\0';
	temp_ln = 0;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	uint8 key;
	boolean shift = FALSE;
	
	boolean running = TRUE;
	while (running) {
		key = poll_key();
		update_mouse_cursor();
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (key != 127) {
			if (key == 1) {
				running = FALSE;
				break;
				
			}
			
			if (key == 42 || key == 54) {
				shift = TRUE;
				
			}
			
			else if (key == 75) temp_ln--;
			else if (key == 77) temp_ln++;
			
			else {
				if (get_letter_from_map(key, shift) == '\b') {
					temp_ln--;
					lines[temp_ln] = '\0';
					
				} else {
					temp_ln++;
					lines[temp_ln - 1] = get_letter_from_map(key, shift);
					lines[temp_ln] = '\0';
					
				}
				
				blit_lines(temp_buffer, buffer, _api_font_mono_15px);
				GFX_blit_image(0, 0, width, height, temp_buffer);
				
				shift = FALSE;
				
			}
			
		}
		
		if (mx > width - 40 && my > height - 40 && mr == 1) {
			running = FALSE;
			break;
			
		} else if (mx < 40 && my > height - 40 && mr == 1) {
			FS_write_program(20, lines);
			flush();
			
		} else if (mx < 80 && my > height - 40 && mr == 1) {
			kfree(lines, 32768);
			lines = FS_read_program(20);
			temp_ln = str_len(lines);
			
			blit_lines(temp_buffer, buffer, _api_font_mono_15px);
			GFX_blit_image(0, 0, width, height, temp_buffer);
			
		} else if (mr == 1 || key == 29) {
			basic_run(lines);
			blit_lines(temp_buffer, buffer, _api_font_mono_15px);
			GFX_blit_image(0, 0, width, height, temp_buffer);
			
		}
		
	}
	
	hide_cursor();
	UI_animate_desktop("fade", 30, desktop);
	
	kfree(desktop, width * height * 3);
	kfree(buffer, width * height * 3);
	kfree(temp_buffer, width * height * 3);
	kfree(lines, 32768);
	
}
