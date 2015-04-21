.include "lib/libPS2/libPS2_config.inc"

PS2GetByte:
	rcall PS2Clock
	sbic KB_PIN, KB_DATA
	rjmp PS2GetByte

	clr data
	ldi cnt, 8
 PS2GetByteLoop:
	rcall PS2Clock
	sec
	sbis KB_PIN, KB_DATA
	clc
	ror data
	dec cnt
	brne PS2GetByteLoop
	
	rcall PS2Clock	
	rcall PS2Clock
	sbis KB_PIN, KB_DATA
	rjmp PS2GetByte
	ret

PS2Clock:
	sbis KB_PIN, KB_CLOCK
	rjmp PS2Clock
 PS2ClockDown:
	sbic KB_PIN, KB_CLOCK
	rjmp PS2ClockDown
	ret

PS2Init:
	rcall PS2GetByte
	cpi data, 0xAA
	breq PS2InitOk
	rjmp PS2Init
 PS2InitOk:
	ret

GetChar:
	rcall PS2GetByte
	cpi data, 0xF0
	brne GetChar
	rcall PS2GetByte
	ret
