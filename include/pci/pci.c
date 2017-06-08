/*
 * pci.c
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

#include "pci.h"

uint16 PCI_read_word(uint8 bus, uint8 slot, uint8 func, uint8 offset) {
	uint32 lbus = (uint32) bus;
	uint32 lslot = (uint32) slot;
	uint32 lfunc = (uint32) func;
	
	uint32 address = (uint32) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32) 0x80000000));
	outportl(0xCF8, address);
	uint16 tmp = (uint16) ((inportl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	
	return tmp;
	
}

uint16 get_PCI_vendor(uint8 bus, uint8 slot, uint8 function) {
	uint16 vendor = PCI_read_word(bus, slot, function, 0);
	uint16 device;
	
	if (vendor != 0xFFFF) {
		device = PCI_read_word(bus, slot, 0, 2);
		
	}
	
	return vendor;
	
}

PCI_device get_PCI_device(uint8 tclass, uint8 tsub_class, uint8 tprog_IF) {
	uint16 class_and_sub;
	uint8 class;
	uint8 sub_class;
	uint8 prog_IF;
	
	PCI_device pci_device = {.bus = 0, .slot = 0, .flags = 0};
	boolean found_device = FALSE;
	
	int bus;
	int device;
	for (bus = 0; bus < 256; bus++) {
		for (device = 0; device < 32; device++) {
			if (get_PCI_vendor(bus, device, 0) != 0xFFFF) {
				class_and_sub = PCI_read_word(bus, device, 0, 10);
				class = (uint8) (class_and_sub >> 8);
				sub_class = (uint8) class_and_sub;
				prog_IF = (uint8) (PCI_read_word(bus, device, 0, 8) >> 8);
				
				if (class == tclass && sub_class == tsub_class && prog_IF == tprog_IF) {
					if (!found_device) {
						pci_device.bus = bus;
						pci_device.slot = device;
						pci_device.flags |= 0b00000001;
						
					} else {
						pci_device.flags |= 0b00000010;
						
					}
					
				}
				
			}
			
		}
		
	}
	
	return pci_device;
	
}

uint32 PCI_make_addr(uint16 bus, uint16 slot, uint16 func, uint16 offset) {
	uint32 lbus = (uint32) bus;
	uint32 lslot = (uint32) slot;
	uint32 lfunc = (uint32) func;
	
	return (uint32) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32) 0x80000000));
	
}

void PCI_write_long(uint16 bus, uint16 slot, uint16 func, uint16 offset, uint32 data) {
	uint32 address = PCI_make_addr(bus, slot, func, offset);
	outportl(PCI_DATA_PORT, address);
	outportl(PCI_COMMAND_PORT, data);
	
}

void PCI_debug(void) {
	println("PCI: Checking all PCI buses ...", 0x0f);
	
	int bus;
	int device;
	for (bus = 0; bus < 256; bus++) {
		for (device = 0; device < 32; device++) {
			if (get_PCI_vendor(bus, device, 0) != 0xFFFF) {
				print("\tPCI: Found ", 0x07);
				
				uint16 class_and_sub = PCI_read_word(bus, device, 0, 10);
				print_PCI_class_code((uint8) (class_and_sub >> 8), (uint8) class_and_sub, (uint8) (PCI_read_word(bus, device, 0, 8) >> 8));
				
				print(" at 0x", 0x07);
				print_hex(bus, 0x07);
				
				print(", 0x", 0x07);
				print_hex(device, 0x07);
				
				if (((uint8) (PCI_read_word(bus, device, 0, 14) & 0xFF)) & 0x80 != 0) {
					println(" with multiple functions.", 0x07);
					
				}
				
			}
			
		}
		
	}
	
}

void print_PCI_class_code(uint8 class_code, uint8 sub_class, uint8 prog_IF) {
	switch (class_code) {
		case 0x1: {
			print("Mass Storage Controller", 0x07);
			break;
			
		} case 0x2: {
			print("Network Controller", 0x07);
			break;
			
		} case 0x3: {
			print("Display Controller", 0x07);
			break;
			
		} case 0x4: {
			print("Mutimedia Controller", 0x07);
			break;
			
		} case 0x5: {
			print("Memory Controller", 0x07);
			break;
			
		} case 0x6: {
			print("Bridge Device", 0x07);
			break;
			
		} case 0x7: {
			print("Simple Communication Controller", 0x07);
			break;
			
		} case 0x8: {
			print("Base System Peripheral", 0x07);
			break;
			
		} case 0x9: {
			print("Input Device", 0x07);
			break;
			
		} case 0xA: {
			print("Docking Station", 0x07);
			break;
			
		} case 0xB: {
			print("Processor", 0x07);
			break;
			
		} case 0xC: {
			print("Serial Bus Controller", 0x07);
			break;
			
		} case 0xD: {
			print("Wireless Controller", 0x07);
			break;
			
		} case 0xE: {
			print("Intelligent IO Controller", 0x07);
			break;
			
		} case 0xF: {
			print("Satellite Communication Controller", 0x07);
			break;
			
		} case 0x10: {
			print("Encryption Controller", 0x07);
			break;
			
		} case 0x11: {
			print("Data Acquisition and Signal Processing Controller", 0x07);
			break;
			
		} default: {
			print("Other Device (", 0x07);
			print_hex(class_code, 0x07);
			printch(')', 0x07);
			
		}
		
	}
	
	print(" (Sub ", 0x07);
	print_hex(sub_class, 0x07);
	
	print(", ", 0x07);
	print_hex(prog_IF, 0x07);
	printch(')', 0x07);
	
}
