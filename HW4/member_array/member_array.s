//Ian Johnson
//C335
//HW4 - member_array
//4/2/18

	.text
	.syntax unified
	.thumb
	.global member_array
	.type member_array, %function
member_array:
	push {r4-r7}
	
	movs r3, #0		//counter
	//assumed that size (r2) is at least 1
	lsls r2, r2, #2		//make r2 (e) a multiple of 4
LOOP:
	ldr r4, [r0, r3]
	cmp r4, r1
	bne INEQUAL
	//found a match
	movs r0, #1
	b END
INEQUAL:
	adds r3, r3, #4
	cmp r3, r2
	bcc LOOP		//continue searching if we haven't reached the end of the array yet
	movs r0, #0		//no match
END:
	pop {r4-r7}
	bx lr
