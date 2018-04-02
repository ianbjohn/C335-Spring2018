//Ian Johnson
//C335
//HW4 - Absolute Value
//4/2/18

	.text
	.syntax unified
	.thumb
	.global	abs	
	.type abs, %function
abs:
	push {r4-r7}

	cmp r0, #0	/*Is Z flag set automatically? */
	bpl END
	negs r0, r0	/* r0 = -r0 */
END:	
	
	pop {r4-r7}	
	bx	lr
