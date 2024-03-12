stack segment para stack
db 256 dup(?)
stack ends

data segment para public
buffer db 242 dup("?")
data ends

code segment para public

assume cs:code,ds:data,ss:stack

start:
	
	mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
	
	mov ah, 3Fh  
    mov dx, offset buffer  
    mov cx, 240
    int 21h   
	
	mov cx, ax; length of readed string to CX
    mov ah, 40h  
    mov dx, offset buffer   
    int 21h
 
    mov ax, 4C00h      ;
    int 21h           ; Завершение программы

code ends

end start