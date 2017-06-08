/*
 * basic_interpreter.c
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

#include "basic_interpreter.h"

#define VERBOSE 1

#define SCPQT 256
#define SCPSZ 10000

#define PRGSZ 50000
#define LINSZ 256
#define ARGSZ 16
#define VARSZ 17

#define LINES 256
#define ARGUS 20
#define VARIS 512

static void compile(char* program) {
	char lines[LINES][LINSZ];
	
	int line_count = 0;
	int line_buffer_index = 0;
	
	int i;
	for (i = 0; i < str_len(program); i++) {
		if (program[i] == ';') {
			line_buffer_index = 0;
			line_count++;
			
		} else if (program[i] == '\t' || program[i] == '\n') {
			lines[line_count][line_buffer_index] = ' ';
			line_buffer_index++;
			
		} else {
			lines[line_count][line_buffer_index] = program[i];
			line_buffer_index++;
			
		}
		
	}
	
	boolean shift = FALSE;
	int q;
	uint8 key;
	
	char _string = 0;
	char args[ARGUS][ARGSZ];
	int args_buffer_index;
	int args_count = 0;
	
	char var_names[VARIS][VARSZ];
	char var_values[VARIS][ARGSZ];
	int var_count = 0;
	
	int var_ints[VARIS];
	int var_id;
	
	int cond_a;
	int cond_b;
	char cond;
	
	uint8** _api_font_mono_15px = api_get_font("monospace", 15);
	
	int c;
	for (i = 0; i < line_count; i++) {
		for (c = 0; c < str_len(lines[i]); c++) {
			if (lines[i][c] == ' ' && !_string && args_buffer_index) {
				args_buffer_index = 0;
				args_count++;
				
			} else if (lines[i][c] == 'c' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = ';';
				args_buffer_index++;
				
			} else if (lines[i][c] == 'n' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = '\n';
				args_buffer_index++;
				
			} else if (lines[i][c] == 't' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = '\t';
				args_buffer_index++;
				
			} else if (lines[i][c] == '"' && lines[i][c - 1] != '\\') {
				if (_string) _string = 0;
				else _string = 1;
				
			} else if ((lines[i][c] != '\\' || lines[i][c - 1] == '\\') && (lines[i][c] != ' ' || _string)) {
				args[args_count][args_buffer_index] = lines[i][c];
				args_buffer_index++;
				
			}
			
		}
		
		if (strcmp(args[0], "var") == 0 || strcmp(args[0], "new") == 0) {
			if (strcmp(args[2], "is") == 0 || strcmp(args[2], "=") == 0 || strcmp(args[2], "<-") == 0) {
				strcpy(var_names[var_count], args[1]);
				strcpy(var_values[var_count], args[3]);
				
				var_ints[var_count] = atoi(args[3]);
				var_count++;
				
			} else {
				if (VERBOSE) print("COMPILER: ERROR, no value was provided.\n", 0x0f);
				
			}
			
		}
		
		for (c = 0; c < args_count + 1; c++) {
			if (args[c][0] == '$') {
				int v;
				for (v = 0; v < var_count; v++) {
					if (strcmp(var_names[v], args[c]) == 0) {
						var_id = v;
						strcpy(args[c], var_values[v]);
						
					}
					
				}
				
			}
			
		}
		
		if (strcmp(args[0], "if") == 0 || strcmp(args[0], "?") == 0) {
			if (strcmp(args[1], "time") == 0) cond_a = compile_time_string();
			else if (args[1][0] == '\'') cond_a = args[1][1];
			else cond_a = atoi(args[1]);
			
			if (strcmp(args[3], "time") == 0) cond_b = compile_time_string();
			else if (args[3][0] == '\'') cond_b = args[3][1];
			else cond_b = atoi(args[3]);
			
			if (strcmp(args[2], "=") == 0 || strcmp(args[2], "==") == 0) cond = cond_a == cond_b;
			else if (strcmp(args[2], "!") == 0 || strcmp(args[2], "!=") == 0) cond = cond_a != cond_b;
			
			else if (strcmp(args[2], "<=") == 0) cond = cond_a <= cond_b;
			else if (strcmp(args[2], ">=") == 0) cond = cond_a >= cond_b;
			
			else if (strcmp(args[2], "<") == 0) cond = cond_a < cond_b;
			else if (strcmp(args[2], ">") == 0) cond = cond_a > cond_b;
			else if (VERBOSE) print("COMPILER: WARNING, condition is unknown. Will use the previous.\n", 0x0f);
			
			if (cond) {
				int x;
				for (x = 0; x < ARGUS; x++) {
					int y;
					for (y = 0; y < ARGSZ; y++) {
						args[x][y] = args[x + 4][y];
						
					}
					
				}
				
			}
			
		} if (strcmp(args[0], "if-str") == 0 || strcmp(args[0], "?-str") == 0 || strcmp(args[0], "if-string") == 0 || strcmp(args[0], "?-string") == 0) {
			if (strcmp(args[2], "=") == 0 || strcmp(args[2], "==") == 0) cond = strcmp(args[1], args[3]) == 0;
			else if (strcmp(args[2], "!") == 0 || strcmp(args[2], "!=") == 0) cond = strcmp(args[1], args[3]) != 0;
			else if (VERBOSE) print("COMPILER: WARNING, string comparator is unknown. Will use the previous.\n", 0x0f);
			
			if (cond) {
				int x;
				for (x = 0; x < ARGUS; x++) {
					int y;
					for (y = 0; y < ARGSZ; y++) {
						args[x][y] = args[x + 4][y];
						
					}
					
				}
				
			}
			
		}
		
		if (strcmp(args[0], "graphics") == 0) {
			if (strcmp(args[1], "init") == 0) {
				hide_cursor();
				
			}
			
			else if (strcmp(args[1], "wash") == 0 || strcmp(args[1], "fill") == 0) GFX_wash_hex(GFX_compile_rgb_to_hex(atoi(args[2]), atoi(args[3]), atoi(args[4])));
			else if (strcmp(args[1], "rect") == 0) GFX_fill_rect_hex(atoi(args[2]), atoi(args[3]), atoi(args[4]), atoi(args[5]), GFX_compile_rgb_to_hex(atoi(args[6]), atoi(args[7]), atoi(args[8])));
			else if (strcmp(args[1], "line") == 0) GFX_draw_line(atoi(args[2]), atoi(args[3]), atoi(args[4]), atoi(args[5]), GFX_compile_rgb_to_hex(atoi(args[6]), atoi(args[7]), atoi(args[8])));
			else if (strcmp(args[1], "text") == 0 || strcmp(args[1], "locate") == 0 || strcmp(args[1], "print") == 0) GFX_blit_text_hex(atoi(args[3]), atoi(args[4]), 0, 130, _api_font_mono_15px, args[2], GFX_compile_rgb_to_hex(atoi(args[5]), atoi(args[6]), atoi(args[7])));
			
			else if (strcmp(args[1], "width") == 0) {
				var_ints[var_id] = GFX_get_mode_info('w');
				strcpy(var_values[var_id], itoa(var_ints[var_id]));
				
			} else if (strcmp(args[1], "height") == 0) {
				var_ints[var_id] = GFX_get_mode_info('h');
				strcpy(var_values[var_id], itoa(var_ints[var_id]));
				
			}
			
		} else if (strcmp(args[0], "event") == 0) {
			if (strcmp(args[1], "key") == 0 || strcmp(args[1], "key-down") == 0 || strcmp(args[1], "key-press") == 0) var_ints[var_id] = poll_key();
			else if (strcmp(args[1], "key-up") == 0 || strcmp(args[1], "key-release") == 0) var_ints[var_id] = poll_key_release();
			
			else if (strcmp(args[1], "key-map") == 0) {
				if (strcmp(args[2], "true") == 0) shift = TRUE;
				else shift = FALSE;
				
				var_ints[var_id] = get_letter_from_map(var_ints[var_id], shift);
				
			} else if (strcmp(args[1], "mouse-down") == 0 || strcmp(args[1], "mouse-press") == 0) {
				update_mouse_cursor();
				var_ints[var_id] = poll_mouse_press();
				
			} else if (strcmp(args[1], "mouse-click") == 0 || strcmp(args[1], "mouse-up") == 0 || strcmp(args[1], "mouse-release") == 0) {
				update_mouse_cursor();
				var_ints[var_id] = poll_mouse_release();
				
			} else if (strcmp(args[1], "mouse-x") == 0) {
				update_mouse_cursor();
				var_ints[var_id] = get_mouse_x();
				
			} else if (strcmp(args[1], "mouse-y") == 0) {
				update_mouse_cursor();
				var_ints[var_id] = get_mouse_y();
				
			}
			
			strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		} else if (strcmp(args[0], "system") == 0) {
			if (strcmp(args[1], "off") == 0) ACPI_poweroff();
			
			else if (strcmp(args[1], "port") == 0) {
				if (strcmp(args[2], "out") == 0) outportb(atoi(args[3]), atoi(args[4]));
				else if (strcmp(args[2], "in") == 0) var_ints[var_id] = inportb(atoi(args[3]));
				
				strcpy(var_values[var_id], itoa(var_ints[var_id]));
				
			}
			
		} else if (strcmp(args[0], "disk") == 0) {
			if (strcmp(args[1], "write") == 0) write28(atoi(args[2]), args[3], atoi(args[4]));
			else if (strcmp(args[1], "read") == 0) strcpy(var_values[var_id], read28(atoi(args[2]), atoi(args[3])));
			else if (strcmp(args[1], "flush") == 0) flush();
			
		} else if (strcmp(args[0], "serial") == 0) {
			if (strcmp(args[1], "port") == 0) {
				if (strcmp(args[2], "COM1") == 0) set_serial_port(COM1);
				if (strcmp(args[2], "COM2") == 0) set_serial_port(COM2);
				if (strcmp(args[2], "COM3") == 0) set_serial_port(COM3);
				if (strcmp(args[2], "COM4") == 0) set_serial_port(COM4);
				
			} else if (strcmp(args[1], "init") == 0) {
				init_serial();
				
			} else if (strcmp(args[1], "read") == 0) {
				var_values[var_id][0] = read_serial();
				var_values[var_id][1] = '\0';
				
			} else if (strcmp(args[1], "write") == 0) {
				int w;
				for (w = 0; w < str_len(args[2]); w++) {
					write_serial(args[2][w]);
					
				}
				
			}
			
		} else if (strcmp(args[0], "do") == 0) {
			cond_a = atoi(args[1]);
			cond_b = atoi(args[3]);
			
			if (strcmp(args[2], "+") == 0) var_ints[var_id] = cond_a + cond_b;
			else if (strcmp(args[2], "-") == 0) var_ints[var_id] = cond_a - cond_b;
			else if (strcmp(args[2], "*") == 0) var_ints[var_id] = cond_a * cond_b;
			else if (strcmp(args[2], "/") == 0) var_ints[var_id] = cond_a / cond_b;
			else if (strcmp(args[2], "%") == 0 || strcmp(args[2], "mod") == 0) var_ints[var_id] = cond_a % cond_b;
			
			else if (strcmp(args[2], "<<") == 0 || strcmp(args[2], "left") == 0) var_ints[var_id] = cond_a << cond_b;
			else if (strcmp(args[2], ">>") == 0 || strcmp(args[2], "right") == 0) var_ints[var_id] = cond_a >> cond_b;
			
			else if (strcmp(args[2], "or") == 0 || strcmp(args[2], "|") == 0) var_ints[var_id] = cond_a | cond_b;
			else if (strcmp(args[2], "xor") == 0 || strcmp(args[2], "^") == 0) var_ints[var_id] = cond_a ^ cond_b;
			else if (strcmp(args[2], "and") == 0 || strcmp(args[2], "&") == 0) var_ints[var_id] = cond_a & cond_b;
			else if (VERBOSE) print("COMPILER: WARNING, operation is unknown. Variable will not be affected.\n", 0x0f);
			
		} else if (strcmp(args[0], "||") == 0 || strcmp(args[0], "abs") == 0) {
			if (var_ints[var_id] < 0) var_ints[var_id] = -var_ints[var_id];
			strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		}
		
		else if (strcmp(args[0], "++") == 0) {
			var_ints[var_id]++;
			strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		} else if (strcmp(args[0], "--") == 0) {
			var_ints[var_id]--;
			strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		}
		
		else if ((strcmp(args[0], "=") == 0 || strcmp(args[0], "put") == 0) && (strcmp(args[2], "to") == 0 || strcmp(args[2], "->") == 0)) {
			if (strcmp(args[1], "time") == 0) var_ints[var_id] = compile_time_string();
			
			else {
				var_ints[var_id] = atoi(args[1]);
				strcpy(var_values[var_id], args[1]);
				
			}
			
		} else if ((strcmp(args[0], "type") == 0 || strcmp(args[0], "trans") == 0) && (strcmp(args[2], "to") == 0 || strcmp(args[2], "->") == 0)) {
			if (strcmp(args[3], "integer") == 0 || strcmp(args[3], "int") == 0) var_ints[var_id] = atoi(var_values[var_id]);
			if (strcmp(args[3], "string") == 0 || strcmp(args[3], "str") == 0) strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		} else if (strcmp(args[0], "print") == 0 || strcmp(args[0], "log") == 0) {
			if (strcmp(args[2], "as") == 0) {
				if (strcmp(args[3], "integer") == 0 || strcmp(args[3], "int") == 0) print(itoa(var_ints[var_id]), 0x0f);
				if (strcmp(args[3], "string") == 0 || strcmp(args[3], "str") == 0) print(var_values[var_id], 0x0f);
				
			} else print(args[1], 0x0f);
			
		} else if (strcmp(args[0], "read") == 0) {
			q = 0;
			while (TRUE) {
				key = poll_key();
				
				if (key != 127) {
					if (key == 28) {
						break;
						
					} else {
						var_values[var_id][q] = get_letter_from_map(key, FALSE);
						q++;
						
					}
					
				}
				
			}
			
			var_values[var_id][q] = '\0';
			var_ints[var_id] = atoi(var_values[var_id]);
			
		} else if (strcmp(args[0], "sleep") == 0) {
			sleep(atoi(args[1]));
			
		} else if (strcmp(args[0], "time") == 0) {
			//print("%d", (int) compile_time_string());
			var_ints[var_id] = compile_time_string();
			strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		} else if (strcmp(args[0], "rand") == 0 || strcmp(args[0], "random") == 0) {
			var_ints[var_id] = rand_int(atoi(args[1]), atoi(args[2]));
			strcpy(var_values[var_id], itoa(var_ints[var_id]));
			
		} else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "stop") == 0) {
			if (VERBOSE) print("\n\nCOMPILER: Exit command received.\n", 0x0f);
			//GFX_wash_hex(0);
			//break;
			
		} else if (strcmp(args[0], "run") == 0 || strcmp(args[0], "exec") == 0 || strcmp(args[0], "goto") == 0) {
			i = atoi(args[1]) - 1;
			
		} else if (strcmp(args[0], "open") == 0) {
			launch_application(args[1]);
			
		}
		
		_string = 0;
		args_buffer_index = 0;
		args_count = 0;
		
		int x;
		for (x = 0; x < ARGUS; x++) {
			int y;
			for (y = 0; y < ARGSZ; y++) {
				args[x][y] = 0;
				
			}
			
		}
		
	}
	
}

void basic_run(string code) {
	print("AQUA script compiler version 1.0\n\n", 0x0f);
	compile(code);
	if (VERBOSE) print("\n\nCOMPILER: End.\n", 0x0f);
	
}

/*
static char scopes[SCPQT][SCPSZ];
static char scope_names[SCPQT][ARGSZ];
static int scope_count = 0;

static char var_names[VARIS][VARSZ];
static char var_values[VARIS][ARGSZ];
static int var_count = 0;

static int var_ints[VARIS];
static int var_id;

static int compile(char program[PRGSZ], unsigned char using_scopes) {
	char lines[LINES][LINSZ];
	int line_count = 0;
	int line_buffer_index = 0;
	
	int i;
	for (i = 0; i < str_len(program); i++) {
		if (program[i] == ';' || program[i] == '{' || program[i] == '}') {
			line_buffer_index = 0;
			line_count++;
			
		} else if (program[i] == '\t' || program[i] == '\n') {
			lines[line_count][line_buffer_index] = ' ';
			line_buffer_index++;
			
		} else {
			lines[line_count][line_buffer_index] = program[i];
			line_buffer_index++;
			
		}
		
	}
	
	char _string = 0;
	char args[ARGUS][ARGSZ];
	int args_buffer_index;
	int args_count = 0;
	
	int cond_a;
	int cond_b;
	char cond;
	
	int scope_length = 0;
	int scope_base = 0;
	int scope_temp_count = 0;
	
	int c;
	for (i = 0; i < line_count; i++) {
		for (c = 0; c < str_len(lines[i]); c++) {
			if (lines[i][c] == ' ' && !_string && args_buffer_index) {
				args_buffer_index = 0;
				args_count++;
				
			} else if (lines[i][c] == 'c' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = ';';
				args_buffer_index++;
				
			} else if (lines[i][c] == 'e' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = '}';
				args_buffer_index++;
				
			} else if (lines[i][c] == 's' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = '{';
				args_buffer_index++;
				
			} else if (lines[i][c] == 'n' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = '\n';
				args_buffer_index++;
				
			} else if (lines[i][c] == 't' && lines[i][c - 1] == '\\') {
				args[args_count][args_buffer_index] = '\t';
				args_buffer_index++;
				
			} else if (lines[i][c] == '"' && lines[i][c - 1] != '\\') {
				if (_string) _string = 0;
				else _string = 1;
				
			} else if ((lines[i][c] != '\\' || lines[i][c - 1] == '\\') && (lines[i][c] != ' ' || _string)) {
				args[args_count][args_buffer_index] = lines[i][c];
				args_buffer_index++;
				
			}
			
		}
		
		if (using_scopes) {
			if (strcmp(args[0], "scope") == 0 || strcmp(args[0], "sub") == 0 || strcmp(args[0], "function") == 0) {
				strcpy(scope_names[scope_count], args[1]);
				scope_count++;
				
				scope_length = 0;
				scope_temp_count = 0;
				
				int a;
				for (a = 0; a < str_len(program); a++) {
					if (scope_temp_count == scope_count) {
						if (program[a] == '}') {
							break;
							
						}
						
						scopes[scope_count - 1][scope_length] = program[a];
						scope_length++;
						
					} else if (program[a] == '{') {
						scope_temp_count++;
						
					}
					
				} 
				
			} else if (strcmp(args[0], "name") == 0) {
				
				
			} else if (strcmp(args[0], "tab") == 0) {
				
				
			} else if (strcmp(args[0], "colour") == 0 || strcmp(args[0], "color")) {
				
				
			}
			
		} else {
			if (strcmp(args[0], "var") == 0 || strcmp(args[0], "new") == 0) {
				if (strcmp(args[2], "is") == 0 || strcmp(args[2], "=") == 0 || strcmp(args[2], "<-") == 0) {
					strcpy(var_names[var_count], args[1]);
					strcpy(var_values[var_count], args[3]);
					
					var_ints[var_count] = atoi(args[3]);
					var_count++;
					
				} else {
					if (VERBOSE) println("BASIC COMPILER: ERROR, no value was provided.", 0x06);
					return 1;
					
				}
				
			}
			
			for (c = 0; c < args_count + 1; c++) {
				if (args[c][0] == '$') {
					int v;
					for (v = 0; v < var_count; v++) {
						if (strcmp(var_names[v], args[c]) == 0) {
							var_id = v;
							strcpy(args[c], var_values[v]);
							
						}
						
					}
					
				}
				
			}
			
			if (strcmp(args[0], "if") == 0 || strcmp(args[0], "?") == 0) {
				if (strcmp(args[1], "time") == 0) cond_a = compile_time_string();
				else cond_a = atoi(args[1]);
				
				if (strcmp(args[3], "time") == 0) cond_b = compile_time_string();
				else cond_b = atoi(args[3]);
				
				if (strcmp(args[2], "=") == 0 || strcmp(args[2], "==") == 0) cond = cond_a == cond_b;
				else if (strcmp(args[2], "!") == 0 || strcmp(args[2], "!=") == 0) cond = cond_a != cond_b;
				
				else if (strcmp(args[2], "<=") == 0) cond = cond_a <= cond_b;
				else if (strcmp(args[2], ">=") == 0) cond = cond_a >= cond_b;
				
				else if (strcmp(args[2], "<") == 0) cond = cond_a < cond_b;
				else if (strcmp(args[2], ">") == 0) cond = cond_a > cond_b;
				else if (VERBOSE) println("BASIC COMPILER: WARNING, condition is unknown. Will use the previous.", 0x06);
				
				if (cond) {
					int x;
					for (x = 0; x < ARGUS; x++) {
						int y;
						for (y = 0; y < ARGSZ; y++) {
							args[x][y] = args[x + 4][y];
							
						}
						
					}
					
				}
				
			} if (strcmp(args[0], "if-str") == 0 || strcmp(args[0], "?-str") == 0 || strcmp(args[0], "if-string") == 0 || strcmp(args[0], "?-string") == 0) {
				if (strcmp(args[2], "=") == 0 || strcmp(args[2], "==") == 0) cond = strcmp(args[1], args[3]) == 0;
				else if (strcmp(args[2], "!") == 0 || strcmp(args[2], "!=") == 0) cond = strcmp(args[1], args[3]) != 0;
				else if (VERBOSE) println("BASIC COMPILER: WARNING, string comparator is unknown. Will use the previous.", 0x06);
				
				if (cond) {
					int x;
					for (x = 0; x < ARGUS; x++) {
						int y;
						for (y = 0; y < ARGSZ; y++) {
							args[x][y] = args[x + 4][y];
							
						}
						
					}
					
				}
				
			}
			
			if (strcmp(args[0], "graphics") == 0) {
				if (strcmp(args[1], "wash") == 0 || strcmp(args[0], "fill") == 0) {
					GFX_wash_rgb(atoi(args[2]), atoi(args[3]), atoi(args[4]));
					
				}
				
			}
			
			else if (strcmp(args[0], "do") == 0) {
				cond_a = atoi(args[1]);
				cond_b = atoi(args[3]);
				
				if (strcmp(args[2], "+") == 0) var_ints[var_id] = cond_a + cond_b;
				else if (strcmp(args[2], "-") == 0) var_ints[var_id] = cond_a - cond_b;
				else if (strcmp(args[2], "*") == 0) var_ints[var_id] = cond_a * cond_b;
				else if (strcmp(args[2], "/") == 0) var_ints[var_id] = cond_a / cond_b;
				else if (strcmp(args[2], "%") == 0 || strcmp(args[2], "mod") == 0) var_ints[var_id] = cond_a % cond_b;
				
				else if (strcmp(args[2], "<<") == 0 || strcmp(args[2], "left") == 0) var_ints[var_id] = cond_a << cond_b;
				else if (strcmp(args[2], ">>") == 0 || strcmp(args[2], "right") == 0) var_ints[var_id] = cond_a >> cond_b;
				
				else if (strcmp(args[2], "or") == 0 || strcmp(args[2], "|") == 0) var_ints[var_id] = cond_a | cond_b;
				else if (strcmp(args[2], "xor") == 0 || strcmp(args[2], "^") == 0) var_ints[var_id] = cond_a ^ cond_b;
				else if (strcmp(args[2], "and") == 0 || strcmp(args[2], "&") == 0) var_ints[var_id] = cond_a & cond_b;
				else if (VERBOSE) println("BASIC COMPILER: WARNING, operation is unknown. Variable will not be affected.", 0x06);
				
			} else if (strcmp(args[0], "||") == 0 || strcmp(args[0], "abs") == 0) {
				if (var_ints[var_id] < 0) var_ints[var_id] = -var_ints[var_id];
				
			}
			
			else if (strcmp(args[0], "++") == 0) var_ints[var_id]++;
			else if (strcmp(args[0], "--") == 0) var_ints[var_id]--;
			
			else if ((strcmp(args[0], "=") == 0 || strcmp(args[0], "put") == 0) && (strcmp(args[2], "to") == 0 || strcmp(args[2], "->") == 0)) {
				if (strcmp(args[1], "time") == 0) var_ints[var_id] = compile_time_string();
				
				else {
					var_ints[var_id] = atoi(args[1]);
					strcpy(var_values[var_id], args[1]);
					
				}
				
			} else if ((strcmp(args[0], "type") == 0 || strcmp(args[0], "trans") == 0) && (strcmp(args[2], "to") == 0 || strcmp(args[2], "->") == 0)) {
				if (strcmp(args[3], "integer") == 0 || strcmp(args[3], "int") == 0) var_ints[var_id] = atoi(var_values[var_id]);
				if (strcmp(args[3], "string") == 0 || strcmp(args[3], "str") == 0) strcpy(var_values[var_id], itoa(var_ints[var_id]));
				
			} else if (strcmp(args[0], "print") == 0) {
				if (strcmp(args[2], "as") == 0) {
					if (strcmp(args[3], "integer") == 0 || strcmp(args[3], "int") == 0) print(itoa(var_ints[var_id]), 0x0f);
					if (strcmp(args[3], "string") == 0 || strcmp(args[3], "str") == 0) print(var_values[var_id], 0x0f);
					
				} else print(args[1], 0x0f);
				
			} else if (strcmp(args[0], "read") == 0) {
				//gets(var_values[var_id]);
				var_ints[var_id] = atoi(var_values[var_id]);
				
			} else if (strcmp(args[0], "sleep") == 0) {
				sleep(atoi(args[1]));
				
			} else if (strcmp(args[0], "time") == 0) {
				print(itoa(compile_time_string()), 0x0f);
				
			} else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "stop") == 0 || strcmp(args[0], "end") == 0) {
				if (VERBOSE) println("\n\nCOMPILER: Exit command received.", 0x07);
				return 0;
				
			} else if (strcmp(args[0], "run") == 0 || strcmp(args[0], "exec") == 0 || strcmp(args[0], "goto") == 0) {
				int s;
				for (s = 0; s < scope_count; s++) {
					if (strcmp(scope_names[s], args[1]) == 0) {
						compile(scopes[s], 0);
						
					}
					
				}
				
			}
			
		}
		
		_string = 0;
		args_buffer_index = 0;
		args_count = 0;
		
		int x;
		for (x = 0; x < ARGUS; x++) {
			int y;
			for (y = 0; y < ARGSZ; y++) {
				args[x][y] = 0;
				
			}
			
		}
		
	}
	
	return 1;
	
}

void basic_run(string code) {
	if (VERBOSE) println("AQUA script compiler version 1.1\n", 0x07);
	
	char program[PRGSZ];
	strcpy(program, code);
	
	compile(program, 1);
	
	int s;
	for (s = 0; s < scope_count; s++) {
		if (strcmp(scope_names[s], "main") == 0) {
			compile(scopes[s], 0);
			
		}
		
	}
	
	if (VERBOSE) println("\n\nCOMPILER: End.", 0x07);
	
}*/
