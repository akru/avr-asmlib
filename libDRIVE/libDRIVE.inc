.include "lib/libDRIVE/libDRIVE_config.inc"

DriveInit:
	ldi tmp, DRIVE_MASK
	out DRIVE_DDR, tmp
	ret

DriveGo:
	in tmp, DRIVE_PORT
	andi tmp, ~DRIVE_MASK
	or tmp, prm
	out DRIVE_PORT, tmp
	ret

DriveStop:
	in tmp, DRIVE_PORT
	andi tmp, ~DRIVE_MASK
	out DRIVE_PORT, tmp
	ret
