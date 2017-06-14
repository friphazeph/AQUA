/*
 * irq.h
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


#ifndef IRQ_H
	#define IRQ_H
	
	#include "../types.h"
	#include "../screen.h"
	#include "../system/system.h"
	
	#include "idt.h"
	#include "../drivers/PIT_driver.h"
	
	extern void irq0();
	extern void irq1();
	extern void irq2();
	extern void irq3();
	extern void irq4();
	extern void irq5();
	extern void irq6();
	extern void irq7();
	extern void irq8();
	extern void irq9();
	extern void irq10();
	extern void irq11();
	extern void irq12();
	extern void irq13();
	extern void irq14();
	extern void irq15();
	
	void irq_add_handler(int irq, void (*handler) (struct registers* r));
	void irq_remove_handler(int irq);
	
	void irq_remap(void);
	void irq_init(void);
	void irq_handler(struct registers* r);
	
#endif
