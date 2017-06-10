/*
 * OS.c
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



#include "OS.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

static uint32 WIDTH;
static uint32 HEIGHT;
static uint8 BPP;
static uint8 CPC;

static boolean IRQ = TRUE;
static boolean buffer_blit = FALSE;

static uint8 boot_disk;

static char dir_buffer1[512];
static char dir_buffer2[512];
/*
static FILE current_dir = {};
static FILE file_buffer = {};

static filesystem temp_fs = {};
static filesystem* _filesystem;*/
static uint8 prog[0x1000];

static boolean drive = TRUE;
static boolean lock_drive = FALSE;

static uint8 place = 0;
static uint32 current_minute;

static uint8* _wallpaper0;
static uint8* _wallpaper2;
static uint8* _wallpaper5;
static uint8* _wallpaper7;
/*
static void progx(void) {
	exec(prog);
	
}

static boolean find_and_execute(char* cmd, boolean wait) {
	FILE* file = kmalloc(sizeof(FILE));
	
	if (_file_system->get_file(cmd, file, _file_system) && !file->is_directory()) {
		if (file->sectors * 512 > 0x1000) {
			println("Executable too large.", 0x06);
			
		} else {
			_file_system->read(file, prog, _file_system);
			process* proc = create_process(cmd, (uint32) progx);
			uint32 pid = add_process(proc);
			while (wait && proc->state == PROCESS_ALIVE);
			
		}
		
		kfree(file, sizeof(FILE));
		return TRUE;
		
	}
	
	return FALSE;
	
}

static void init_ext2_buffers(filesystem* __filesystem) {
	_filesystem = __filesystem;
	_filesystem->get_file("/", &current_dir, _filesystem);
	
	dir_buffer1[0] = '/';
	dir_buffer2[0] = '\0';
	
}*/

static void get_minute_update(uint16 fdl, uint8 font_data[94][fdl]) {
	uint32 new_minute = CMOS_get_time('m');
	
	if (new_minute != current_minute) {
		uint32 hour = CMOS_get_time('h');
		uint32 minute = CMOS_get_time('m');
		
		string time_string = "  :  ";
		string time_string_minute = itoa(current_minute);
		string time_string_hour = itoa(hour);
		
		if (hour < 10) time_string[1] = time_string_hour[0];
		else {
			time_string[0] = time_string_hour[0];
			time_string[1] = time_string_hour[1];
			
		} if (minute < 10) time_string[4] = time_string_minute[0];
		else {
			time_string[3] = time_string_minute[0];
			time_string[4] = time_string_minute[1];
			
		}
		
		uint text_potential_length = GFX_get_text_potential_length(time_string, 0, fdl, font_data) + 10;
		
		if (place == 1) {
			GFX_blit_image_part(WIDTH - 800, 5, WIDTH - 800, 5, 800, 20, WIDTH, 20, _wallpaper0);
			GFX_blit_text_hex(WIDTH - text_potential_length, 5, 0, fdl, font_data, time_string, 0x00FFFFFF);
			
		} else if (place == 2) {
			GFX_blit_image_part(WIDTH - 800, 35, WIDTH - 800, 35, 800, 20, WIDTH, 20, _wallpaper5);
			GFX_blit_text_hex(WIDTH - text_potential_length, 35, 0, fdl, font_data, time_string, 0x00FFFFFF);
			
		}
		
		current_minute = new_minute;
		
	}
	
}

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
) {
	//clear_screen();
	println("GRUB: Booting on Secondary Master ...", 0x0f);
	
	println("GRUB: Parsing multiboot 2 structure ...", 0x0f);
	
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		print("\tGRUB ERROR: Invalid magic number: 0x", 0x06);
		print_hex((unsigned) magic, 0x06);
		printch('\n', 0x06);
		
		while (1);
		
	}
	
	multiboot_info_t* mbi = (multiboot_info_t*) addr;
	
	print("\tGRUB: flags = 0x", 0x07);
	print_hex((unsigned) mbi->flags, 0x07);
	printch('\n', 0x07);
	
	if (CHECK_FLAG(mbi->flags, 0)) {
		print("\tGRUB: mem_lower = ", 0x07);
		print(itoa((unsigned) mbi->mem_lower), 0x07);
		print(", mem_upper = ", 0x07);
		print(itoa((unsigned) mbi->mem_upper), 0x07);
		printch('\n', 0x07);
		
	} if (CHECK_FLAG(mbi->flags, 1)) {
		print("\tGRUB: boot_device = 0x", 0x07);
		print_hex((unsigned) mbi->boot_device, 0x07);
		printch('\n', 0x07);
		
	} if (CHECK_FLAG(mbi->flags, 2)) {
		print("\tGRUB: cmdline = ", 0x07);
		print((char*) mbi->cmdline, 0x07);
		printch('\n', 0x07);
		
	} if (CHECK_FLAG(mbi->flags, 3)) {
		multiboot_module_t* mod;
		
		print("\tGRUB: mods_count = ", 0x07);
		print(itoa((int) mbi->mods_count), 0x07);
		print(", mods_addr = 0x", 0x07);
		print_hex((int) mbi->mods_addr, 0x07);
		printch('\n', 0x07);
		
		int i;
		for (i = 0, mod = (multiboot_module_t*) mbi->mods_addr; i < mbi->mods_count; i++, mod++) {
			print("\t\tmod_start = ", 0x07);
			print(itoa((unsigned) mod->mod_start), 0x07);
			
			print(", mod_end = ", 0x07);
			print(itoa((unsigned) mod->mod_end), 0x07);
			
			print(", cmdline = ", 0x07);
			print((char*) mod->cmdline, 0x07);
			printch('\n', 0x07);
			
		}
		
	} if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
		print("\tGRUB ERROR: Both bits 4 and 5 are set.\n", 0x06);
		while (1);
		
	} if (CHECK_FLAG(mbi->flags, 4)) {
		multiboot_aout_symbol_table_t* multiboot_aout_sym = &(mbi->u.aout_sym);
		
		print("\tGRUB: multiboot_aout_symbol_table: tabsize = 0x", 0x07);
		print_hex((unsigned) multiboot_aout_sym->tabsize, 0x07);
		
		print(", strsize = 0x", 0x07);
		print_hex((unsigned) multiboot_aout_sym->strsize, 0x07);
		
		print(", addr = 0x", 0x07);
		print_hex((unsigned) multiboot_aout_sym->addr, 0x07);
		printch('\n', 0x07);
		
	} if (CHECK_FLAG(mbi->flags, 5)) {
		multiboot_elf_section_header_table_t* multiboot_elf_sec = &(mbi->u.elf_sec);
		
		print("\tGRUB: multiboot_elf_sec: num = ", 0x07);
		print(itoa((unsigned) multiboot_elf_sec->num), 0x07);
		
		print(", size = 0x", 0x07);
		print_hex((unsigned) multiboot_elf_sec->size, 0x07);
		
		print(", addr = 0x", 0x07);
		print_hex((unsigned) multiboot_elf_sec->addr, 0x07);
		
		print(", shndx = 0x", 0x07);
		print_hex((unsigned) multiboot_elf_sec->shndx, 0x07);
		printch('\n', 0x07);
		
	} if (CHECK_FLAG(mbi->flags, 6)) {
		multiboot_memory_map_t* mmap;
		
		print("\tGRUB: mmap_addr = 0x", 0x07);
		print_hex(mbi->mmap_addr, 0x07);
		
		print(", mmap_length = 0x", 0x07);
		print_hex(mbi->mmap_length, 0x07);
		printch('\n', 0x07);
		
		for (mmap = (multiboot_memory_map_t*) mbi->mmap_addr; (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length; mmap = (multiboot_memory_map_t*) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))) {
			print("\t\tsize = 0x", 0x07);
			print_hex((unsigned) mmap->size, 0x07);
			
			add_mmap_size((unsigned) mmap->size);
			
			print(", base_addr = 0x", 0x07);
			print_hex(mmap->addr >> 32, 0x07);
			print_hex(mmap->addr & 0xffffffff, 0x07);
			
			print(", length = 0x", 0x07);
			print_hex(mmap->len >> 32, 0x07);
			print_hex(mmap->len & 0xffffffff, 0x07);
			
			print(", type = 0x", 0x07);
			print_hex((unsigned) mmap->type, 0x07);
			printch('\n', 0x07);
			
		}
		
	}
	
	println("GRUB: Setting up framebuffer information ...", 0x0f);
	
	WIDTH = mbi->framebuffer_width;
	HEIGHT = mbi->framebuffer_height;
	BPP = mbi->framebuffer_bpp;
	CPC = BPP / 8;
	
	println("Memory management: Initializing heap ...", 0x0f);
	init_heap();
	
	println("Loading the loading screen ...", 0x07);
	
	string temp = "AQUA";
	
	uint32 hex;
	uint32 x = (WIDTH / 2) - 72;
	uint32 y = (HEIGHT / 2) - 25;
	
	uint32 double_buffer_size = WIDTH * HEIGHT * 3;
	uint8* loading_buffer = (uint8*) kmalloc(double_buffer_size);
	
	GFX_init(WIDTH, HEIGHT, BPP, mbi->framebuffer_addr, buffer_blit);
	UI_init(font_aqua_20px_l, font_aqua_20px);
	
	//dawn_boot();
	
	int b;
	for (b = 0; b < 255; b += 30) {
		hex = GFX_compile_rgb_to_hex(b, b, b);
		
		loading_buffer = GFX_fill_rect_hex_to_ptr8(loading_buffer, WIDTH, 0, 0, WIDTH, HEIGHT, hex);
		loading_buffer = GFX_blit_text_hex_to_ptr8(loading_buffer, WIDTH, x, y, 25, font_aqua_50px_l, font_aqua_50px, temp, 0x00000000);
		GFX_blit_image(0, 0, WIDTH, HEIGHT, loading_buffer);
		
	}
	
	kfree(loading_buffer, double_buffer_size);
	
	print("System: Initializing ACPI driver in RAM ...\n\t", 0x0f);
	ACPI_init();
	
	print("System: Setting seed for PRNG based on the compiled time string: ", 0x0f);
	println((string) itoa(compile_time_string()), 0x0f);
	rand_seed(compile_time_string());
	
	println("System: Loading Global Descriptor Table ...", 0x0f);
	load_gdt();
	
	println("Initializing mouse cursor ...", 0x0f);
	init_cursor(buffer_blit);
	
	println("Interrupts: Installing ISR ...", 0x0f);
	isr_install();
	
	println("Interrupts: Installing PIT handler on IRQ0 ...", 0x0f);
	
	if (IRQ) {
		timer_install(buffer_blit);
		
		if (buffer_blit) timer_phase(100000000);
		else timer_phase(1);
		
	}
	
	println("Interrupts: Installing keyboard handler on IRQ1 ...", 0x0f);
	keyboard_install(IRQ);
	
	println("Interrupts: Installing mouse handler on IRQ12 ...", 0x0f);
	if (IRQ) mouse_install();
	
	if (IRQ) {
		println("Interrupts: Initializing IRQs ...", 0x0f);
		irq_init();
		
		println("Interrupts: Storing interrupt flags ...", 0x0f);
		__asm__ __volatile__("sti");
		
	}
	
	println("Memory management: Setting up paging ...", 0x0f);
	//setup_paging();
	
	println("\nSerial: Making sure serial port is est to COM1 ...", 0x0f);
	set_serial_port(COM1);
	print_hex(COM1, 0x0f);
	
	println("\nSerial: Initializing serial port ...", 0x0f);
	init_serial();
	
	println("PCI: Scanning for devices ...", 0x0f);
	PCI_debug();
	
	println("Detecting RAM ...", 0x0f);
	
	uint8* warning;
	uint8 key;
	
	int32 mx;
	int32 my;
	uint8 mp;
	uint8 mr;
	
	/*hide_cursor();
	UI_animate_desktop("fade", 15, loading_buffer);
	kfree(loading_buffer, double_buffer_size);
	show_cursor();
	*/
	println("Passing necessary rescorces to application programming interface ...", 0x0f);
	
	api_rescorces_init(
		icon_off, \
		icon_back, \
		icon_save, \
		icon_load, \
		icon_next, \
		\
		(uint8**) font_mono_15px, \
		(uint8**) font_aqua_50px, \
		(uint8**) font_aqua_20px \
		\
	);
	
	println("Loading wallpapers ...", 0x0f);
	
	int widget_pages = 4;
	
	uint32 wallpaper_colour = 0x00FFFFFF;
	uint32 text_colour = 0x00FFFFFF;
	
	uint32 off_colour = 0x00FF0000;
	uint32 back_colour = 0x0000FF00;
	uint32 next_colour = 0x00FFFF00;
	
	_wallpaper0 = (uint8*) kmalloc(double_buffer_size);
	_wallpaper2 = (uint8*) kmalloc(double_buffer_size);
	_wallpaper5 = (uint8*) kmalloc(double_buffer_size);
	_wallpaper7 = (uint8*) kmalloc(double_buffer_size);
	
	_wallpaper0 = GFX_blit_image_to_ptr8(_wallpaper0, WIDTH, 0, 0, WIDTH, HEIGHT, GFX_scale_ptr8(wallpaper0, 800, 600, WIDTH, HEIGHT));
	_wallpaper2 = GFX_blit_image_to_ptr8(_wallpaper2, WIDTH, 0, 0, WIDTH, HEIGHT, GFX_scale_ptr8(wallpaper2, 800, 600, WIDTH, HEIGHT));
	_wallpaper5 = GFX_blit_image_to_ptr8(_wallpaper5, WIDTH, 0, 0, WIDTH, HEIGHT, GFX_scale_ptr8(wallpaper5, 800, 600, WIDTH, HEIGHT));
	_wallpaper7 = GFX_blit_image_to_ptr8(_wallpaper7, WIDTH, 0, 0, WIDTH, HEIGHT, GFX_scale_ptr8(wallpaper7, 800, 600, WIDTH, HEIGHT));
	
	println("Passing necessary rescorces to the widget page ...", 0x0f);
	
	init_widget_page(
		WIDTH, \
		HEIGHT, \
		\
		_wallpaper7, \
		\
		icon_off, \
		icon_back, \
		icon_edit, \
		\
		(uint8**) font_aqua_20px, \
		(uint8**) font_aqua_50px \
		\
	);
	
	println("Passing necessary rescorces to the start wizard ...", 0x0f);
	
	init_start_wizard(
		_wallpaper2, \
		icon_next, \
		\
		font_aqua_20px \
		\
	);
	
	println("ATA: Setting up Advanced Technoligy Attachment at 0x1F0 ...", 0x0f);
	ata_setup(FALSE, 0x1F0);
	
	println("ATA: Identifying ...", 0x0f);
	//identify();
	
	println("FS: Initializing ...", 0x0f);
	//FS_reformat();
	
	//FS_new_file("Pepe", "My name is jeff.");
	/*string pepe = read28(20 + ALLOCATION_INDEX_END, 512);//FS_read_file(FS_find_file("Pepe"));
	pepe[5] = 0;
	GFX_blit_text_hex(0, 0, 0, 439, font_aqua_20px, pepe, 0x00FF00FF);*/
	//sleep(50);
	
	lock_drive = init_fs();
	
	if (lock_drive) {
		println("\tFS WARNING: File system is not correctly formatted. Locking drive ...", 0x06);
		
		enter_start_wizard();
		launch_application("Settings");
		
	}
	
	load_settings();
	
	if (get_total_RAM() / 1048576 < 512 && get_setting(0) == 'f') {
		println("WARNING, you do not have enough RAM.", 0x06);
		
		warning = (uint8*) kmalloc(WIDTH * HEIGHT * 3);
		
		warning = GFX_fill_rect_hex_to_ptr8(warning, WIDTH, 0, 0, WIDTH, HEIGHT, 0x00292929);
		warning = GFX_blit_text_hex_to_ptr8(warning, WIDTH, 40, 20, 0, font_aqua_50px_l, font_aqua_50px, translate("Warning:"), 0x00FF0000);
		warning = GFX_blit_text_hex_to_ptr8(warning, WIDTH, 10, 90, 0, font_aqua_20px_l, font_aqua_20px, translate("You do not have enough RAM."), 0x00FF0000);
		
		temp = translate("You have");
		//str_append(temp, " ");
		//str_append(temp, /*itoa(get_total_RAM() / 1048576)*/"61");
		//str_append(temp, " ");
		//str_append(temp, translate("mebibytes of RAM."));
		
		warning = GFX_blit_text_hex_to_ptr8(warning, WIDTH, 10, 120, 0, font_aqua_20px_l, font_aqua_20px, temp, 0x00FFFFFF);
		warning = GFX_blit_text_hex_to_ptr8(warning, WIDTH, 10, 150, 0, font_aqua_20px_l, font_aqua_20px, translate("Recommended amount is 512 mebibytes of RAM."), 0x00FFFFFF);
		
		warning = GFX_blit_icon_hex_to_ptr8(warning, WIDTH, 10, HEIGHT - 30, 20, 20, icon_off, 0x00FF0000);
		warning = GFX_blit_icon_hex_to_ptr8(warning, WIDTH, WIDTH - 30, HEIGHT - 30, 20, 20, icon_next, 0x00FFFF00);
		
		hide_cursor();
		
		UI_animate_desktop("slide_up", 1, warning);
		show_cursor();
		
		boolean ramming = TRUE;
		while (ramming) {
			key = poll_key();
			update_mouse_cursor();
			
			mx = get_mouse_x();
			my = get_mouse_y();
			
			mp = poll_mouse_press();
			mr = poll_mouse_release();
			
			if ((mx < 40 && my > HEIGHT - 40 && mr == 1) || key == 1) {
				UI_dialog(font_aqua_20px_l, font_aqua_20px, translate("Are you sure you want to shutdown the computer?"), icon_off, 0x00FF0000, "poweroff", icon_back, 0x0000FF00, "NULL");
				
			} else if (key == 28 || (mx > WIDTH - 40 && my > HEIGHT - 40 && mr == 1)) {
				ramming = FALSE;
				kfree(warning, WIDTH * HEIGHT * 3);
				break;
				
			}
			
		}
		
	}
	
	println("Loading desktop buffers ...", 0x0f);
	
	uint8* login_buffer = (uint8*) kmalloc(double_buffer_size);
	login_buffer = GFX_blit_image_to_ptr8(login_buffer, WIDTH, 0, 0, WIDTH, HEIGHT, _wallpaper0);
	
	login_buffer = GFX_blit_icon_hex_to_ptr8(login_buffer, WIDTH, 10, HEIGHT - 30, 20, 20, icon_off, off_colour);
	
	temp = "AQUA OS";
	login_buffer = GFX_blit_text_hex_to_ptr8(login_buffer, WIDTH, (WIDTH / 2) - (GFX_get_text_potential_length(temp, 25, font_aqua_50px_l, font_aqua_50px) / 2), 100, 25, font_aqua_50px_l, font_aqua_50px, temp, text_colour);
	
	temp = translate("Please enter your password.");
	
	/*temp = translate("Hello");
	str_append(temp, ", ");
	str_append(temp, FS_get_name());
	str_append(temp, "! ");
	str_append(temp, translate("Please enter your password."));
	*/
	login_buffer = GFX_blit_text_hex_to_ptr8(login_buffer, WIDTH, (WIDTH / 2) - (GFX_get_text_potential_length(temp, 0, font_aqua_20px_l, font_aqua_20px) / 2), 200, 0, font_aqua_20px_l, font_aqua_20px, temp, text_colour);
	login_buffer = GFX_blit_icon_hex_to_ptr8(login_buffer, WIDTH, WIDTH - 30, 240, 20, 20, icon_next, next_colour);
	
	uint8* desktop_buffer = (uint8*) kmalloc(double_buffer_size);
	desktop_buffer = GFX_blit_image_to_ptr8(desktop_buffer, WIDTH, 0, 0, WIDTH, HEIGHT, _wallpaper5);
	
	desktop_buffer = GFX_fill_rect_hex_to_ptr8(desktop_buffer, WIDTH, 10, 30, WIDTH - 20, 1, text_colour);
	desktop_buffer = GFX_blit_icon_hex_to_ptr8(desktop_buffer, WIDTH, 10, HEIGHT - 30, 20, 20, icon_off, off_colour);
	
	desktop_buffer = GFX_blit_text_hex_to_ptr8(desktop_buffer, WIDTH, 10, 5, 0, font_aqua_20px_l, font_aqua_20px, translate("Tools"), text_colour);
	desktop_buffer = GFX_blit_text_hex_to_ptr8(desktop_buffer, WIDTH, 10 + ((WIDTH - 20) / 4), 5, 0, font_aqua_20px_l, font_aqua_20px, translate("Games"), text_colour);
	desktop_buffer = GFX_blit_text_hex_to_ptr8(desktop_buffer, WIDTH, 10 + (((WIDTH - 20) / 4) * 2), 5, 0, font_aqua_20px_l, font_aqua_20px, translate("Random"), text_colour);
	desktop_buffer = GFX_blit_text_hex_to_ptr8(desktop_buffer, WIDTH, 10 + (((WIDTH - 20) / 4) * 3), 5, 0, font_aqua_20px_l, font_aqua_20px, translate("My Tab"), text_colour);
	
	desktop_buffer = GFX_fill_rect_hex_to_ptr8(desktop_buffer, WIDTH, (WIDTH / 2) - (5 * widget_pages), HEIGHT - 15, (10 * widget_pages) + 1, 1, text_colour);
	
	int w;
	for (w = 0; w < widget_pages; w++) {
		desktop_buffer = GFX_blit_icon_hex_to_ptr8(desktop_buffer, WIDTH, ((WIDTH / 2) - (5 * widget_pages)) + (10 * w) + 3, HEIGHT - 12, 4, 4, icon_circle, text_colour);
		
	}
	
	uint8* card_desktop_temp_buffer = (uint8*) kmalloc(double_buffer_size);
	card_desktop_temp_buffer = GFX_blit_image_to_ptr8(card_desktop_temp_buffer, WIDTH, 0, 0, WIDTH, HEIGHT, desktop_buffer);
	if (get_setting(1) == 'f') card_desktop_temp_buffer = blur(card_desktop_temp_buffer, WIDTH, HEIGHT, 2);
	
	uint8* card = (uint8*) kmalloc((WIDTH / 10) * (HEIGHT / 10) * 3);
	card = GFX_blit_image_to_ptr8(card, WIDTH / 10, 0, 0, WIDTH, HEIGHT, GFX_scale_ptr8(card_desktop_temp_buffer, WIDTH, HEIGHT, WIDTH / 10, HEIGHT));
	kfree(card_desktop_temp_buffer, double_buffer_size);
	
	hide_cursor();
	UI_animate_desktop("slide_up", 1, login_buffer);
	show_cursor();
	
	boolean code_granted = FALSE;
	
	place = 1;
	current_minute = 61;
	
	boolean running = TRUE;
	while (running) {
		key = poll_key();
		update_mouse_cursor();
		//get_minute_update(font_aqua_20px_l, font_aqua_20px);
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if ((mx < 40 && my > HEIGHT - 40 && mr == 1) || key == 1) {
			UI_dialog(font_aqua_20px_l, font_aqua_20px, translate("Are you sure you want to shutdown the computer?"), icon_off, off_colour, "poweroff", icon_back, back_colour, "NULL");
			
		} else if ((key == 28 || (mx > WIDTH - 40 && my > 230 && my < 270 && mr == 1)) && !code_granted) {
			UI_animate_card("slide_up", 100, card, WIDTH / 10, HEIGHT / 10);
			kfree(card, (WIDTH / 10) * (HEIGHT / 10) * 3);
			code_granted = TRUE;
			
		} else if ((key == 28 || (mx > (WIDTH / 2) - (WIDTH / 20) && mx < (WIDTH / 2) + (WIDTH / 20) && my > HEIGHT - (HEIGHT / 10) && mr == 1)) && code_granted) {
			UI_animate_desktop("slide_up", 1, desktop_buffer);
			widget_set_desktop();
			
			show_cursor();
			break;
			
		}
		
	}
	
	place = 2;
	current_minute = 61;
	
	string command;
	tab_entry_list entries_list[4];
	
	entries_list[0].amount = 3;
	entries_list[1].amount = 2;
	entries_list[2].amount = 2;
	entries_list[3].amount = 2;
	
	entries_list[0].entries[0].name = "Utilities";
	entries_list[0].entries[0].colour = 0x00FFFFFF;
	entries_list[0].entries[1].name = "Draw";
	entries_list[0].entries[1].colour = 0x00FFFFFF;
	entries_list[0].entries[2].name = "Settings";
	entries_list[0].entries[2].colour = 0x00FFFFFF;
	
	entries_list[1].entries[0].name = "Random laser game";
	entries_list[1].entries[0].colour = 0x00FFFFFF;
	entries_list[1].entries[1].name = "Langton's ant";
	entries_list[1].entries[1].colour = 0x00dba087;
	
	entries_list[2].entries[0].name = "About";
	entries_list[2].entries[0].colour = 0x00FFFFFF;
	entries_list[2].entries[1].name = "Log";
	entries_list[2].entries[1].colour = 0x00FFFFFF;
	
	entries_list[3].entries[0].name = "My notepad";
	entries_list[3].entries[0].colour = 0x00FFFFFF;
	entries_list[3].entries[1].name = "My script";
	entries_list[3].entries[1].colour = 0x00FFFFFF;
	
	string stupid_messages[] = {
		"My name is Jeff",
		"I am AQUA",
		"This is Sparta",
		"John cena",
		"Chuck Norris ***chuckle***",
		"ZZzzzzzZZzZZZZ",
		"Introducing, fry",
		
	};
	
	string time_string;
	string week_day;
	string day_string = "IF YOU SEE THIS MESSAGE THEN WTF";
	boolean dragging = FALSE;
	
	while (running) {
		key = poll_key();
		update_mouse_cursor();
		//get_minute_update(font_aqua_20px_l, font_aqua_20px);
		
		mx = get_mouse_x();
		my = get_mouse_y();
		
		mp = poll_mouse_press();
		mr = poll_mouse_release();
		
		if (mx <= 0 && mp == 1) {
			dragging = TRUE;
			
		}
		
		if ((mr != 127 && dragging) || key == 75) {
			dragging = FALSE;
			
			if (mx > WIDTH / 3) {
				hide_cursor();
				
				time_string = format_time();
				strcpy(day_string, itoa(CMOS_get_time('d')));
				
				if (day_string[str_len(day_string) - 1] == '1') day_string = str_append(day_string, translate("st of "));
				else if (day_string[str_len(day_string) - 1] == '2') day_string = str_append(day_string, translate("nd of "));
				else if (day_string[str_len(day_string) - 1] == '3') day_string = str_append(day_string, translate("rd of "));
				else day_string = str_append(day_string, translate("th of "));
				
				day_string = str_append(day_string, translate(get_month_name()));
				
				uint16 time = 1;
				uint32 side_bar_buffer_size = (WIDTH / 3) * HEIGHT * 3;
				
				uint8* side_bar_buffer = (uint8*) kmalloc(side_bar_buffer_size);
				uint8* side_bar_buffer_trace = (uint8*) kmalloc(side_bar_buffer_size);
				
				side_bar_buffer_trace = GFX_compile_section_to_ptr8(side_bar_buffer_trace, 0, 0, WIDTH / 3, HEIGHT);
				
				float s = 0;
				uint32 x;
				
				uint f;
				for (f = 0; f < 70 * time; f += 2) {
					s += ((s / 10.0f) + 0.2f) / time * 2;
					x = (WIDTH / 3) - ((uint32) ((float) (WIDTH / 3) / (s + 1.0f)));
					
					side_bar_buffer = GFX_blit_image_to_ptr8(side_bar_buffer, WIDTH / 3, 0, 0, WIDTH / 3, HEIGHT, side_bar_buffer_trace);
					side_bar_buffer = GFX_fill_rect_hex_to_ptr8_wa(side_bar_buffer, WIDTH / 3, 0, 0, x, HEIGHT, 0x00000000, 40);
					side_bar_buffer = GFX_blit_text_hex_to_ptr8(side_bar_buffer, WIDTH / 3, x + WIDTH / 3 + 20, 20, 0, font_aqua_50px_l, font_aqua_50px, time_string, 0x00FFFFFF);
					side_bar_buffer = GFX_blit_text_hex_to_ptr8(side_bar_buffer, WIDTH / 3, x + WIDTH / 3 + 20, 80, 0, font_aqua_20px_l, font_aqua_20px, "Day of the week", 0x00FFFFFF);
					side_bar_buffer = GFX_blit_text_hex_to_ptr8(side_bar_buffer, WIDTH / 3, x + WIDTH / 3 + 20, 110, 0, font_aqua_20px_l, font_aqua_20px, day_string, 0x00FFFFFF);
					
					GFX_blit_image(0, 0, WIDTH / 3, HEIGHT, side_bar_buffer);
					
				}
				
				kfree(side_bar_buffer, side_bar_buffer_size);
				kfree(side_bar_buffer_trace, side_bar_buffer_size);
				kfree(time_string, 6);
				
				show_cursor();
				
				boolean sliding = TRUE;
				while (sliding) {
					key = poll_key();
					
					update_mouse_cursor();
					
					mx = get_mouse_x();
					my = get_mouse_y();
					
					mp = poll_mouse_press();
					mr = poll_mouse_release();
					
					if (mr == 1) {
						hide_cursor();
						
						sliding = FALSE;
						break;
						
					}
					
				}
				
				UI_animate_desktop("fade", 60, desktop_buffer);
				show_cursor();
				
			}
			
		}
		
		if (key == 57) {
			command = UI_entry(font_aqua_20px_l, font_aqua_20px, translate("Run dialog. Enter a command:"), icon_next, 0x00FFFF00);
			
			if (strcmp(command, "shutdown") == 0) UI_turn_off();
			else if (strcmp(command, "sleep") == 0) {
				UI_sleep(stupid_messages[rand_int(0, 6)], font_aqua_50px_l, font_aqua_50px);
				
			}
			else if (strcmp(command, "crash") == 0) asm("int $18");
			else launch_application(command);
			
		}
		
		if ((mx < 40 && my > HEIGHT - 40 && mr == 1) || key == 1) {
			UI_dialog(font_aqua_20px_l, font_aqua_20px, translate("Are you sure you want to shutdown the computer?"), icon_off, off_colour, "poweroff", icon_back, back_colour, "NULL");
			
		} else if ((my > HEIGHT - 20 && mr == 1) || key == 80) {
			enter_widget_page();
			
		} if (my < 30 && mr == 1) {
			if (mx < 10 + ((WIDTH - 20) / 4)) UI_slide_down(font_aqua_15px_l, font_aqua_15px, 0, entries_list[0].amount, entries_list[0].entries);
			else if (mx < 10 + (((WIDTH - 20) / 4) * 2)) UI_slide_down(font_aqua_15px_l, font_aqua_15px, 1, entries_list[1].amount, entries_list[1].entries);
			else if (mx < 10 + (((WIDTH - 20) / 4) * 3)) UI_slide_down(font_aqua_15px_l, font_aqua_15px, 2, entries_list[2].amount, entries_list[2].entries);
			else UI_slide_down(font_aqua_15px_l, font_aqua_15px, 3, entries_list[3].amount, entries_list[3].entries);
			
		}
		
	}
	
	kfree(login_buffer, double_buffer_size);
	kfree(desktop_buffer, double_buffer_size);
	
	kfree(_wallpaper0, double_buffer_size);
	kfree(_wallpaper2, double_buffer_size);
	kfree(_wallpaper5, double_buffer_size);
	kfree(_wallpaper7, double_buffer_size);
	
	ACPI_poweroff();
	
}
