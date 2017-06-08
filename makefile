#  makefile
#  
#  Copyright 2017 Obiwac
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  


COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm

VERSION = "AQUA OS"

CFLAGS = -m32 -c -ffreestanding
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T src/link.ld

EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -kernel

OBJS = obj/kernel_asm.o \
	obj/kernel_c.o \
	\
	obj/OS.o \
	obj/screen.o \
	\
	obj/utils/math.o \
	obj/utils/time.o \
	obj/utils/utils.o \
	obj/utils/string.o \
	obj/utils/endian.o \
	obj/utils/bubble_sort.o \
	\
	obj/system/gdt.o \
	obj/system/system.o \
	\
	obj/universal/lang.o \
	obj/universal/key_map.o \
	\
	obj/specs/settings.o \
	obj/specs/RAM_count.o \
	\
	obj/user/cursor_task.o \
	obj/user/widget_page.o \
	obj/user/start_wizard.o \
	obj/user/app_launcher.o \
	\
	obj/user/compilers/c_compiler.o \
	obj/user/interpreters/basic_interpreter.o \
	\
	obj/memory/heap.o \
	obj/memory/memory.o \
	obj/memory/paging.o \
	\
	obj/graphics/UI.o \
	obj/graphics/GFX.o \
	obj/graphics/aqua3d.o \
	\
	obj/graphics/effects/graphics_blurring.o \
	\
	obj/drivers/PIT_driver.o \
	obj/drivers/ATA_driver.o \
	obj/drivers/CMOS_driver.o \
	obj/drivers/ACPI_driver.o \
	\
	obj/drivers/serial_driver.o \
	\
	obj/pci/pci.o \
	\
	obj/interrupts/idt.o \
	obj/interrupts/isr.o \
	obj/interrupts/irq.o \
	\
	obj/interrupts/pic.o \
	obj/interrupts/nmi.o \
	\
	obj/drivers/mouse_driver.o \
	obj/drivers/keyboard_driver.o \
	\
	obj/file_system/AQUA_fs.o \
	\
	obj/user/applications/api.o \
	\
	obj/user/applications/application_draw.o \
	obj/user/applications/application_shell.o \
	obj/user/applications/application_utils.o \
	\
	obj/user/applications/application_lasergame.o \
	obj/user/applications/application_ant.o \
	\
	obj/user/applications/application_log.o \
	obj/user/applications/application_about.o \
	\
	obj/user/applications/application_my_notes.o \
	\

OUTPUT = aqua/boot/kernel.bin

run: all
	$(EMULATOR) $(EMULATOR_FLAGS) $(OUTPUT)

iso: build
	(killall virtualbox && sleep 1) || true
	virtualbox --startvm $(VERSION)

all: $(OBJS)
	echo "iso" > aqua/aqua.iso
	rm aqua/aqua.iso
	
	mkdir aqua/ -p
	mkdir aqua/boot/ -p
	
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)
	strip aqua/boot/kernel.bin

kernel.bin: src/link.ld $(OBJS)
	ld -melf_i386 -T $< -o $@ $(OBJS)

obj/kernel_asm.o: src/kernel.asm
	$(ASSEMBLER) $(ASMFLAGS) -o obj/kernel_asm.o src/kernel.asm

obj/kernel_c.o: src/kernel.c
	$(COMPILER) $(CFLAGS) -o obj/kernel_c.o src/kernel.c

obj/%.o: include/%.c
	mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -c -o $@ $<

build: all
	rm aqua/boot/grub/ -r -f
	mkdir aqua/boot/grub/
	
	cp src/grub.cfg aqua/boot/grub/grub.cfg
	grub-mkrescue --output=aqua/aqua.iso aqua/

install: kernel.bin
	sudo cp $< aqua/boot/kernel.bin

.PHONY: clean
clean:
	rm -rf obj/*.o
	rm -rf aqua/
