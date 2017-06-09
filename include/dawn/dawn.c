
#include "dawn.h"

#define bswap64 __builtin_bswap64

static uint32 width = 800;
static uint32 height = 600;

static int64* vga;

static uint64* utasitaskezd;
static int64 a;

static int64 b;
static int64 b_ertek;

static int64* RAM;

int64 dawn_loop(int64 eip) {
	int i;
	for (i = 0; i < 4096; i++) {
		utasitaskezd = (uint64*) &RAM[eip];
		a = bswap64(utasitaskezd[0]);
		b = bswap64(utasitaskezd[1]);
		
		b_ertek = bswap64((uint64*) &RAM[b]) - bswap64((uint64*) &RAM[a]);
		*(int64*) &RAM[b] = bswap64(b_ertek);
		
		if (b_ertek <= 0) {
			eip = bswap64(utasitaskezd[2]);
			
		} else {
			eip += 24;
			
		}
		
	}
	
	return eip;
	
}

void dawn_boot(void) {
	RAM = (int64*) kmalloc(400000000);
	
	int r;
	for (r = 0; r < 400000000 / 4; r++) {
		RAM[r] = 0;
				
	}
	
	GFX_wash_hex(0x00FF00FF);
	sleep(100);
	
	int64 eip = 0;
	int cycle = 0;
	
	while (TRUE) {
		vga = (int64*) (&RAM[FELBONTAS_ADAT]);
		vga[0] = bswap64(width);
		vga[1] = bswap64(height);
		
		int i;
		for (i = 0; i < 4096; i++) {
			utasitaskezd = (uint64*) &RAM[eip];
			a = bswap64(utasitaskezd[0]);
			b = bswap64(utasitaskezd[1]);
			
			b_ertek = bswap64((uint64*) &RAM[b]) - bswap64((uint64*) &RAM[a]);
			*(int64*) &RAM[b] = bswap64(b_ertek);
			
			if (b_ertek <= 0) {
				eip = bswap64(utasitaskezd[2]);
				
			} else {
				eip += 24;
				
			}
			
		}
		
		cycle++;
		
		if (cycle > 4096) {
			GFX_blit_image(0, 0, width, height, (uint8*) vga);
			cycle = 0;
			
		}
		
	}
	
}
