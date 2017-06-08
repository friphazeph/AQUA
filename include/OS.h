
#ifndef OS_H
	#define OS_H
	
	#include "types.h"
	#include "multiboot.h"
	
	#include "universal/lang.h"
	
	#include "utils/math.h"
	#include "utils/time.h"
	#include "utils/utils.h"
	#include "utils/string.h"
	
	#include "system/gdt.h"
	#include "system/system.h"
	
	#include "memory/memory.h"
	#include "memory/paging.h"
	
	#include "graphics/UI.h"
	#include "graphics/GFX.h"
	#include "graphics/aqua3d.h"
	
	#include "graphics/effects/graphics_blurring.h"
	
	#include "interrupts/isr.h"
	#include "interrupts/idt.h"
	#include "interrupts/pic.h"
	#include "interrupts/irq.h"
	
	#include "specs/settings.h"
	#include "specs/RAM_count.h"
	
	#include "file_system/AQUA_fs.h"
	
	#include "user/cursor_task.h"
	#include "user/widget_page.h"
	#include "user/app_launcher.h"
	
	#include "user/interpreters/basic_interpreter.h"
	
	#include "drivers/PIT_driver.h"
	#include "drivers/ATA_driver.h"
	#include "drivers/CMOS_driver.h"
	#include "drivers/ACPI_driver.h"
	#include "drivers/AC97_driver.h"
	#include "drivers/serial_driver.h"
	
	#include "drivers/mouse_driver.h"
	#include "drivers/keyboard_driver.h"
	
	void OS_run(\
		unsigned long magic, \
		unsigned long addr, \
		\
		uint8* wallpaper0, \
		uint8* wallpaper2, \
		uint8* wallpaper5, \
		uint8* wallpaper7, \
		\
		uint16 font_aqua_15px_l, \
		uint8 font_aqua_15px[94][font_aqua_15px_l], \
		uint16 font_aqua_20px_l, \
		uint8 font_aqua_20px[94][font_aqua_20px_l], \
		uint16 font_aqua_50px_l, \
		uint8 font_aqua_50px[94][font_aqua_50px_l], \
		uint16 font_mono_15px_l, \
		uint8 font_mono_15px[94][font_mono_15px_l], \
		\
		uint8* icon_off, \
		uint8* icon_back, \
		uint8* icon_next, \
		uint8* icon_edit, \
		uint8* icon_circle, \
		\
		uint8* icon_save, \
		uint8* icon_load \
		\
	);
	
#endif
