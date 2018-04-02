//Ian Johnson
//C335
//HW4 - GCD
//4/2/18

	.text
	.syntax unified
	.thumb
	.global	gcd	
	.type gcd, %function
gcd:
	push {r4-r7}

	movs r2, #0	/* d */

	movs r4, #1
LOOP1:
	mov r3, r0
	ands r3, r3, r4	/* r3 and r4 are used as temp register of sorts */
	bne LOOP2
	mov r3, r1
	ands r3, r3, r4
	bne LOOP2

	lsrs r0, r0, #1
	lsrs r1, r1, #1
	adds r2, r2, #1
	b LOOP1
LOOP2:
	cmp r0, r1
	beq DONE

CASE0:
	mov r3, r0
	ands r3, r3, r4
	bne CASE1
	lsrs r0, r0, #1
	b LOOP2
CASE1:
	mov r3, r1
	ands r3, r3, r4
	bne CASE2
	lsrs r1, r1, #1
	b LOOP2
CASE2:
	cmp r0, r1
	bcc DEFAULT
	beq DEFAULT //only execute if r0 > r1
	subs r0, r0, r1
	lsrs r0, r0, #1	//r0 = (r0 - r1) >> 1;
	b LOOP2
DEFAULT:
	subs r1, r1, r0
	lsrs r1, r1, #1	//r1 = (r1 - r0) >> 1;
	b LOOP2
DONE:
	lsls r0, r0, r2
	
	pop {r4-r7}
	bx	lr
