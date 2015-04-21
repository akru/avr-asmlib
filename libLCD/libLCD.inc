;      libLCD.inc
;      
;      Copyright 2009 Александр Крупенькин <alexandr.krupenkin@gmail.com>
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

.include "lib/libLCD/libLCD_config.inc"

LcdInit:						; --------------------------------------
								; | Инициализация дисплея
	ldi buf, LCD_PIN_MASK		; |    Устанавливаем правляющие пины 
	out LCD_DDR, buf			; |    на выход
								; |
	ldi prm, (IR<<RS)			; |    Работаем с регистром команд
								; |
	ldi data, INIT4				; |    Устанавливаем 4-разрядную ШД и
	rcall LcdWrite4				; |    2-х строчную развертку
	rcall LcdWrite				; |    Повторяем для 4-битного режима
								; |
	ldi data, INIT3 			; |
	rcall LcdWrite				; |
								; |
	ldi data, INIT2				; |
	rcall LcdWrite				; |
								; |
	ldi data, INIT1				; |
	rcall LcdWrite				; |
								; |
	ldi data, CLEAN_SCREEN		; |
	rcall LcdWrite				; |
	ret							; --------------------------------------

CleanScr:						; --------------------------------------
	ldi prm, (IR<<RS)			; | Очистка экрана
	ldi data, CLEAN_SCREEN		; |
	rcall LcdWrite				; |
	ret							; --------------------------------------

PrintC:							; --------------------------------------
	ldi prm, (DR<<RS)			; | Вывод символа на дисплей
	rcall LcdWrite				; |    data должен содержать код символа
	ret							; --------------------------------------


PrintS:							; --------------------------------------
	sbic EECR, EEPE				; | Вывод строки символов на дисплей
	rjmp PrintS					; |     prm должен содержать адрес начала
	out EEAR, prm				; |     Строка должна оканчиваться на 0
	sbi EECR, EERE				; |
	in data, EEDR				; |
	cpi data, 0					; |
	breq QuitPrintS				; |
	rcall PrintC				; |
	inc prm						; |
	rjmp PrintS					; |
 QuitPrintS:					; |
	ret							; --------------------------------------


SetCursor:						; --------------------------------------
	ldi data, 0					; | Установка курсора в указаную позицию
	sbrc prm, 4					; |     старшая тетрада prm - № строки
	ldi data, 0x40				; |     младшая тетрада prm - № символа
	andi prm, 0xf				; |
	add data, prm				; |
	ori data, 0b10000000		; |
	ldi prm, (IR<<RS)			; |
	rcall LcdWrite				; |
	ret							; --------------------------------------


LcdWrite:						; --------------------------------------
	push data					; | Запись по 4-х битной ШД c ожиданием
	push prm					; | готовности дисплея
								; |
	ldi prm, (IR<<RS)			; --------------------------------------
 LcdNoFree:						; |     Проверка на готовность
	rcall LcdRead4				; |
	sbrc data, BF				; |
	rjmp LcdNoFree				; --------------------------------------
								; |
	pop prm						; |
	pop data					; |
								; --------------------------------------
	rcall LcdWrite4				; |     Пишем
	ret							; --------------------------------------


LcdRead:
LcdRead4:						; --------------------------------------
	sbrs prm, RS				; | Чтение по 4-х битной ШД
	cbi	LCD_PORT, RS			; |    Устанавливаем значение RS
	sbrc prm, RS				; |
	sbi LCD_PORT, RS			; |
								; --------------------------------------
	sbi LCD_PORT, RW			; |    Устанивливаем RW = 1
	nop							; --------------------------------------
	sbi LCD_PORT, E				; |    Устанивливаем E = 1
	nop							; --------------------------------------
								; --------------------------------------
	in buf, LCD_PIN				; |    Читаем старшую тетраду
								; --------------------------------------
	mov tmp, buf				; --------------------------------------
	andi tmp, DMASK				; |    Выборка и сдвиг старшей тетрады
	ldi cnt, 4-D4				; |
 rep1:							; |
	lsl tmp						; |
	dec cnt						; |
	brne rep1					; --------------------------------------
								; --------------------------------------
	mov data, tmp				; |     Сохраняем старшую тетраду
								; --------------------------------------
	cbi LCD_PORT, E				; |     Устанивливаем E = 0
	nop							; --------------------------------------
	sbi LCD_PORT, E				; |     Устанивливаем E = 1
								; --------------------------------------
	in buf, LCD_PIN				; |     Читаем младшую тетраду
								; --------------------------------------
	mov tmp, buf				; --------------------------------------
	andi tmp, DMASK				; |     Выборка и сдвиг младшей тетрады
	ldi cnt, D4					; |
 rep2:							; |
	lsr tmp						; |
	dec cnt						; |
	brne rep2					; --------------------------------------
								; --------------------------------------
	or data, tmp				; |     Сохраняем младшую тетраду
								; --------------------------------------
	cbi LCD_PORT, E				; |     Устанивливаем E = 0
	nop							; --------------------------------------
	cbi LCD_PORT, RW			; |     Устанивливаем RW = 0
	ret							; --------------------------------------


LcdWrite4:						; --------------------------------------
	sbrs prm, RS				; | Запись по 4-х битной ШД
	cbi	LCD_PORT, RS			; |     Устанавливаем значение RS
	sbrc prm, RS				; |
	sbi LCD_PORT, RS			; |
								; --------------------------------------
	mov tmp, data				; --------------------------------------
	andi tmp, 0xf0				; |     Выборка и сдвиг старшей тетрады
	ldi cnt, 4 - D4				; |
 rep3:							; |
	lsr tmp						; |
	dec cnt						; |
	brne rep3					; --------------------------------------
								; |
	in buf, LCD_DDR				; -------------------------------------- 
	ori buf, DMASK				; |     Шина данных на выход
	out LCD_DDR, buf			; --------------------------------------
								; |
	in buf, LCD_PORT			; --------------------------------------
	andi buf, ~DMASK			; |     Запись старшей тетрады в шину
	or buf, tmp					; |
	out LCD_PORT, buf			; --------------------------------------
								; --------------------------------------
	sbi LCD_PORT, E				; |     Устанивливаем E = 1
	nop							; --------------------------------------
	cbi LCD_PORT, E				; |     Устанавливаем E = 0
								; --------------------------------------
	mov tmp, data				; --------------------------------------
	andi tmp, 0xf				; |     Выборка и сдвиг младшей тетрады
	ldi cnt, D4					; |
 rep4:							; |
	lsl tmp						; |
	dec cnt						; |
	brne rep4					; --------------------------------------
								; |
	in buf, LCD_PORT			; --------------------------------------
	andi buf, ~DMASK			; |     Запись младшей тетрады в шину
	or buf, tmp					; |
	out LCD_PORT, buf			; --------------------------------------
								; --------------------------------------
	sbi LCD_PORT, E				; |     Устанивливаем E = 1
	nop							; --------------------------------------
	cbi LCD_PORT, E				; |     Устанавливаем E = 0
								; --------------------------------------
	in buf, LCD_DDR				; --------------------------------------	
	andi buf, ~DMASK			; |     Шина данных на вход
	out LCD_DDR, buf			; |
	nop							; |
	in buf, LCD_PORT			; |
	andi buf, ~DMASK			; |
	out LCD_PORT, buf			; |	
	ret							; --------------------------------------

KeymapConvert:
	sbic EECR, EEPE			
	rjmp KeymapConvert
	out EEAR, data
	sbi EECR, EERE		
	in data, EEDR
	ret
