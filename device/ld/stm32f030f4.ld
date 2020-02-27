_estack = 0x20001000;

/* Set minimum size for stack and dynamic memory. */
/* (The linker will generate an error if there is
 * less than this much RAM leftover.) */
/* (1KB) */
_Min_Leftover_RAM = 0x400;

MEMORY
{
	FLASH (rx)	:	ORIGIN = 0x08000000, LENGTH = 16k
	RAM (rxw)	:	ORIGIN = 0x20000000, LENGTH = 4k
}

INCLUDE "sections.ld"
