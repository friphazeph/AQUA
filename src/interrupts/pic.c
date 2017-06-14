/*
 * pic.c
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

#include "pic.h"

void PIC_send_EOI(uint8 irq) {
	if (irq >= 8) {
		outportb(PIC2_COMMAND, PIC_EOI);
		
	}
	
	outportb(PIC1_COMMAND, PIC_EOI);
	
}

void PIC_remap(int offset1, int offset2) {
	uint8 a1 = inportb(PIC1_DATA);
	uint8 a2 = inportb(PIC2_DATA);
	
	outportb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);
	outportb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	
	outportb(PIC1_DATA, offset1);
	outportb(PIC2_DATA, offset2);
	
	outportb(PIC1_DATA, 4);
	outportb(PIC2_DATA, 2);
	
	outportb(PIC1_DATA, ICW4_8086);
	outportb(PIC2_DATA, ICW4_8086);
	
	outportb(PIC1_DATA, a1);
	outportb(PIC2_DATA, a2);
	
}

void IRQ_set_mask(uint8 IRQ_line) {
	uint16 port;
	uint8 value;
	
	if (IRQ_line < 8) {
		port = PIC1_DATA;
		
	} else {
		port = PIC2_DATA;
		IRQ_line -= 8;
		
	}
	
	value = inportb(port) | (1 << IRQ_line);
	outportb(port, value);
	
}

void IRQ_clear_mask(uint8 IRQ_line) {
	uint16 port;
	uint8 value;
	
	if (IRQ_line < 8) {
		port = PIC1_DATA;
		
	} else {
		port = PIC2_DATA;
		IRQ_line -= 8;
		
	}
	
	value = inportb(port) & ~(1 << IRQ_line);
	outportb(port, value);
	
}

static uint16 __pic_get_irq_reg(int ocw3) {
	outportb(PIC1_CMD, ocw3);
	outportb(PIC2_CMD, ocw3);
	
	return (inportb(PIC2_CMD) << 8) | inportb(PIC1_CMD);
	
}

uint16 pic_get_irr(void) {
	return __pic_get_irq_reg(PIC_READ_IRR);
	
}

uint16 pic_get_isr(void) {
	return __pic_get_irq_reg(PIC_READ_ISR);
	
}
