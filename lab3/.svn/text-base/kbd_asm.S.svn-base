/* void kbd_scan_handler_asm(); */

#include "i8042.h"

.global	_kbd_scan_handler_asm, _scancode

.data

.text
_kbd_scan_handler_asm:
	inb $STAT_REG 					#al receives the status byte
	mov %al, %dl
	test $OBF, %dl  				#if OBF bit is zero, there is no data to be read
	jz end
	inb $OUT_BUF 					#al receives the scancode
	test $(PAR_ERR | TO_ERR), %dl 	#if there is an error, the data is invalid
	jnz end
	movb %al, _scancode 			#scancode updated
	ret
	
end: xor %al, %al
	 movb %al, _scancode			#if no key received or some error ocurred, scancode = 0
	 ret
