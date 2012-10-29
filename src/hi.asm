start:	
		mov     dx, 04E9H           ; get display port I/O address
        mov     al, 'H'            ; display 'H'
        out     [dx], al
        mov 	ax, 0b1001111
        
label2:        mov     SP, 'i'             ; display 'i'
        out     [dx], al

        hlt                         ;  STOP!
	
        end     start


