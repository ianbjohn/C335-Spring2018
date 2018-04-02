//Ian Johnson
//C335
//HW4 - Fibonacci Sequence
//4/2/18

	.text
	.syntax unified
	.thumb
	.global	fib	
	.type fib, %function
fib:
	push {r4-r7}

	movs r1, #1
	movs r2, #1	//two prevs
	movs r3, #1	//val
	movs r4, #0	//counter
	
	cmp r0, #0
	beq END		//if num is 0, return 0	
	cmp r0, #1
	beq END		//if num is 1, return 1

	/* the loop */
LOOP:	
	subs r5, r0, #2
	cmp r4, r5
	bcs DONE	/* exit loop if counter >= num-2 */
	adds r3, r1, r2 /* val = prev1 + prev2 */
	mov r2, r1	/* prev1 = prev2 */
	mov r1, r3	/* prev1 = val */
	adds r4, r4, #1
	b LOOP
DONE:
	mov r0, r3	/* return val */
END:	
	pop {r4-r7}
	bx	lr			
