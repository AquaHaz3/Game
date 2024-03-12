stack segment para stack
db 256 dup(?)
stack ends

data segment para public

len0 db 0, 0
buffer0 db 242 dup("?")
len1 db 0, 0
buffer1 db 242 dup("?")
len2 db 0, 0
buffer2 db 242 dup("?")

end0 dw 0
end1 dw 0
end2 dw 0

new_line db 0ah,0dh,'$'

data ends

code segment para public

assume cs:code,ds:data,ss:stack

start:
	
	mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
	
	mov ah, 0ah
    mov byte ptr [len0], 240
    mov dx, offset len0
    int 21h
	mov ah, 0ah
	mov cl, byte ptr [len0+1]
	mov di, cx
	mov end0, di
	mov bx, offset buffer0
	mov byte ptr [bx + di], ' '
	mov byte ptr [bx + di + 1], '$'
	
	mov dl, ah
	mov ah, 02h
	int 21h
	
	mov ah, 0ah
    mov byte ptr [len1], 240
    mov dx, offset len1
    int 21h
	mov ah, 0ah
	mov cl, byte ptr [len1+1]
	mov di, cx
	mov end1, di
	mov bx, offset buffer1
	mov byte ptr [bx + di], ' '
	mov byte ptr [bx + di + 1], '$'
	
	mov dl, ah
	mov ah, 02h
	int 21h
	
	mov ah, 0ah
    mov byte ptr [len2], 240
    mov dx, offset len2
    int 21h
	mov ah, 0ah
	mov cl, byte ptr [len2+1]
	mov di, cx
	mov end2, di
	mov bx, offset buffer2
	mov byte ptr [bx + di], ' '
	mov byte ptr [bx + di + 1], '$'
	
	mov dl, ah
	mov ah, 02h
	int 21h
	
	mov ah, 09h
    mov dx, offset buffer0
    int 21h
	mov dx, offset buffer1
    int 21h
	mov dx, offset buffer2
    int 21h
	mov dx, offset new_line
	int 21h
	
	mov di, end0
	mov byte ptr [di + offset buffer0], ';'
	mov di, end1
	mov byte ptr [di + offset buffer1], ';'
	mov di, end2
	mov byte ptr [di + offset buffer2], ';'
	
    mov dx, offset buffer0
    int 21h
	mov dx, offset buffer1
    int 21h
	mov dx, offset buffer2
    int 21h
	mov dx, offset new_line
	int 21h
	
	mov di, end0
	mov byte ptr [di + offset buffer0], 0ah
	mov di, end1
	mov byte ptr [di + offset buffer1], 0ah
	mov di, end2
	mov byte ptr [di + offset buffer2], 0ah
	
	mov dx, offset buffer0
    int 21h
	mov dx, offset buffer1
    int 21h
	mov dx, offset buffer2
    int 21h
 
    mov ax, 4C00h      ;
    int 21h           ; Завершение программы

code ends

end start