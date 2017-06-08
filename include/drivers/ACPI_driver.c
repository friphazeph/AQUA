/*
 * ACPI_driver.c
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


#include "ACPI_driver.h"

static dword* SMI_CMD;

static byte ACPI_ENABLE;
static byte ACPI_DISABLE;

static dword* PM1a_CNT;
static dword* PM1b_CNT;

static word SLP_TYPa;
static word SLP_TYPb;
static word SLP_EN;
static word SCI_EN;

static byte PM1_CNT_LEN;

uint* ACPI_check_RSD_ptr(uint* ptr) {
	char* sig = "RSD PTR ";
	struct RSD_ptr* RSDP = (struct RSD_ptr*) ptr;
	
	byte* B_ptr;
	byte check = 0;
	
	int i;
	
	if (memcmp(sig, RSDP, 8) == 0) {
		B_ptr = (byte*) ptr;
		
		for (i = 0; i < sizeof(struct RSD_ptr); i++) {
			check += *B_ptr;
			B_ptr++;
			
		}
		
		if (check == 0) {
			if (RSDP->revision == 0) { // desc->revision
				print("ACPI 1\n", 0x0f);
				
			} else {
				print("ACPI 2\n", 0x0f);
				
			}
			
			return (uint*) RSDP->RSDT_address;
			
		}
		
	}
	
	return NULL;
	
}

uint* ACPI_get_RSD_ptr(void) {
	uint* addr;
	uint* RSDP;
	
	for (addr = (uint*) 0x000E0000; (int) addr < 0x00100000; addr += 0x10 / sizeof(addr)) {
		RSDP = ACPI_check_RSD_ptr(addr);
		
		if (RSDP != NULL) {
			return RSDP;
			
		}
		
	}
	
	int EBDA = *((int16*) 0x40E);
	EBDA = EBDA * 0x10 & 0x000FFFFF;
	
	for (addr = (uint*) EBDA; (int) addr < EBDA + 1024; addr += 0x10 / sizeof(addr)) {
		RSDP = ACPI_check_RSD_ptr(addr);
		
		if (RSDP != NULL) {
			return RSDP;
			
		}
		
	}
	
	return NULL;
	
}

int ACPI_check_header(uint* ptr, char* sig) {
	if (memcmp(ptr, sig, 4) == 0) {
		char* check_ptr = (char*) ptr;
		int len = *(ptr + 1);
		int8 check = 0;
		
		while (0 < len--) {
			check += *check_ptr;
			check_ptr++;
			
		}
		
		if (check == 0) {
			return 0;
			
		}
		
	}
	
	return -1;
	
}

int ACPI_enable(void) {
	if ((inportw((uint) PM1a_CNT) & SCI_EN) == 0) {
		if (SMI_CMD != 0 && ACPI_ENABLE != 0) {
			outportb((uint) SMI_CMD, ACPI_ENABLE);
			
			int i;
			for (i = 0; i < 300; i++) {
				if ((inportw((uint) PM1a_CNT) & SCI_EN) == 1) {
					break;
					
				}
				
				sleep(10);
				
			}
			
			if (PM1b_CNT != 0) {
				for (; i < 300; i++) {
					if ((inportw((uint) PM1b_CNT) & SCI_EN) == 1) {
						break;
						
					}
					
					sleep(10);
					
				}
				
			}
			
			if (i < 300) {
				print("ACPI enabled with success!\n", 0x0f);
				return 0;
				
			} else {
				print("ACPI failed to be enabled.\n", 0x06);
				return -1;
				
			}
			
		} else {
			print("No known way to enable ACPI.\n", 0x06);
			return -1;
			
		}
		
	} else {
		print("ACPI was already enabled.\n", 0x0f);
		return 0;
		
	}
	
}

int ACPI_init(void) {
	uint* ptr = ACPI_get_RSD_ptr();
	
	if (ptr != NULL && ACPI_check_header(ptr, "RSDT") == 0) {
		int entrys = *(ptr + 1);
		
		entrys = (entrys - 36) / 4;
		ptr += 36 / 4;
		
		while (0 < entrys--) {
			if (ACPI_check_header((uint*) *ptr, "FACP") == 0) {
				entrys = -2;
				struct FACP* _FACP = (struct FACP*) *ptr;
				
				if (ACPI_check_header((uint*) _FACP->DSDT, "DSDT") == 0) {
					char* S5_addr = (char*) _FACP->DSDT + 36;
					int DSDT_length = *(_FACP->DSDT + 1) - 36;
					
					while (0 < DSDT_length--) {
						if (memcmp(S5_addr, "_S5_", 4) == 0) {
							break;
							
						}
						
						S5_addr++;
						
					}
					
					if (DSDT_length > 0) {
						if ((*(S5_addr - 1) == 0x08 || (*(S5_addr - 2) == 0x08 && *(S5_addr - 1) == '\\')) && *(S5_addr + 4) == 0x12) {
							S5_addr += 5;
							S5_addr += ((*S5_addr & 0xC0) >> 6) + 2;
							
							if (*S5_addr == 0x0A) {
								S5_addr++;
								
							}
							
							SLP_TYPa = *(S5_addr) << 10;
							S5_addr++;
							
							if (*S5_addr == 0x0A) {
								S5_addr++;
								
							}
							
							SLP_TYPb = *(S5_addr) << 10;
							
							SMI_CMD = _FACP->SMI_CMD;
							
							ACPI_ENABLE = _FACP->ACPI_ENABLE;
							ACPI_DISABLE = _FACP->ACPI_DISABLE;
							
							PM1a_CNT = _FACP->PM1a_CNT_BLK;
							PM1b_CNT = _FACP->PM1b_CNT_BLK;
							
							PM1_CNT_LEN = _FACP->PM1_CNT_LEN;
							
							SLP_EN = 1 << 13;
							SCI_EN = 1;
							
							return 0;
							
						} else {
							print("\\_S5 parse error.\n", 0x06);
							
						}
						
					} else {
						print("\\_S5 not present.\n", 0x06);
						
					}
					
				} else {
					print("DSDT is invalid.\n", 0x06);
					
				}
				
			}
			
			ptr++;
			
		}
		
		print("No valid FACP was found.\n", 0x06);
		
	} else {
		print("No ACPI was found.\n", 0x06);
		
	}
	
	return -1;
	
}

void ACPI_poweroff(void) {
	if (SCI_EN == 0) {
		return;
		
	}
	
	ACPI_enable();
	outportw((uint) PM1a_CNT, SLP_TYPa | SLP_EN);
	
	if (PM1b_CNT != 0) {
		outportw((uint) PM1b_CNT, SLP_TYPb | SLP_EN);
		
	}
	
	print("ACPI poweroff failed. Trying a halt.\n", 0x06);
	asm("hlt");
	
}
