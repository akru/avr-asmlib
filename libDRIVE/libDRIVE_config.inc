.equ	DRIVE_PORT		= PORTD
.equ	DRIVE_DDR		= DDRD

.equ	DRIVE_MASK		 = (1<<PD3) | (1<<PD2) | (1<<PD1) | (1<<PD0)
.equ	DRIVE_LEFT_MASK	 = (1<<PD3) | (1<<PD2)
.equ	DRIVE_RIGHT_MASK = (1<<PD1) | (1<<PD0)
.equ	DRIVE_LEFT_UP	 = (1<<PD0)
.equ	DRIVE_LEFT_DOWN  = (1<<PD1)
.equ	DRIVE_RIGHT_UP	 = (1<<PD3)
.equ	DRIVE_RIGHT_DOWN = (1<<PD2)
.equ	DRIVE_GO_LEFT	 = DRIVE_LEFT_DOWN	| DRIVE_RIGHT_UP
.equ	DRIVE_GO_RIGHT	 = DRIVE_LEFT_UP	| DRIVE_RIGHT_DOWN
.equ	DRIVE_GO_UP		 = DRIVE_LEFT_UP	| DRIVE_RIGHT_UP
.equ	DRIVE_GO_DOWN	 = DRIVE_LEFT_DOWN	| DRIVE_RIGHT_DOWN