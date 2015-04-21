BeepInit:
	sbi BEEP_DDR, BEEP_PIN
	clr beepcnt
	ret

Beep:
	ldi beepf, 1
	ret
