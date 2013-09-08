	.dw 30FFh
num1:
	.db 4040h
	.db 00h
	.dw main
	.dw 0




    Testing:
    SHR AX, 1
     
    MOV AX,5
    CMP AX,10
    JLE Testing
     
    HLT


.end Testing
;this is a comment which ends without an endline