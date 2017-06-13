;  kernel.asm
;  
;  Copyright 2017 Obiwac
;  
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;  
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;  
;  You should have received a copy of the GNU General Public License
;  along with this program; if not, write to the Free Software
;  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
;  MA 02110-1301, USA.
;  
;  


	bits 32

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)

section .data
	align 0x1000
	
	global boot_page_directory
	boot_page_directory:
		dd 0x00000083
		times (KERNEL_PAGE_NUMBER - 1) dd 0
		
		dd 0x00000083
		times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0
	
	retfromkernel:
		db 0xa, "|-----------------------|",0xa,"| Returned from kernel. |",0xa,"|-----------------------|"

section .text
	align 4
	
	dd 0x1BADB002
	dd 0x04
	dd -(0x1BADB002 + 0x04)
	
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	
	dd 1;0
	dd 800;800
	dd 600;600
	dd 32;32
	
	extern main
	extern call_constructors
	
	global start
	start:
		cli
		
		mov ecx, (boot_page_directory - KERNEL_VIRTUAL_BASE)
		mov cr3, ecx
		
		mov ecx, cr4
		or ecx, 0x00000010
		mov cr4, ecx
		
		;mov ecx, cr0
		;or ecx, 0x80000000
		;mov cr0, ecx
		
		lea ecx, [start_hh]
		jmp ecx
	
	start_hh:
		mov dword [boot_page_directory], 0
		invlpg [0]
		
		mov esp, stack + 0x4000
		call call_constructors
		
		push dword ebx
		push dword eax
		
		call main
		
		mov eax, 1
		mov ebx, retfromkernel
		int 0x80
		jmp $
		
		call stop
	
	stop:
		cli
		
		hlt
		jmp stop
	
	global load_page_dir
	load_page_dir:
		push dword [ebp + 8]
		
		mov ebp, esp
		mov eax, esp
		
		mov cr3, eax
		mov esp, ebp
		
		pop ebp
		ret
		
		;push ebp
		;mov ebp, esp
		;push dword [ebp + 8]
		;pop eax
		
		;pusha
		;mov cr3, eax
		
		;mov eax, cr0
		;or eax, 0x80000000
		;mov cr0, eax
		;popa
		
		;mov esp, ebp
		;pop ebp
		;ret
	
	global enable_paging
	enable_paging:
		push ebp
		mov ebp, esp
		
		mov eax, cr0
		or eax, 0x80000000
		mov cr0, eax
		
		mov esp, ebp
		pop ebp
		ret
	
	extern gdt_ptr
	
	global gdt_flush
	gdt_flush:
		cli
		lgdt [gdt_ptr]
		
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		
		jmp 0x08:flush2
	
	flush2:
		ret
	
	global tss_flush
	tss_flush:
		mov ax, 0x2B
		ltr ax
		ret
	
	global tss_install
	tss_install:
		mov ax, 0x28
		ltr ax
	
	extern user_function
	
	global jump_usermode
	jump_usermode:
		mov ax, 0x23
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		
		mov eax, esp
		push 0x23
		push eax
		
		pushf
		push 0x1B
		push user_function
		
		iret
	
	disk_load:
		push dx
		
		mov ah, 0x02
		mov al, dh
		mov ch, 0x00
		mov dh, 0x00
	
	disk_load_go:
		int 0x13
		jc disk_error
		pop dx
		cmp dh, al
		
		jne disk_error
		ret
	
	disk_error:
		jmp $
	
	global idt_load
	extern idtp
	
	idt_load:
		lidt [idtp]
		ret
	
	global irq0
	global irq1
	global irq2
	global irq3
	global irq4
	global irq5
	global irq6
	global irq7
	global irq8
	global irq9
	global irq10
	global irq11
	global irq12
	global irq13
	global irq14
	global irq15
	
	irq0:
		cli
		push byte 0
		push byte 32
		jmp irq_common_stub
	
	irq1:
		cli
		push byte 0
		push byte 33
		jmp irq_common_stub
	
	irq2:
		cli
		push byte 0
		push byte 34
		jmp irq_common_stub
	
	irq3:
		cli
		push byte 0
		push byte 35
		jmp irq_common_stub
	
	irq4:
		cli
		push byte 0
		push byte 36
		jmp irq_common_stub
	
	irq5:
		cli
		push byte 0
		push byte 37
		jmp irq_common_stub
	
	irq6:
		cli
		push byte 0
		push byte 38
		jmp irq_common_stub
	
	irq7:
		cli
		push byte 0
		push byte 39
		jmp irq_common_stub
	
	irq8:
		cli
		push byte 0
		push byte 40
		jmp irq_common_stub
	
	irq9:
		cli
		push byte 0
		push byte 41
		jmp irq_common_stub
	
	irq10:
		cli
		push byte 0
		push byte 42
		jmp irq_common_stub
	
	irq11:
		cli
		push byte 0
		push byte 43
		jmp irq_common_stub
	
	irq12:
		cli
		push byte 0
		push byte 44
		jmp irq_common_stub
	
	irq13:
		cli
		push byte 0
		push byte 45
		jmp irq_common_stub
	
	irq14:
		cli
		push byte 0
		push byte 46
		jmp irq_common_stub
	
	irq15:
		cli
		push byte 0
		push byte 47
		jmp irq_common_stub
	
	extern irq_handler
	irq_common_stub:
		pusha
		push ds
		push es
		push fs
		push gs
		
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		
		mov eax, esp
		push eax
		mov eax, irq_handler
		call eax
		pop eax
		
		pop gs
		pop fs
		pop es
		pop ds
		popa
		
		add esp, 8
		iret

section .bss
	align 32
	
	stack:
		resb 0x4000
