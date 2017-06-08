/*
 * isr.c
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

#include "isr.h"

void isr_install() {
	set_idt_gate(0, (uint32) isr0);
	set_idt_gate(1, (uint32) isr1);
	set_idt_gate(2, (uint32) isr2);
	set_idt_gate(3, (uint32) isr3);
	set_idt_gate(4, (uint32) isr4);
	set_idt_gate(5, (uint32) isr5);
	set_idt_gate(6, (uint32) isr6);
	set_idt_gate(7, (uint32) isr7);
	set_idt_gate(8, (uint32) isr8);
	set_idt_gate(9, (uint32) isr9);
	set_idt_gate(10, (uint32) isr10);
	set_idt_gate(11, (uint32) isr11);
	set_idt_gate(12, (uint32) isr12);
	set_idt_gate(13, (uint32) isr13);
	set_idt_gate(14, (uint32) isr14);
	set_idt_gate(15, (uint32) isr15);
	set_idt_gate(16, (uint32) isr16);
	set_idt_gate(17, (uint32) isr17);
	set_idt_gate(18, (uint32) isr18);
	set_idt_gate(19, (uint32) isr19);
	set_idt_gate(20, (uint32) isr20);
	set_idt_gate(21, (uint32) isr21);
	set_idt_gate(22, (uint32) isr22);
	set_idt_gate(23, (uint32) isr23);
	set_idt_gate(24, (uint32) isr24);
	set_idt_gate(25, (uint32) isr25);
	set_idt_gate(26, (uint32) isr26);
	set_idt_gate(27, (uint32) isr27);
	set_idt_gate(28, (uint32) isr28);
	set_idt_gate(29, (uint32) isr29);
	set_idt_gate(30, (uint32) isr30);
	set_idt_gate(31, (uint32) isr31);
	
	register_idt();
	
}

void isr0() {
	print(exception_messages[0], 0x06);
	asm("hlt");
	
		
}

void isr1() {
	print(exception_messages[1], 0x06);
	asm("hlt");
	
}

void isr2() {
	print(exception_messages[2], 0x06);
	asm("hlt");
	
}

void isr3() {
	print(exception_messages[3], 0x06);
	asm("hlt");
	
}

void isr4() {
	print(exception_messages[4], 0x06);
	asm("hlt");
	
}

void isr5() {
	print(exception_messages[5], 0x06);
	asm("hlt");
	
}

void isr6() {
	print(exception_messages[6], 0x06);
	asm("hlt");
	
}

void isr7() {
	print(exception_messages[7], 0x06);
	asm("hlt");
	
}

void isr8() {
	print(exception_messages[8], 0x06);
	asm("hlt");
	
}

void isr9() {
	print(exception_messages[9], 0x06);
	asm("hlt");
	
}

void isr10() {
	print(exception_messages[10], 0x06);
	asm("hlt");
	
}

void isr11() {
	print(exception_messages[11], 0x06);
	asm("hlt");
	
}

void isr12() {
	print(exception_messages[12], 0x06);
	asm("hlt");
	
}

void isr13() {
	print(exception_messages[13], 0x06);
	asm("hlt");
	
}

void isr14() {
	print(exception_messages[14], 0x06);
	asm("hlt");
	
}

void isr15() {
	print(exception_messages[15], 0x06);
	asm("hlt");
	
}

void isr16() {
	print(exception_messages[16], 0x06);
	asm("hlt");
	
}

void isr17() {
	print(exception_messages[17], 0x06);
	asm("hlt");
	
}

void isr18() {
	print(exception_messages[18], 0x06);
	asm("hlt");
	
}

void isr19() {
	print(exception_messages[19], 0x06);
	asm("hlt");
	
}

void isr20() {
	print(exception_messages[20], 0x06);
	asm("hlt");
	
}

void isr21() {
	print(exception_messages[21], 0x06);
	asm("hlt");
	
}

void isr22() {
	print(exception_messages[22], 0x06);
	asm("hlt");
	
}

void isr23() {
	print(exception_messages[23], 0x06);
	asm("hlt");
	
}

void isr24() {
	print(exception_messages[24], 0x06);
	asm("hlt");
	
}

void isr25() {
	print(exception_messages[25], 0x06);
	asm("hlt");
	
}

void isr26() {
	print(exception_messages[26], 0x06);
	asm("hlt");
	
}

void isr27() {
	print(exception_messages[27], 0x06);
	asm("hlt");
	
}

void isr28() {
	print(exception_messages[28], 0x06);
	asm("hlt");
	
}

void isr29() {
	print(exception_messages[29], 0x06);
	asm("hlt");
	
}

void isr30() {
	print(exception_messages[30], 0x06);
	asm("hlt");
	
}

void isr31() {
	print(exception_messages[31], 0x06);
	asm("hlt");
	
}

string exception_messages[32] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	
};
