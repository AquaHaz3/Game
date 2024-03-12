stack segment para stack
db 256 dup(?)
stack ends

data segment para public
buffer0 db 242 dup("?")
buffer1 db 242 dup("?")
buffer2 db 242 dup("?")
data ends

code segment para public

assume cs:code,ds:data,ss:stack

start:
	
	mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
	
	mov ah, 0ah
    mov bx, offset buffer0
    mov byte ptr [bx], 240
    mov dx, bx
    int 21h
	mov ah, 0ah
	mov cl, byte ptr [bx+1]
	mov di, cx
	mov byte ptr [bx + di + 2], 0ah
	mov byte ptr [bx + di + 3], '$'
	
	mov dl, ah
	mov ah, 02h
	int 21h
	
	mov ah, 0ah
    mov bx, offset buffer1
    mov byte ptr [bx], 240
    mov dx, bx
    int 21h
	mov cl, byte ptr [bx+1]
	mov di, cx
	mov byte ptr [bx + di + 2], 0ah
	mov byte ptr [bx + di + 3], '$'
	
	mov dl, ah
	mov ah, 02h
	int 21h
	
	mov ah, 0ah
    mov bx, offset buffer2
    mov byte ptr [bx], 240
    mov dx, bx
    int 21h
	mov cl, byte ptr [bx+1]
	mov di, cx
	mov byte ptr [bx + di + 2], 0ah
	mov byte ptr [bx + di + 3], '$'
	
	mov dl, ah
	mov ah, 02h
	int 21h
	
	mov ah, 09h
    mov dx, [offset buffer0 + 2]
    int 21h
	mov dx, [offset buffer1 + 2]
    int 21h
	mov dx, [offset buffer2 + 2]
    int 21h
 
    mov ax, 4C00h      ;
    int 21h           ; Завершение программы

code ends

end start