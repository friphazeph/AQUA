
#include "sleep.h"

static int font_length;
static uint8** font;

static uint32 width;
static uint32 height;

void init_sleep(int _font_length, uint8** _font) {
	font_length = _font_length;
	font = _font;
	
	width = GFX_get_mode_info('w');
	height = GFX_get_mode_info('h');
	
}

void enter_sleep_mode(void) {
	string time_string;
	
	hide_cursor();
	__asm__ __volatile__("cli");
	
	uint8* old_buffer = (uint8*) kmalloc(width * height * 3);
	old_buffer = GFX_compile_section_to_ptr8(old_buffer, 0, 0, width, height);
	
	float s = 0;
	
	uint f;
	for (f = 0; f < 70; f++) {
		s += (s / 10.0f) + 0.2f;
		GFX_fill_rect_hex(0, 0, width, (height - ((int) ((float) height / (s + 1.0f)))), 0x00000000);
		
	}
	
	int timer = 0;
	inportb(0x64);
	
	boolean sleeping = TRUE;
	while (sleeping) {
		if (!(timer % 60)) {
			if (!timer) {
				timer = CMOS_get_time('s');
				
			}
			
			time_string = format_time();
			
			uint8* blitter = (uint8*) kmalloc(width * 300);
			uint8* old_blitter = (uint8*) kmalloc(width * 300);
			
			old_blitter = GFX_compile_section_to_ptr8(old_blitter, 0, 0, width, 300);
			
			uint16 a;
			for (a = 0; a < 127; a += 5) {
				blitter = GFX_blit_image_to_ptr8(blitter, width, 0, 0, width, 100, old_blitter);
				blitter = GFX_blit_text_hex_to_ptr8(blitter, width, 50, 0, 0, font_length, font, time_string, GFX_compile_rgb_to_hex((uint8) a, (uint8) a, (uint8) a));
				
				GFX_blit_image(0, 100, width, 100, blitter);
				
			}
			
			kfree(blitter, width * 300);
			kfree(old_blitter, width * 300);
			
		}
		
		sleep(1);
		timer++;
		
		if (inportb(0x64) & 0x1) {
			__asm__ __volatile__("sti");
			sleeping = FALSE;
			break;
			
		}
		
	}
	
	UI_animate_desktop("slide_up", 1, old_buffer);
	kfree(old_buffer, width * height * 3);
	show_cursor();
	
}
