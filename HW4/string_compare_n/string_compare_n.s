//Ian Johnson
//C335
//HW4 - strncmp
//4/2/18

	.text
	.syntax unified
	.thumb
	.global string_compare_n
	.type string_compare_n, %function
string_compare_n:
	push {r4-r7}

	//r2 (n) is assumed to be at least 1
LOOP:
	cmp r2, #0
	beq DONE
	subs r2, r2, #1		//counter--
	ldrb r3, [r0, #0]	//make sure we're only looking at chars
	ldrb r4, [r1, #0]
	cmp r3, r4
	bne INEQUAL
	adds r0, r0, #1		//increment string 1 pointer
	adds r1, r1, #1		//increment string 2 pointer
	b LOOP
INEQUAL:
	subs r0, r3, r4
	b END
DONE:
	movs r0, #0
END:
	pop {r4-r7}
	bx lr
