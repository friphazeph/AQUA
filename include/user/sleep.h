
#ifndef SLEEP_H
	#define SLEEP_H
	
	#include "../types.h"
	#include "cursor_task.h"
	
	#include "../graphics/UI.h"
	#include "../graphics/GFX.h"
	
	#include "../utils/time.h"
	#include "../drivers/CMOS_driver.h"
	
	#include "../system/system.h"
	#include "../memory/memory.h"
	
	void init_sleep(int _font_length, uint8** _font);
	void enter_sleep_mode(void);
	
#endif
