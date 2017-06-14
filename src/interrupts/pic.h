/*
 * pic.h
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


#ifndef PIC_H
	#define PIC_H
	
	#include "../types.h"
	#include "../system/system.h"
	
	#define PIC1 0x20
	#define PIC2 0xA0
	
	#define PIC1_COMMAND PIC1
	#define PIC1_DATA (PIC1 + 1)
	#define PIC2_COMMAND PIC2
	#define PIC2_DATA (PIC2 + 1)
	
	#define PIC_EOI 0x20
	
	#define ICW1_ICW4 0x01
	#define ICW1_SINGLE 0x02
	#define ICW1_INTERVAL4 0x04
	#define ICW1_LEVEL 0x08
	#define ICW1_INIT 0x10
	
	#define ICW4_8086 0x01
	#define ICW4_AUTO 0x02
	#define ICW4_BUF_SLAVE 0x08
	#define ICW4_BUF_MASTER 0x0C
	#define ICW4_SFNM 0x10
	
	#define PIC1_CMD 0x20
	#define PIC2_CMD 0xA0
	#define PIC_READ_IRR 0x0a
	#define PIC_READ_ISR 0x0b
	
	void PIC_send_EOI(uint8 irq);
	void PIC_remap(int offset1, int offset2);
	
	void IRQ_set_mask(uint8 IRQ_line);
	void IRQ_clear_mask(uint8 IRQ_line);
	
	uint16 pic_get_irr(void);
	uint16 pic_get_isr(void);
	
#endif
