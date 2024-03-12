data segment
	;INPUT
    a dw 3h;
    b dw 5h;
	;VARS
    sum dw 0;
    sum_square dw 0,0
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
    
    xor cx, cx
    xor bx, bx    
    
    mov ax, word ptr[a]
    add ax, word ptr[b]
    mov word ptr[sum], ax;  
    
    imul ax;
    mov word ptr[sum_square+2], dx
    mov word ptr[sum_square], ax 
    
    mov ax, 4c00h
    int 21h    
code ends

end start 