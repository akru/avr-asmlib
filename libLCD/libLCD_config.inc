;      libLCD_config.inc
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


.equ	LCD_PORT	= PORTD
.equ	LCD_DDR		= DDRD
.equ	LCD_PIN		= PIND

.equ	RS	= PD0
.equ	RW	= PD1
.equ	E	= PD6
.equ	LCD_PIN_MASK = (1<<RS) | (1<<RW) | (1<<E)

.equ	D4	= PD2
.equ	D5	= PD3
.equ	D6	= PD4
.equ	D7	= PD5
.equ	DMASK = (1<<D4)|(1<<D5)|(1<<D6)|(1<<D7)

.equ	BF	= 7
.equ	IR	= 0							; Регистр команд
.equ	DR	= 1							; Регистр данных

.equ	CLEAN_SCREEN	= 0b00000001	; Очистка экрана, АС = 0, адресация АС на DDRAM
.equ	INIT1_MASK		= 0b00000100	; Выбирается направление сдвига курсора или экрана (ID, S)		
.equ	INIT2_MASK		= 0b00001000	; Выбирается режим отображения (D, C, B)
.equ	INIT3_MASK		= 0b00010000	; Команда сдвига курсора/экрана (SC, RL)
.equ	INIT4_MASK		= 0b00100000	; Определение параметров развертки и ширины шины данных (DL, N, F)

.equ	ID	= 1		; Режим смещения счетчика адреса АС, 0 - уменьшение, 1 - увеличение.
.equ	S	= 0		; 0 - сдвиг экрана не производится, 1 - после записи в DDRAM очередного кода экран сдвигается
.equ	D	= 2		; Наличие изображения: 0 - выключено, 1 - включено
.equ	C	= 1		; Курсор в виде подчерка: 0 - выключен, 1 - включен
.equ	B	= 0		; Курсор в виде мерцающего знакоместа: 0 - выключен, 1 - включен.
.equ	SC	= 3		; Определяет объект смещения: 0 - сдвигается курсор, 1 - сдвигается экран.
.equ	RL	= 2		; Уточняет направление сдвига: 0 - влево, 1 - вправо.
.equ	DL	= 4		; Флаг, определяющий ширину шины данных: 0 - 4 разряда, 1 - 8 разрядов.
.equ	N	= 3		; Режим развертки изображения на ЖКИ: 0 - одна строка, 1 - две строки
.equ	F	= 2		; Размер матрицы символов: 0 - 5 х 8 точек, 1 - 5 х 10 точек.

;	Инициализации
.equ	INIT1 = INIT1_MASK | (1<<ID)
.equ	INIT2 = INIT2_MASK | (1<<D) | (1<<B)
.equ	INIT3 = INIT3_MASK | (1<<RL)
.equ	INIT4 = INIT4_MASK | (1<<N)
