.include "lib/libI2C/libI2C_config_slave.inc" 

I2CInit:
	ldi tmp, (1<<INT0)
	out MCUCR, tmp
	ldi tmp, (1<<)
	ret

I2CRead:
	ldi data, 1
 LoopRead:
	sbis I2C_PIN, SCL
	rjmp LoopRead
	clc
	sbic I2C_PIN, SDA
	sec
	rol data
	brcc NextBWait
	ret
 NextBWait:	
	sbic I2C_PIN, SCL
	rjmp NextBWait
	rjmp LoopRead	


 ReadACKVerOk:
	sbic I2C_PIN, SCL
	rjmp ReadACKVerOk
	sbi I2C_DDR, SDA
 SCLWU:
	sbis I2C_PIN, SCL
	rjmp SCLWU
 SCLWD:
	sbic I2C_PIN, SCL
	rjmp SCLWD
	cbi I2C_DDR, SDA
	ret


I2CWrite:
	ldi cnt, 8
 LoopWrite:
	sbic I2C_PIN, SCL
	rjmp LoopWrite
	rol data
	brcs Write1
	sbi I2C_DDR, SDA
	rjmp Write0
 Write1:	
	cbi I2C_DDR, SDA
 Write0:
	dec cnt
	brne WriteBWait
 ACKW:
	sbis I2C_PIN, SCL
	rjmp ACKW
 SCLWDW:
	sbic I2C_PIN, SCL
	rjmp SCLWDW
 SCLWUW:
	sbis I2C_PIN, SCL
	rjmp SCLWUW
 SCLWDW2:
	sbic I2C_PIN, SCL
	rjmp SCLWDW2
	ret
 WriteBWait:	
	sbis I2C_PIN, SCL
	rjmp WriteBWait
	rjmp LoopWrite
