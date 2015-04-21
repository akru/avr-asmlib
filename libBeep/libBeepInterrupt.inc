.include "lib/libBeep/libBeepConfig.inc"

	sbrs beepf, 0
	reti

	in tmp, SREG
	sbrc beepf, 1
	rjmp FlagIsSet
	ori beepf, 0b00000010
	sbi BEEP_PORT, BEEP_PIN
	rjmp BeepCountInc
 FlagIsSet:
	andi beepf, 0b11111101
	cbi BEEP_PORT, BEEP_PIN

 BeepCountInc:
	inc beepcnt
	cpi beepcnt, BEEP_DELAY
	brne TimOvfQuit
	clr beepf
	clr beepcnt
 TimOvfQuit:
	out SREG, tmp
