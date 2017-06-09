
#ifndef DAWN_H
	#define DAWN_H
	
	#include "../types.h"
	#include "../graphics/GFX.h"
	
	#define FELBONTAS_ADAT 335540096
	
	/*asm("\n\
	.text; .even; .globl  bswap64    ; .type  bswap64    ,@function;  bswap64    :\n\
		movl	%sp@(4),%d1\n\
		movl	%sp@(8),%d0\n\
		rolw	#8,%d1\n\
		rolw	#8,%d0\n\
		swap	%d1\n\
		swap	%d0\n\
		rolw	#8,%d0\n\
		rolw	#8,%d1\n\
		rts\n\
	");
	
	extern _bswap64();
	*/
	void dawn_boot(void);
	
#endif
