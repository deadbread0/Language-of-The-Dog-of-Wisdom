global _start
section .bss
BUF_FOR_PRINTF: resb 4
section .data
section .text
_start:

push rbp
mov rbp, rsp				;start main func
push 1				;num
push 6				;num
mov [rbp - 8 * 1], rax				;var = t

pop rax
add rax, 48				;make symb from num
mov [BUF_FOR_PRINTF], rax

mov rax, 0x01
mov rdi, 1
mov rsi, BUF_FOR_PRINTF
mov rdx, 1
syscall

mov rax, [BUF_FOR_PRINTF]
sub rax, 48				;make num from symb
mov [BUF_FOR_PRINTF], rax


pop rbp ;from main

mov rax, 0x3C      ; exit64 (rdi)
xor rdi, rdi
syscall
