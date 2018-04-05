//Ian Johnson
//C335
//HW4 - reg_name
//4/2/18

	.text
	.syntax unified
	.thumb
	.global reg_name
	.type reg_name, %function
reg_name:
	push {r4-r7}
	
	cmp r0, 16	//return 0 if passed value is outside range of 0-15
	bcs ERROR
	ldr r1, .labA
	lsls r0, r0, 2	//multiply by 4 so indexing for loading works correctly
	ldr r0, [r1, r0]	//pointer is now ready to be returned
	b END
ERROR:
	movs r0, 0		//return a null pointer if index was out of bounds
END:
	pop {r4-r7}
	bx lr
	
	.align 2
.labA:
	.word names