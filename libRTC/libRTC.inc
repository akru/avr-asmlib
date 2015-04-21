.include "lib/libRTC/libRTC_config.inc" 


RtcInit:
	sbis I2C_PIN, SDA
	rcall Err
	ldi prm, 7
	rcall RtcRead
	mov tmp, data
	ldi prm, 0
	rcall RtcRead
	brcs Err
	
	cpi data, 0x80
	brsh SetSec
	cpi tmp, 0x10
	brne SetClk
	rjmp SetRAM
 SetSec:
	ldi prm, 0
	ldi data, 0
	rcall RtcWrite
	brcs Err
 SetClk:
	ldi prm, 7
	ldi data, 0b00010000
	rcall RtcWrite
	brcs Err
 SetRam:
	rcall RtcGet
	rcall RtcConv
	ret

SetTime:		; Часы - минуты :: prm - data
	push prm
	ldi prm, 1
	rcall RtcWrite
	inc prm
	pop data
	rcall RtcWrite
	ret
	
SetDate:		; Дата - месяц - год :: data - prm - tmp
	push tmp
	push prm
	ldi prm, 4
	rcall RtcWrite
	inc prm
	pop data
	rcall RtcWrite
	inc prm
	pop data
	rcall RtcWrite
	ret


Err:
	rcall CleanScr
	ldi prm, 0x0B
	rcall SetCursor
	ldi prm, low(ErrMsg)
	rcall PrintS
 ErrCykle:
	rjmp ErrCykle


RtcGet:
	ldi ZL, low(RTCSec)
	ldi prm, 0
	sbis I2C_PIN, SDA
	rcall Err
	rcall I2cStart
	ldi data, CLOCK_ADDR | WRITE
	rcall I2cWrite
	brcs Err
	ldi data, 0
	rcall I2cWrite
	brcs Err
	rcall I2cStart
	ldi data, CLOCK_ADDR | READ
	rcall I2cWrite
	brcs Err
	set
	rcall I2cRead
	brcs Err
	st Z+, data
	rcall I2cRead
	brcs Err
	st Z+, data
	rcall I2cRead
	brcs Err
	st Z+, data
	rcall I2cRead
	brcs Err
	rcall I2cRead
	brcs Err
	st Z+, data
	rcall I2cRead
	brcs Err
	st Z+, data
	clt	
	rcall I2cRead
	brcs Err
	st Z+, data
	rcall I2cStop
	ret


RtcConv:
	ldi ZL, low(RTCHour)
	ld tmp, Z
	mov data, tmp
	andi tmp, 0b00001111
	ldi ZL, low(HourL)
	st Z, tmp
	andi data, 0b11110000
	swap data
	ldi ZL, low(HourH)
	st Z, data
	
	ldi ZL, low(RTCMin)
	ld tmp, Z
	mov data, tmp
	andi tmp, 0b00001111
	ldi ZL, low(MinL)
	st Z, tmp
	andi data, 0b11110000
	swap data
	ldi ZL, low(MinH)
	st Z, data
	
	ldi ZL, low(RTCDate)
	ld tmp, Z
	mov data, tmp
	andi tmp, 0b00001111
	ldi ZL, low(DateL)
	st Z, tmp
	andi data, 0b11110000
	swap data
	ldi ZL, low(DateH)
	st Z, data
	
	ldi ZL, low(RTCMonth)
	ld tmp, Z
	mov data, tmp
	andi tmp, 0b00001111
	ldi ZL, low(MonthL)
	st Z, tmp
	andi data, 0b11110000
	swap data
	ldi ZL, low(MonthH)
	st Z, data
	
	ldi ZL, low(RTCYear)
	ld tmp, Z
	mov data, tmp
	andi tmp, 0b00001111
	ldi ZL, low(YearL)
	st Z, tmp
	andi data, 0b11110000
	swap data
	ldi ZL, low(YearH)
	st Z, data
	ret


RtcWrite:
	cbi I2C_PORT, SDA
	cbi I2C_PORT, SCL
	ldi cnt, 120
 loop_cw:
	push data
	rcall I2cStart
	ldi data, CLOCK_ADDR | WRITE
	rcall I2cWrite
	brcs rt_write
	mov data, prm
	rcall I2cWrite
	brcs rt_write
	pop data
	rcall I2cWrite
	brcs rt_
	rcall I2cStop
	brcs rt_
	ret
 rt_write:
	pop data
 rt_:
	brcc Ok_wr_
	dec cnt
	brne loop_cw
 Ok_wr_:
	ret
	
	
RtcRead:
	cbi I2C_PORT, SDA
	cbi I2C_PORT, SCL
	ldi cnt, 120
 loop_cr:
	rcall I2cStart
	ldi data, CLOCK_ADDR | WRITE
	rcall I2cWrite
	brcs rt__
	mov data, prm
	rcall I2cWrite
	brcs rt__
	rcall I2cStart
	ldi data, CLOCK_ADDR | READ
	rcall I2cWrite
	brcs rt__
	clt
	rcall I2cRead
	rcall I2cStop
	brcs rt__
	ret
 rt__:
	dec cnt
	brne loop_cr
	ret
