start:
        mov     dx, 04E9H           ; get display port I/O address
        mov BYTE PTR    al, 'H'             ; display 'H'
        out     [dx], al

label2:        mov     SP, 'i'             ; display 'i'
        out     [dx], al

        hlt                         ;  STOP!

        end     start


