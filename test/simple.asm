	.dw 30FFh
num1:
	.db 4040h
	.db 00h
	.dw main
	.dw 0


main:
dec CH
sub byte [si], 0Beefh
;jmp testing
;mov BX, 0x0001
;mov AL, [0x3000]
;mov AX, [BP]
;mov AX, [BX]
mov AL, [SI]
jnz testing
mov AH, [BX]
;mov AX, [DI]
;mov SI, 2
;mov DI, 1
;mov [BP + 2], AX
mov AX, [BP + 3]
mov AX, [BX + 4]
mov AX, [SI + 256]
mov AX, [DI + 1]

;MOV SI, 10
;mov BX, 15
;mov BP, 5
mov BX, [BP + SI + 3]
;mov BX, 5
;mov BP, [BX + SI]
;mov BP, 0
;mov BX, 1

mov [BX + SI + 1], AX
;mov AL,[num1]
;cmp AL, 0x00
;add SI, 0x02
;mov CL, 2
;XOR CL, CL
;shl AL, CL
;mov AX, 0xFFFF
;mov BX, 0x9000
;mov CX, 0x4000

mov SI, num1
;mov [num1], CL
mov word [SI], 4000
;int 0
;div word  [num1]
;neg byte  [num1]
testing:
hlt

.end main
;this is a comment which ends without an endline