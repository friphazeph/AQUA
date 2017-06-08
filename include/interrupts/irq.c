/*
 * irq.c
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


#include "irq.h"

void* irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
	
};

void irq_add_handler(int irq, void (*handler) (struct registers* r)) {
	irq_routines[irq] = handler;
	
}

void irq_remove_handler(int irq) {
	irq_routines[irq] = 0;
	
}

void irq_remap(void) {
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);
	
}

void irq_init(void) {
	irq_remap();
	
	set_idt_gate(32, (unsigned) irq0);
	set_idt_gate(33, (unsigned) irq1);
	set_idt_gate(34, (unsigned) irq2);
	set_idt_gate(35, (unsigned) irq3);
	set_idt_gate(36, (unsigned) irq4);
	set_idt_gate(37, (unsigned) irq5);
	set_idt_gate(38, (unsigned) irq6);
	set_idt_gate(39, (unsigned) irq7);
	set_idt_gate(40, (unsigned) irq8);
	set_idt_gate(41, (unsigned) irq9);
	set_idt_gate(42, (unsigned) irq10);
	set_idt_gate(43, (unsigned) irq11);
	set_idt_gate(44, (unsigned) irq12);
	set_idt_gate(45, (unsigned) irq13);
	set_idt_gate(46, (unsigned) irq14);
	set_idt_gate(47, (unsigned) irq15);
	
}

void irq_handler(struct registers* r) {
	void (*handler) (struct registers* r);
	handler = irq_routines[r->num - 32];
	
	if (handler) {
		handler(r);
		
	} if (r->num >= 40) {
		outportb(0xA0, 0x20);
		
	}
	
	outportb(0x20, 0x20);
	
}
