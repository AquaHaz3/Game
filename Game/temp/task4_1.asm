data segment
	;INPUT
    x dw 3h;
    y dw 5h;
	; RESULT
    z dw 0h;
data ends 

stack segment
    dw   128  dup(0)
stack ends 

code segment

assume cs:code,ds:data,ss:stack

start:

    mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax   
    
	mov ax, x
	mov bx, y
    imul bx; dx:ax = x*y
	
	mov cx, x
	add cx, y
	idiv cx;
	
	mov z, cx
    
    mov ax, 4c00h
    int 21h    
code ends

end start 