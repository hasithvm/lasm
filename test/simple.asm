; SYSC2001 - Lab 2a
; Program to complete 8-bit unsigned shift & add multiplication
; AH = A; CH = Q; BH = M; Final 16-bit result in AX

.ORG 00h
Data:
	Y:	.DB	5		; Multiplicand
	X: 	.DB	2		; Multiplier
	
.ORG 0010h
Init:

	mov BX, [BP + 2]
	MOV AX,0		; Initialize AX to zero. AH serves as accumulator and AX will hold product
	MOV CH,[X]		; Initialize CH (Q) = X
	MOV	BH,[Y]		; Init BH (M) = Y
	MOV	DL,8 		; Init DL as a loop counter with number of iterations required
	MOV	CL,1		; Init CL with required number of shifts (1)
	
mainLoop:
	SHR CH,CL		; Shift out the lsb of the multiplier (Q[0]) into the carry flag
	JNC shift		; Check the carry flag: If Q[0] was not set, skip over Add and just shift
AddM:
	ADD AH, BH		; A = A + M
shift:
	RCR AX,CL		; Shift AH and AL (16-bit result will be here eventually). Also need to shift C into MSb of AH...
	DEC DL			; Decrement loop counter
	JNZ	mainLoop		; If loop counter reaches zero, quit, else, loop back
quit:
	HLT
.END	Init