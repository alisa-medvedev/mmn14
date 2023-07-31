LOOP: 	jmp L1
MAIN: 	mov @r3 ,LENGTH
	mcro m1fwerf 
		sub @r1, @r4
		bne END
	endmcro
	prn -5
	bne LOOP
	m1fwerf 
L1: 	inc K
bne 	LOOP
	mcro m2 
		sub @r6, @r4
	endmcro
END: 	stop
;vserb
STR: 	.string “abcdef”
	m2


m1fwerf 
m1fwerf 


K: 	.data 2