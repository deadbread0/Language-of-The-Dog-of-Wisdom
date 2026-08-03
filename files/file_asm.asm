; nasm -f elf64 -l files/file_asm.lst files/file_asm.asm
; ld -s -o files/file_asm.exe files/file_asm.o
; ./files/file_asm.exe

global _start
section .bss
BUF_FOR_PRINTF: resb 32
section .data
section .text

_start:


push rbp
mov rbp, rsp				;start main func
sub rsp, 256

push 1				;num
push 5				;num
pop rax
mov [rbp - 8], rax				;var = t
mov rax, [rbp - 8]				;var = t
push rax

pop rax				;забрать параметр
call func

push rax				;функция с возвращаемым значением
pop rax
mov [rbp - 16], rax				;var = g
mov rax, [rbp - 16]				;var = g
push rax

pop rax				;все передаем через стек ага
mov rdi, rax				;передача аргумента на печать
call printf_num

pop rax
pop rbp ;from main

mov rax, 60      ; exit64 (rdi)
xor rdi, rdi
syscall

func:
push rbp
mov rbp, rsp
sub rsp, 256

mov [rbp - 8], rax				;параметр сюда
mov rax, [rbp - 8]				;var = x
push rax
mov rax, [rbp - 8]				;var = x
push rax
push 1				;num

pop rbx
pop rax
sub rax, rbx
push rax

pop rax
mov [rbp - 8], rax				;var = x
mov rax, [rbp - 8]				;var = x
push rax

pop rax
test rax, rax
je loop0
mov rax, [rbp - 8]				;var = x
push rax

pop rax				;забрать параметр
call func

push rax				;функция с возвращаемым значением
pop rax
mov [rbp - 16], rax				;var = f
mov rax, [rbp - 8]				;var = x
push rax
push 1				;num

pop rax
pop rbx
add rax, rbx
push rax

mov rax, [rbp - 16]				;var = f
push rax

pop rax
pop rbx
mul rbx
push rax

pop rax
mov [rbp - 24], rax				;var = k
mov rax, [rbp - 24]				;var = k
push rax
pop rax				;возвращаемый параметр

pop rbx
add rsp, 256
pop rbp
ret

loop0:
push 1				;num
pop rax				;возвращаемый параметр

pop rbx
add rsp, 256
pop rbp
ret

pop rbx
add rsp, 256
pop rbp
ret
