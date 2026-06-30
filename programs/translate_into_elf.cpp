#include "../headers/translate_into_elf.h"


int main()
{
    FILE* file_elf = fopen("files/file_elf.c", "w");

    // fprintf(file_elf, "\n\tchar objcode[] = {"
    //     "0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xa, // Hello!/n
    //     0xb8, // mov rax (32 bit)
    //     1, 0, 0, 0, // write syscall 1
    //     0xbf, // mov rdi (32 bit)
    //     1, 0, 0, 0, // stdout
    //     0x48, 0xbe, // mov rsi, 64 bit pointer
    //     0x78, 0, 0x40, 0, 0, 0, 0, 0, // Hello strings address in virtual memory
    //     0xba, // mov rdx (32 bit)
    //     7, 0, 0, 0, //number of bytes in Hello!\n
    //     0xf, 0x5, // syscall
    //     0xb8, //mov rax (32 bit)
    //     0x3c, 0, 0, 0, // 60 = exit syscall
    //     0x48, 0x31, 0xff, // xor rdx, rdx
    //     0xf, 0x5 // syscall"
    //     "\n\t};"); //objcode

    int objcode_size = 0;
    int* objcode = TranslatefObjcode(file_elf, &objcode_size);

    FprintfCodeBeforeObjcode(file_elf, objcode_size);
    FprintfObjcode(file_elf, objcode, objcode_size);
    FprintfCodeAfterObjcode(file_elf);  

}

void FprintfCodeBeforeObjcode(FILE* file_elf, int objcode_size)
{
    fprintf(file_elf, "#include <elf.h>\n#include <stdio.h>\n#include <stdlib.h>\n#include <errno.h>\n\n");

    fprintf(file_elf, "int main(int argc, char* argv[])\n{\n\tElf64_Ehdr header = {\n\t\t.e_ident = {\n\t\t\tELFMAG0,\n\t\t\tELFMAG1,"
        "\n\t\t\tELFMAG2,\n\t\t\tELFMAG3,\n\t\t\tELFCLASS64,\n\t\t\tELFDATA2LSB,\n\t\t\tEV_CURRENT,\n\t\t\tELFOSABI_SYSV,"
        "\n\t\t\t0, 0, 0, 0, 0, 0, 0, 0\n\t\t},\n\t\t.e_type = ET_EXEC,\n\t\t.e_machine = EM_X86_64,"
        "\n\t\t.e_entry = 0x40007f, // 8 + 7 = f" 
        "\n\t\t.e_phoff = 64,\n\t\t.e_shoff = 0,\n\t\t.e_flags = 0,\n\t\t.e_ehsize = 64,"
        "\n\t\t.e_phentsize = 56," //всегда 56 для 64-битной системы
        "\n\t\t.e_phnum = 1,\n\t\t.e_shentsize = 64,\n\t\t.e_shnum = 0,\n\t\t.e_shstrndx = SHN_UNDEF\n\t};"

        "\n\tElf64_Phdr phdr = {\n\t\t.p_type = PT_LOAD,"
        "\n\t\t.p_offset = 0x78, // 64 + 56 = 120" 
        "\n\t\t.p_vaddr = 0x400078," 
        "\n\t\t.p_paddr = 0x400078," 
        "\n\t\t.p_filesz = %d," //размер objcode
        "\n\t\t.p_memsz = %d," //размер objcode + 32 (из .bss)
        "\n\t\t.p_flags = PF_X | PF_R,\n\t\t.p_align = 0x8\n\t};", objcode_size, objcode_size + 32); //before objcode
}

void FprintfCodeAfterObjcode(FILE* file_elf)
{
    fprintf(file_elf, "\n\tFILE* f = fopen(\"bin\", \"w\");\n\n\tif (!f)\n\t{\n\t\tperror(\"open\");\n\t\texit(EXIT_FAILURE);\n\t}"
        "\n\n\tsize_t s = fwrite(&header, 1, sizeof(header), f);\n\n\tif (s != sizeof(header))\n\t{\n\t\tperror(\"fwrite\");"
        "\n\t\texit(EXIT_FAILURE);\n\t}"

        "\n\n\ts = fwrite(&phdr, 1, sizeof(phdr), f);\n\n\tif (s != sizeof(phdr))\n\t{\n\t\tperror(\"fwrite\");"
        "\n\t\texit(EXIT_FAILURE);\n\t}"

        "\n\n\ts = fwrite(&objcode, 1, sizeof(objcode), f);\n\n\tif (s != sizeof(objcode))\n\t{\n\n\tperror(\"fwrite\");"
        "\n\t\texit(EXIT_FAILURE);\n\t}\n\n\tfclose(f);\n\n\treturn EXIT_SUCCESS;\n\t}"); //after objcode
}

void FprintfObjcode(FILE* file_elf, int* objcode, int obj_size)
{
    fprintf(file_elf, "\n\tchar objcode[] = {");

    for (int i = 0; i < obj_size; i++)
        fprintf(file_elf, "%x, ", objcode[i]);

    fprintf(file_elf, "\n\t};");
}

int* TranslatefObjcode(FILE* file_elf, int* countobj)
{
    FILE* file_asm = fopen("files/file_asm.asm", "r");

    char* data = PutDataFromFileToBuffer(input_asm_file_path);
    int size = GetSizeOfInputFile(input_asm_file_path);
    node_t* new_tokens = (node_t*)calloc(size, sizeof(node_t));
    int* objcodebody = (int*)calloc(size, sizeof(int));
    
    int pos = 0;
    LexAnalysisAsm(data, &pos, new_tokens);

    for (int i = 0; i < size; i++)
    {
        if (new_tokens[i].type == OP_STACK && new_tokens[i + 1].type == REG)
        {
            if (strncmp(new_tokens[i].value.op_name, PUSH, strlen(PUSH)) == 0)
            {
                for (int j = 0; j < amount_of_regs; j++)
                {
                    if (strncmp(new_tokens[i + 1].value.op_name, REGS[j], strlen(REGS[j])) == 0)
                    {
                        objcodebody[(*countobj)] = 0x50 + j;
                        (*countobj)++;
                        i++;
                        break;
                    }
                }
            }

            if (strncmp(new_tokens[i].value.op_name, POP, strlen(POP)) == 0)
            {
                for (int j = 0; j < amount_of_regs; j++)
                {
                    if (strncmp(new_tokens[i + 1].value.op_name, REGS[j], strlen(REGS[j])) == 0)
                    {
                        objcodebody[(*countobj)] = 0x58 + j;
                        (*countobj)++;
                        i++;
                        break;
                    }
                }
            }

            if (new_tokens[i].type == OP_STACK && new_tokens[i + 1].type == NUM)
            {
                if (strncmp(new_tokens[i].value.op_name, PUSH, strlen(PUSH)) == 0)
                {
                    if (new_tokens[i + 1].value.op_num < 256) //надо бы для чисел побольше тоже сделать
                    {
                        objcodebody[(*countobj)] = 0x6A; 
                        (*countobj)++;
                        objcodebody[(*countobj)] = new_tokens[i + 1].value.op_num; //NUM
                    }
                    
                    (*countobj)++;
                    i++;
                }
                //pop num как будто бы не существует
            }

            //mov reg, num: (48(64 бит)) (B8+номер_регистра) (num(64 бит))
            //mov rax, [rbp - 8*2]: (48(64 бит)) (8B) (01_XXX_101, xxx-reg id) (-16 d 8-битном представлении) xxx - код регистра-приемника, 101-rbp
            //mov [RBP - 16], reg: (48) (89) (01_XXX_101, xxx-reg id) (-16 d 8-битном представлении)

            if (new_tokens[i].type == OP_MOV && new_tokens[i + 1].type == REG && new_tokens[i + 3].type == NUM) //пропустили запятую
            {
                objcodebody[(*countobj)] = 0x48; //постоянно для 64бит
                (*countobj)++;
                objcodebody[(*countobj)] = 0xb8 + FindRegId(new_tokens[i].value.op_name);
                (*countobj)++;

                objcodebody[(*countobj)] = new_tokens[i + 3].value.op_num; //тут опять для простоты num < 256
                objcodebody++;
                for (int p = 0; p < 3; p++)
                {
                    objcodebody[(*countobj)] = 0xb8 + FindRegId(new_tokens[i].value.op_name);
                    (*countobj)++;
                }

                i += 3;
            }

            if (new_tokens[i].type == OP_MOV && new_tokens[i + 1].type == REG && new_tokens[i + 3].type == QBRACKET_OPEN_T) //пропустили запятую
            {
                //1) копаемся в стеке
                //2) резервированная память из bss
                if (new_tokens[i + 4].type == REG)
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x8b;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0b01000101 + 8 * FindRegId(new_tokens[i + 1].value.op_name);
                    (*countobj)++;

                    if (new_tokens[i + 5].type == MINUS)
                        objcodebody[(*countobj)] = (-1) * new_tokens[i + 6].value.op_num;  
                    else if (new_tokens[i + 5].type == PLUS)
                        objcodebody[(*countobj)] = new_tokens[i + 6].value.op_num;  
                    else if (new_tokens[i + 5].type == NUM)
                        objcodebody[(*countobj)] = new_tokens[i + 5].value.op_num; 
                    else
                        objcodebody[(*countobj)] = 0x00; 

                }

                if (new_tokens[i + 4].type == MEM)
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xbe; //rsi
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x00;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x20;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x40;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x00;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x00;
                    (*countobj)++;
                } 
            }

            if (new_tokens[i].type == OP_MOV && new_tokens[i + 1].type == QBRACKET_OPEN_T) //пропустили запятую
            {
                objcodebody[(*countobj)] = 0x48;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x89;
                (*countobj)++;

                if (new_tokens[i + 3].type == MINUS)
                {
                    objcodebody[(*countobj)] = 0b01000101 + 8 * FindRegId(new_tokens[i + 7].value.op_name);
                    (*countobj)++;
                    objcodebody[(*countobj)] = (-1) * new_tokens[i + 4].value.op_num;
                    (*countobj)++;
                    i += 8;
                }
                else if (new_tokens[i + 3].type == PLUS)
                {
                    objcodebody[(*countobj)] = 0b01000101 + 8 * FindRegId(new_tokens[i + 7].value.op_name);
                    (*countobj)++;
                    objcodebody[(*countobj)] = new_tokens[i + 4].value.op_num;
                    (*countobj)++;
                    i += 8;
                }
                else if (new_tokens[i + 3].type == QBRACKET_CLOSE_T)
                {
                    objcodebody[(*countobj)] = 0b01000101 + 8 * FindRegId(new_tokens[i + 7].value.op_name);
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x00;
                    (*countobj)++;
                    i += 3;
                }
            }

            if (strncmp(new_tokens[i].value.op_name, XOR, strlen(XOR)) == 0)
            {
                if (strncmp(new_tokens[i + 1].value.op_name, new_tokens[i + 3].value.op_name, strlen(RAX)) == 0) //reg1 == reg2
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x31;
                    (*countobj)++;
                    int reg_id = FindRegId(new_tokens[i + 1].value.op_name);
                    objcodebody[(*countobj)] = 0b11000000 + 8 * reg_id + reg_id;
                    (*countobj)++;
                    i += 3;
                }
            }

            if (strncmp(new_tokens[i].value.op_name, SYSCALL, strlen(SYSCALL)) == 0)
            {
                objcodebody[(*countobj)] = 0x0f;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x05;
                (*countobj)++;
                i+=2;
            }


            if (new_tokens[i].type == OP_PRINTF) //дада я сделаю это честно, но пока для проверки так 
            {
                objcodebody[(*countobj)] = 0x50; //push rax
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //xor rdx, rdx
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x31; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xd2; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //xor rdi, rdi
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x31; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xff; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //cqto
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x99; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0xbb; //mov rbx, 10
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x0a; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x00; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x00; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x00; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //div rbx
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xf7; 
                (*countobj)++;
                i++;objcodebody[(*countobj)] = 0xf3; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //add rdx, 30h
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x83; 
                (*countobj)++;
                i++;objcodebody[(*countobj)] = 0xc2; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x30; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x52; //push rdx
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //inc rdi
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xff; 
                (*countobj)++;
                i++;objcodebody[(*countobj)] = 0xc7; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //cqto
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x99; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //cmp rax, 0
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x83; 
                (*countobj)++;
                i++;objcodebody[(*countobj)] = 0xf8; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x00; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x74; //je NumIsOver //кстати загадочно.. почему не меняются команды при изменении кол-ва инструкций
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x02; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0xe2; //loop MakeDigit
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xe6; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //mov rcx, rdi
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x89; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xf9; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //mov rdx, rdi
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x89; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xfa; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //xor rdi, rdi
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x31; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xff; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x5a; //pop rdx
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x88; //mov byte [BUF_FOR_PRINTF + rdi], dl
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x97; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x00; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x20; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x40; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0x00; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x48; //inc rdi
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xff; 
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xc7; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0xe2; //loop FillBuf
                (*countobj)++;
                i++;
                objcodebody[(*countobj)] = 0xf4; 
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0x58; //pop rax
                (*countobj)++;
                i++;

                objcodebody[(*countobj)] = 0xc3; //ret
                (*countobj)++;
                // i++;
            }
    }


}
return objcodebody;
}

int FindRegId(char* reg)
{
    for (int j = 0; j < amount_of_regs; j++)
    {
        if (strncmp(reg, REGS[j], strlen(REGS[j])) == 0)
            return j;
    }

    return 0; //default rax
}