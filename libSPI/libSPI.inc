.include "lib/libSPI/libSPI_config.inc"


SpiInit:
	sbi SPI_DDR, SCK
	sbi SPI_DDR, MOSI
	cbi SPI_PORT, SCK
	cbi SPI_PORT, MOSI
	ret

SPIWR:
	cli
	ldi cnt, 8
	clr tmp
 SPILoop:
	lsl data
	brcc Put0
	sbi SPI_PORT, MOSI
	nop
	rjmp RBit
 Put0:
	cbi SPI_PORT, MOSI
	nop
 RBit:
	sbi SPI_PORT, SCK
	nop
	sbic SPI_PIN, MISO
	rjmp R1Bit
	clc
	rjmp REndBit
 R1Bit:
	sec
 REndBit:
	rol tmp
	cbi SPI_PORT, SCK
	nop
	dec cnt
	brne SPILoop
	mov data, tmp
	sei
	ret
