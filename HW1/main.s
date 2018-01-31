00008b88 <main>:
    8b88:	b510      	push	{r4, lr}
    8b8a:	2400      	movs	r4, #0
    8b8c:	f7ff ffb4 	bl	8af8 <stop_light>
    8b90:	1c20      	adds	r0, r4, #0
    8b92:	f000 f805 	bl	8ba0 <inc>
    8b96:	1e04      	subs	r4, r0, #0
    8b98:	2c23      	cmp	r4, #35	; 0x23
    8b9a:	ddf7      	ble.n	8b8c <main+0x4>
    8b9c:	bd10      	pop	{r4, pc}
    8b9e:	46c0      	nop			; (mov r8, r8)
