.include "lib/libMMC/libMMC_config.inc" 


MMCInit:
	sbi CS_DDR, MMC_CS
	sbi CS_PORT, MMC_CS
	cbi CS_DDR, MMC_LOCK
	cbi CS_PORT, MMC_LOCK

 LockVerif:
	sbis CS_PIN, MMC_LOCK
	rjmp MMCLockPrint
	
	rcall MMCCardInit
	ret

 MMCLockPrint:
	cli
	rcall CleanScr
	ldi prm, 0x10
	rcall SetCursor
	ldi ZH, high(CNF*2)
	ldi ZL, low(CNF*2)
	rcall PrintS
	sei
	rjmp LockVerif


MMCCardInit:
	cli
	rcall CleanScr
	ldi prm, 0x10
	rcall SetCursor
	ldi ZH, high(RF*2)
	ldi ZL, low(RF*2)
	rcall PrintS
	sei
	
	cbi CS_PORT, MMC_CS
	ser data
	rcall SPIWR
	ldi data, 0x40
	rcall SPIWR
	clr data
	rcall SPIWR
	clr data
	rcall SPIWR
	clr data
	rcall SPIWR
	clr data
	rcall SPIWR
	ldi data, 0x95
	rcall SPIWR
	ser data
	rcall SPIWR
	ser data
	rcall SPIWR
	ldi cnt, 8
 CIDelay:
	ser data
	rcall SPIWR	
	dec cnt
	brne CIDelay
	clr AdrHH
	clr AdrHL
	clr AdrLH
	clr AdrLL
 SDIni:
	ldi data, 0x41
	rcall SPISendCmd
	cpi data, 0
	brne SDIni
	ser data
	rcall SPIWR
	sbi CS_PORT, MMC_CS
	
	cli
	rcall CleanScr
	ldi prm, 0x10
	rcall SetCursor
	ldi ZH, high(MMCOK*2)
	ldi ZL, low(MMCOK*2)
	rcall PrintS
	sei
	
	ret


SPISendCmd:
	push data
	ser data
	rcall SPIWR
	pop data
	rcall SPIWR
	mov data, AdrHH
	rcall SPIWR
	mov data, AdrHL
	rcall SPIWR
	mov data, AdrLH
	rcall SPIWR
	mov data, AdrLL
	rcall SPIWR
	ser data
	rcall SPIWR
	ser data
	rcall SPIWR
	ser data
	rcall SPIWR
	ret
