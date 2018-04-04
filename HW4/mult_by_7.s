//Ian Johnson
//C335
//HW4 - Multiply by 7 (Shift and subtract)
//4/2/18

	.text
	.syntax unified
	.thumb
	.global mult_by_7
	.type mult_by_7, %function
mult_by_7:
	push {r4-r7}
	
	mov r1, r0	//r0 about to get overwritten, back it up
	lsls r0, r0, #3	//multiply by 8
	subs r0, r0, r1	//subtract original value once, the result is a multiplication by 7
	mov r0, r1		//return the proper product
	
	pop {r4-r7}
	bx lr