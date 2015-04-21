 EdgeStartWait:
	brne SDAStartWait
	sbis I2C_PIN, SDA
	rjmp SCLStartWait
 SDAStartWait:
	sbis I2C_PIN, SCL
	rjmp I2CWaitStart
	sbis I2C_PIN, SDA
	sez
	rjmp EdgeStartWait
 SCLStartWait:	
	sbic I2C_PIN, SCL
	rjmp SCLStartWait
