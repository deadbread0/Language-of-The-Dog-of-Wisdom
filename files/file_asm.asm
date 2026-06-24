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
sub rsp, 8 * 32
push 1				;num
push 6				;num
pop rax
mov [rbp - 8 * 1], rax				;var = t
push 9				;num
pop rax
mov [rbp - 8 * 2], rax				;var = y
mov rax, [rbp - 8 * 1]				;var = t
push rax

pop rax
mov rbx, 0
cmp rax, rbx
je loop0
push 0				;num
pop rax
mov [rbp - 8 * 1], rax				;var = t
mov rax, [rbp - 8 * 1]				;var = t
push rax

pop rax
mov rbx, 0
cmp rax, rbx
je loop1
push 7				;num
pop rax
mov [rbp - 8 * 2], rax				;var = y

loop1:

loop0:
mov rax, [rbp - 8 * 2]				;var = y
push rax
mov rax, [rbp - 8 * 1]				;var = t
push rax

pop rax
pop rbx
add rax, rbx
push rax

pop rax
mov [rbp - 8 * 3], rax				;var = g
push 1				;num
pop rax
mov [rbp - 8 * 4], rax				;var = v
mov rax, [rbp - 8 * 3]				;var = g
push rax

call remake_nums10

mov rax, 0x01
mov rdi, 1
mov rsi, BUF_FOR_PRINTF
syscall


pop rax
pop rbp ;from main

mov rax, 0x3C      ; exit64 (rdi)
xor rdi, rdi
syscall


remake_nums10:
		push rax
		xor rdx, rdx
		xor rdi, rdi
		cqo                         ;там при делении надо чтоб размеры делителя и частного определенные были,                            ;так вот это для расширения rax

		MakeDigit:
		mov rbx, 10                 ;на это делить надо
		div rbx                     ;остаток от деления в rdx

		add rdx, 30h                ;цифра -> буква
		push rdx                    ;кладется на стек

		inc rdi
		cqo

		cmp rax, 0
		je NumIsOver                ;если число закончилось, заканчиваем вынос цифр в стек

		loop MakeDigit

		NumIsOver:
		mov rcx, rdi                ;в rdi кол-во цифр из прошлого цикла
		mov rdx, rdi
		xor rdi, rdi
		FillBuf:
		pop rdx
		mov byte [BUF_FOR_PRINTF + rdi], dl
		inc rdi
		loop FillBuf

		pop rax

		ret
