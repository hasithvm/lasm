num1:
	.db 40h
	.db 00h
	.dw main
	.dw 0
main: 
mov AL, 128
cmp AL, 0x00
add SI, 0x02
mov CL, 2
XOR CL, CL
shl AL, CL
;mov AX, 0xFFFF
;mov BX, 0x9000
;mov CX, 0x4000

;mov SI, num1
mov [num1], CL
int 0
;div word  [num1]
;neg byte  [num1]
hlt

.end main
;this is a comment which ends without and enl
