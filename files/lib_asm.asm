printf_num:                         ;через rdi параметр прилетает
		push rax
        mov rax, rdi
		xor rdx, rdx
		xor rdi, rdi
		cqto                         ;там при делении надо чтоб размеры делителя и частного определенные были,                            ;так вот это для расширения rax

		MakeDigit:
		mov rbx, 10                 ;на это делить надо
		div rbx                     ;остаток от деления в rdx

		add rdx, 48                ;цифра -> буква
		push rdx                    ;кладется на стек

		add rdi, 1
		cqto

		cmp rax, 0
		je NumIsOver                ;если число закончилось, заканчиваем вынос цифр в стек

		jmp MakeDigit

		NumIsOver:
		mov rcx, rdi                ;в rdi кол-во цифр из прошлого цикла
		mov rdx, rdi
		xor rdi, rdi
		FillBuf:
		pop rdx       
		mov [BUF_FOR_PRINTF + rdi], rdx
		add rdi, 1
        sub rcx, 1
        cmp rcx, 0
		jne FillBuf

        mov rax, 1
        mov rdi, 1
        mov rsi, BUF_FOR_PRINTF
        syscall

		pop rax

		ret 