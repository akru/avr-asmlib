.include "lib/libI2C/libI2C_config.inc" 

I2cWrite:
	ldi cnt, 8
 I2cWriteLoop:
	rol data
	brcs I2cWriteOne
	sbi I2C_DDR, SDA
	rjmp I2cWriteOk
 I2cWriteOne:
	cbi I2C_DDR, SDA
 I2cWriteOk:
	cbi I2C_DDR, SCL
	rcall I2cDelay
	sbi I2C_DDR, SCL
	rcall I2cDelay
	dec cnt
	brne I2cWriteLoop
						; ACK
	cbi I2C_DDR, SDA
	rcall I2cDelay	
	cbi I2C_DDR, SCL
	rcall I2cDelay
	clc
	sbic I2C_PIN, SDA
	sec
	sbi I2C_DDR, SCL
	rcall I2cDelay
	ret


I2cRead:
	ldi data, 1
 I2cReadLoop:
	sbi I2C_DDR, SCL
	cbi I2C_DDR, SDA
	rcall I2cDelay
	cbi I2C_DDR, SCL
	rcall I2cDelay
	clc
	sbic I2C_PIN, SDA
	sec
	rol data
	brcc I2cReadLoop
						; ACK
	sbi I2C_DDR, SCL
	rcall I2cDelay
	brts I2cReadACKYes
	cbi I2C_DDR, SDA
	rjmp I2cReadACKOk
 I2cReadACKYes:
	sbi I2C_DDR, SDA
 I2cReadACKOk:
	rcall I2cDelay
	cbi I2C_DDR, SCL
	rcall I2cDelay
	ret


I2cStart:
	cbi I2C_DDR, SDA
	cbi I2C_DDR, SCL
	rcall I2cDelay
	sbis I2C_PIN, SDA
	rjmp I2cStart
	sbis I2C_PIN, SCL
	rjmp I2cStart
	sbi I2C_DDR, SDA
	rcall I2cDelay
	sbi I2C_DDR, SCL
	rcall I2cDelay
	ret


I2cStop:
	sbi I2C_DDR, SDA
	sbi I2C_DDR, SCL
	rcall I2cDelay
	cbi I2C_DDR, SCL
	rcall I2cDelay
	cbi I2C_DDR, SDA
	rcall I2cDelay
	clc
	sbic I2C_PIN, SDA
	ret
	sbic I2C_PIN, SCL
	ret
	sec
	ret


I2cDelay:
	nop
	nop
	nop
	nop
	nop
	ret
