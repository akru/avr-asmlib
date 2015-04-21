;      libDS1624.inc
;      
;      Copyright 2010 Александр Крупенькин <alexandr.krupenkin@gmail.com>
;      
;      This program is free software; you can redistribute it and/or modify
;      it under the terms of the GNU General Public License as published by
;      the Free Software Foundation; either version 2 of the License, or
;      (at your option) any later version.
;      
;      This program is distributed in the hope that it will be useful,
;      but WITHOUT ANY WARRANTY; without even the implied warranty of
;      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;      GNU General Public License for more details.
;      
;      You should have received a copy of the GNU General Public License
;      along with this program; if not, write to the Free Software
;      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
;      MA 02110-1301, USA.


DS1624Init:
	rcall I2cStart	; Auto conversion
	ldi data, 0x90
	rcall I2cWrite
	brcs DS1624Init
	ldi data, 0xAC
	rcall I2cWrite
	brcs DS1624Init
	ldi data, 0x00
	rcall I2cWrite
	brcs DS1624Init
	rcall I2cStop

 StartConv:
	rcall I2cStart	; Start conversion
	ldi data, 0x90
	rcall I2cWrite
	brcs StartConv
	ldi data, 0xEE
	rcall I2cWrite
	brcs StartConv
	rcall I2cStop
	ret


GetTemp:
	rcall I2cStart
	ldi data, 0x90
	rcall I2cWrite
	brcs GetTemp
	ldi data, 0xAA
	rcall I2cWrite
	brcs GetTemp
	rcall I2cStart
	ldi data, 0x91
	rcall I2cWrite
	brcs GetTemp
	set
	rcall I2cRead
	mov TempH, data
	set
	rcall I2cRead
	mov TempL, data
	rcall I2cStop
	ret
