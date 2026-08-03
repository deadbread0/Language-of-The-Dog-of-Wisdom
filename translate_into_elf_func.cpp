#ifndef ASM
#define ASM
#endif

#include "headers/translate_into_elf_func.h"

void FprintfCodeBeforeObjcode(FILE* file_elf, int objcode_size)
{
    assert(file_elf != nullptr);

    fprintf(file_elf, "#include <elf.h>\n#include <stdio.h>\n#include <stdlib.h>\n#include <errno.h>\n\n");

    fprintf(file_elf, "int main(int argc, char* argv[])\n{\n\tElf64_Ehdr header = {\n\t\t.e_ident = {\n\t\t\tELFMAG0,\n\t\t\tELFMAG1,"
        "\n\t\t\tELFMAG2,\n\t\t\tELFMAG3,\n\t\t\tELFCLASS64,\n\t\t\tELFDATA2LSB,\n\t\t\tEV_CURRENT,\n\t\t\tELFOSABI_SYSV,"
        "\n\t\t\t0, 0, 0, 0, 0, 0, 0, 0\n\t\t},\n\t\t.e_type = ET_EXEC,\n\t\t.e_machine = EM_X86_64,"
        "\n\t\t.e_entry = 0x%x, // addr(.text)" 
        "\n\t\t.e_phoff = 64,\n\t\t.e_shoff = 0,\n\t\t.e_flags = 0,\n\t\t.e_ehsize = 64,"
        "\n\t\t.e_phentsize = 56," //всегда 56 для 64-битной системы
        "\n\t\t.e_phnum = 3,\n\t\t.e_shentsize = 64,\n\t\t.e_shnum = 0,\n\t\t.e_shstrndx = SHN_UNDEF\n\t};"

        "\n\tElf64_Phdr phdrs[3] = {\n\t\t//сегмент 0: заголовки ELF\n\t\t{\n\t\t\t.p_type = PT_LOAD,"
        "\n\t\t\t.p_offset = 0x0," 
        "\n\t\t\t.p_vaddr = 0x%x," 
        "\n\t\t\t.p_paddr = 0x%x," 
        "\n\t\t\t.p_filesz = 64 + 3 * 56," 
        "\n\t\t\t.p_memsz = 64 + 3 * 56," 
        "\n\t\t\t.p_flags = PF_R,\n\t\t\t.p_align = 0x1000\n\t\t\t},"
        "\n\t\t//сегмент 1: код (.text)\n\t\t{"
        "\n\t\t\t.p_type = PT_LOAD,"
        "\n\t\t\t.p_offset = 0x%x," 
        "\n\t\t\t.p_vaddr = 0x%x," 
        "\n\t\t\t.p_paddr = 0x%x," 
        "\n\t\t\t.p_filesz = 0x%x," 
        "\n\t\t\t.p_memsz = 0x%x," 
        "\n\t\t\t.p_flags = PF_X | PF_R,\n\t\t\t.p_align = 0x1000\n\t\t\t},"
        "\n\t\t//сегмент 2: данные (.bss)\n\t\t{"
        "\n\t\t\t.p_type = PT_LOAD,"
        "\n\t\t\t.p_offset = 0x0," 
        "\n\t\t\t.p_vaddr = 0x%x," 
        "\n\t\t\t.p_paddr = 0x%x," 
        "\n\t\t\t.p_filesz = 0x0," 
        "\n\t\t\t.p_memsz = 0x20," 
        "\n\t\t\t.p_flags = PF_W | PF_R,\n\t\t\t.p_align = 0x1000\n\t\t\t}\n\t\t};", ADDR_TEXT, ADDR_ELF, ADDR_ELF, 
        ADDR_TEXT - ADDR_ELF, ADDR_TEXT, ADDR_TEXT, objcode_size, objcode_size, ADDR_BUF, ADDR_BUF); //before objcode
}

void FprintfCodeAfterObjcode(FILE* file_elf)
{
    assert(file_elf != nullptr);

    fprintf(file_elf, "\n\tFILE* f = fopen(\"bin\", \"wb\");\n\n\tif (!f)\n\t{\n\t\tperror(\"open\");\n\t\texit(EXIT_FAILURE);\n\t}"
        "\n\n\tsize_t s = fwrite(&header, 1, sizeof(header), f);\n\n\tif (s != sizeof(header))\n\t{\n\t\tperror(\"fwrite\");"
        "\n\t\texit(EXIT_FAILURE);\n\t}"

        "\n\n\ts = fwrite(phdrs, sizeof(Elf64_Phdr), 3, f);\n\n\tif (s != 3)\n\t{\n\t\tperror(\"fwrite\");"
        "\n\t\texit(EXIT_FAILURE);\n\t}"

        "\n\n\tfseek(f, 0x1000, SEEK_SET);"

        "\n\n\ts = fwrite(&objcode, 1, sizeof(objcode), f);\n\n\tif (s != sizeof(objcode))\n\t{\n\n\tperror(\"fwrite\");"
        "\n\t\texit(EXIT_FAILURE);\n\t}\n\n\tfclose(f);\n\n\treturn EXIT_SUCCESS;\n\t}"); //after objcode
}

void FprintfObjcode(FILE* file_elf, int* objcode, int obj_size)
{
    assert(file_elf != nullptr);
    assert(objcode != nullptr);

    fprintf(file_elf, "\n\tchar objcode[] = {");

    for (int i = 0; i < obj_size - 1; i++)
        fprintf(file_elf, "0x%x, ", objcode[i]);

    fprintf(file_elf, "0x%x", objcode[obj_size - 1]);
    fprintf(file_elf, "\n\t};");
}

int* TranslatefObjcode(FILE* file_elf, int* countobj, loops_t* loops_struct)
{
    assert(file_elf != nullptr);
    assert(countobj != nullptr);

    FILE* file_asm = fopen("files/file_asm.asm", "r");

    char* data = PutDataFromFileToBuffer(input_asm_file_path);
    int size = GetSizeOfInputFile(input_asm_file_path);         
    char* data_lib = PutDataFromFileToBuffer(lib_name);
    int size_lib = GetSizeOfInputFile(lib_name);    
                                                                          
    loop_t* loop_num = (loop_t*)calloc(size, sizeof(loop_t)); 
    loops_struct->arr = loop_num;

    node_t* new_tokens = (node_t*)calloc(size + size_lib, sizeof(node_t));
    int* objcodebody = (int*)calloc(size + size_lib, sizeof(int));
    
    int pos_data = 0, pos_lib = 0;
    int amount_of_nodes = 0;
    
    loops_struct->last_func_num = 1; //потому что возникала ошибка, по дефолту это поле для прыжков тоже 0

    bool start = false;
    LexAnalysisAsm(data, &pos_lib, &amount_of_nodes, new_tokens, loops_struct, &start); 
    LexAnalysisAsm(data_lib, &pos_data, &amount_of_nodes, new_tokens, loops_struct, &start); 
    loops_struct->last_index = 0;

    bool for_printf = true;

    for (int i = 0; i < size + size_lib; i++)
    {
        if (new_tokens[i].value.op_name == 0 && new_tokens[i].value.op_num == 0)
            return objcodebody;

            if (new_tokens[i].type == PUSH_T && new_tokens[i + 1].type == REG)
            {
                int reg_id = FindRegId(new_tokens[i + 1].value.op_name);
                objcodebody[(*countobj)] = 0x50 + reg_id;
                (*countobj)++;
                i++;
                continue;
            }

            else if (new_tokens[i].type == POP_T && new_tokens[i + 1].type == REG)
            {
                int reg_id = FindRegId(new_tokens[i + 1].value.op_name);
                objcodebody[(*countobj)] = 0x58 + reg_id;
                (*countobj)++;
                i++;
                continue;
            }

            else if (new_tokens[i].type == PUSH_T && new_tokens[i + 1].type == NUM)
            {
                objcodebody[(*countobj)] = 0x68; 
                (*countobj)++;
                SetNum(countobj, objcodebody, new_tokens[i + 1].value.op_num, 4);
                i++;
            }
                //pop num как будто бы не существует

            else if (new_tokens[i].type == OP_MOV && new_tokens[i + 1].type == REG) //пропустили запятую
            {
                //1) копаемся в стеке
                //2) резервированная память из bss
                //3) mov reg1, reg2
                //4) mov reg, num
                int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);

                if (new_tokens[i + 4].type == REG && new_tokens[i + 3].type == QBRACKET_OPEN_T)
                {
                    int reg2_id = FindRegId(new_tokens[i + 4].value.op_name);
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x8b;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x80 + 8 * reg1_id + reg2_id;
                    (*countobj)++;

                    if (new_tokens[i + 5].type == QBRACKET_CLOSE_T)
                    {
                        SetNum(countobj, objcodebody, 0, 4);
                        i += 5;
                    }
                    
                    else
                    {
                        int num = new_tokens[i + 6].value.op_num;
                        int num_sign = 1;
                        if (new_tokens[i + 5].type == MINUS)
                            num_sign = -1;

                        SetNum(countobj, objcodebody, num * num_sign, 4);
                        i += 7;
                    }

                }

                else if (new_tokens[i + 3].type == MEM) 
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;

                    objcodebody[(*countobj)] = 0xb8 + reg1_id; 
                    (*countobj)++;
                    SetNum(countobj, objcodebody, ADDR_BUF, 4);
                    SetNum(countobj, objcodebody, 0, 4);
                    i += 3;
                    
                } 

                else if (new_tokens[i + 3].type == REG)
                {
                    int reg2_id = FindRegId(new_tokens[i + 3].value.op_name);
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x89;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xc0 + 8 * reg2_id + reg1_id;
                    (*countobj)++;
                    i += 3;
                }

                else if (new_tokens[i + 3].type == NUM)
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xb8 + reg1_id;
                    (*countobj)++;
                    SetNum(countobj, objcodebody, new_tokens[i + 3].value.op_num, 4);
                    SetNum(countobj, objcodebody, 0, 4);
                    i += 3;
                }
                continue;
            }

            else if (new_tokens[i].type == OP_MOV && new_tokens[i + 1].type == QBRACKET_OPEN_T) //пропустили запятую
            {
                objcodebody[(*countobj)] = 0x48;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x89;
                (*countobj)++;

                int reg_id = FindRegId(new_tokens[i + 7].value.op_name);

                if (new_tokens[i + 3].type != QBRACKET_CLOSE_T) 
                {
                    if (new_tokens[i + 4].type == NUM)
                    {
                        int num = new_tokens[i + 4].value.op_num;

                        int num_sign = 1;
                        if (new_tokens[i + 3].type == MINUS)
                            num_sign = -1;

                        objcodebody[(*countobj)] = 0x85 + (8 * reg_id);
                        (*countobj)++;
                        SetNum(countobj, objcodebody, num * num_sign, 4); 
                    }

                    else if (new_tokens[i + 4].type == REG && new_tokens[i + 2].type == MEM) //mov [buf + reg0], reg
                    {
                        int reg0_id = FindRegId(new_tokens[i + 4].value.op_name);
                        objcodebody[(*countobj)] = 0x04 + 8 * reg_id;
                        (*countobj)++;
                        objcodebody[(*countobj)] = 0x05 + 8 * reg0_id;
                        (*countobj)++;
                        SetNum(countobj, objcodebody, ADDR_BUF, 4); 
                    }

                    i += 7;
                }
             
                else 
                {
                    objcodebody[(*countobj)] = 0x45 + (8 * reg_id);
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x00; 
                    (*countobj)++;
                    i += 3;
                }
                continue;
            }

            else if (new_tokens[i].type == XOR_T)
            {
                int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);
                int reg2_id = FindRegId(new_tokens[i + 3].value.op_name);
                
                objcodebody[(*countobj)] = 0x48;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x31;
                (*countobj)++;
                objcodebody[(*countobj)] = 0xc0 + 8 * reg2_id + reg1_id;
                (*countobj)++;
                i += 3;
                
                continue;
            }

            else if (new_tokens[i].type == OP_RETURN)
            {
                objcodebody[(*countobj)] = 0xc3;
                (*countobj)++;
                continue;
            }

            else if (new_tokens[i].type == TEST_T)
            {
                int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);
                int reg2_id = FindRegId(new_tokens[i + 3].value.op_name);

                objcodebody[(*countobj)] = 0x48;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x85;
                (*countobj)++;
                objcodebody[(*countobj)] = 0xc0 + 8 * reg2_id + reg1_id;
                (*countobj)++;
                i += 3;
                continue;
            }

            else if (new_tokens[i].type == CMP_T)
            {
                if (new_tokens[i + 1].type == REG && new_tokens[i + 3].type == REG)
                {
                    int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);
                    int reg2_id = FindRegId(new_tokens[i + 3].value.op_name);

                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x39;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xc0 + 8 * reg2_id + reg1_id;
                    (*countobj)++;
                }

                else if (new_tokens[i + 1].type == REG && new_tokens[i + 3].type == NUM)
                {
                    int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);
                    int num = new_tokens[i + 3].value.op_num;

                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x81;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xf8 + reg1_id;
                    (*countobj)++;
                    SetNum(countobj, objcodebody, num, 4);
                }

                i += 3;
                continue;
            }

            else if (new_tokens[i].type == JMP_T)
            {
                objcodebody[(*countobj)] = 0xe9;
                (*countobj)++;
                SetNum(countobj, objcodebody, 0, 4);
                
                loops_struct->arr[loops_struct->last_index].index_in_objcode = (*countobj); //адрес инструкции, следующий за адресом прыжка 
                loops_struct->last_index++;
                i++; //skip func
                
                continue;
            }

            else if (new_tokens[i].type == JNE_T)
            {
                objcodebody[(*countobj)] = 0x0f;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x85;
                (*countobj)++;
                SetNum(countobj, objcodebody, 0, 4);

                loops_struct->arr[loops_struct->last_index].index_in_objcode = (*countobj); //адрес инструкции, следующий за адресом прыжка 
                loops_struct->last_index++;
                i++; //skip func

                continue;
            }

            else if (new_tokens[i].type == JE_T)
            {
                objcodebody[(*countobj)] = 0x0f;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x84;
                (*countobj)++;
                SetNum(countobj, objcodebody, 0, 4);

                loops_struct->arr[loops_struct->last_index].index_in_objcode = (*countobj); //адрес инструкции, следующий за адресом прыжка 
                loops_struct->last_index++;
                i++; //skip func

                continue;
            }

            else if (new_tokens[i].type == CQTO_T)
            {
                objcodebody[(*countobj)] = 0x48; 
                (*countobj)++;
                objcodebody[(*countobj)] = 0x99; 
                (*countobj)++;
                continue;
            }

            else if (new_tokens[i].type == OP_FUNC)
            {
                loops_struct->arr[loops_struct->last_index].index_in_objcode = *countobj; //финишиндекс
                loops_struct->last_index++;
                continue;
            }

            else if (new_tokens[i].type == SYSCALL_T)
            {
                objcodebody[(*countobj)] = 0x0f;
                (*countobj)++;
                objcodebody[(*countobj)] = 0x05;
                (*countobj)++;
                continue;
            }

            else if (new_tokens[i].type == CALL_T)
            {
                objcodebody[(*countobj)] = 0xe8;
                (*countobj)++;
                SetNum(countobj, objcodebody, 0, 4);
                i++;

                loops_struct->arr[loops_struct->last_index].index_in_objcode = (*countobj); //вставка на -4
                loops_struct->last_index++;

                continue;
            }

            else if (new_tokens[i].type == SUB_T)
            {
                int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);

                if (new_tokens[i + 3].type == NUM) 
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x81;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xe8 + reg1_id;
                    (*countobj)++;
                    SetNum(countobj, objcodebody, new_tokens[i + 3].value.op_num, 4);
                }

                else if (new_tokens[i + 3].type == REG)
                {
                    int reg2_id = FindRegId(new_tokens[i + 3].value.op_name);
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x29; 
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xc0 + 8 * reg2_id + reg1_id;
                    (*countobj)++;
                }

                i += 3;
                continue;
            }

            else if (new_tokens[i].type == ADD_T)
            {
                int reg1_id = FindRegId(new_tokens[i + 1].value.op_name);

                if (new_tokens[i + 3].type == NUM) 
                {
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x81;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xc0 + reg1_id;
                    (*countobj)++;
                    SetNum(countobj, objcodebody, new_tokens[i + 3].value.op_num, 4);
                }

                else if (new_tokens[i + 3].type == REG)
                {
                    int reg2_id = FindRegId(new_tokens[i + 3].value.op_name);
                    objcodebody[(*countobj)] = 0x48;
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0x01; 
                    (*countobj)++;
                    objcodebody[(*countobj)] = 0xc0 + 8 * reg2_id + reg1_id;
                    (*countobj)++;
                }

                i += 3;
                continue;
            }

            else if (new_tokens[i].type == DIV_T)
            {
                int reg_id = FindRegId(new_tokens[i + 1].value.op_name);

                objcodebody[(*countobj)] = 0x48;
                (*countobj)++;
                objcodebody[(*countobj)] = 0xf7; 
                (*countobj)++;
                objcodebody[(*countobj)] = 0xf0 + reg_id;
                (*countobj)++;
                i++;
            }

            else if (new_tokens[i].type == MUL_T)
            {
                int reg_id = FindRegId(new_tokens[i + 1].value.op_name);

                objcodebody[(*countobj)] = 0x48;
                (*countobj)++;
                objcodebody[(*countobj)] = 0xf7; 
                (*countobj)++;
                objcodebody[(*countobj)] = 0xe0 + reg_id;
                (*countobj)++;
                i++;
            }


    }

    free(new_tokens);
    fclose(file_asm);

    return objcodebody;
}

void SetNum(int* countobj, int* objcodebody, int num, int amount_of_bytes) 
{
    assert(countobj != nullptr);
    assert(objcodebody != nullptr);

    for (int i = 0; i < amount_of_bytes; i++)
        objcodebody[(*countobj) + i] = (num >> 8 * i) & 0xff;
    (*countobj) += amount_of_bytes;
}

int FindRegId(char* reg)
{
    assert(reg != nullptr);

    for (int j = 0; j < amount_of_regs; j++)
    {
        if (strncmp(reg, REGS[j], strlen(REGS[j])) == 0)
            return j;
    }

    return 0; //default rax
}

void FillJMPs(int* objcodebody, loops_t* loops_struct)
{
    assert(objcodebody != nullptr);
    assert(loops_struct != nullptr);

    for (int i = 0; i < loops_struct->last_index; i++)
    {
        loop_t* loop_arr = loops_struct->arr;
        if (loop_arr[i].addr_start)
        {
            int addr_finish = 0;

                addr_finish = FindCallFinish(loops_struct, loop_arr[i].func_struct.num_of_func);

                int index = loop_arr[i].index_in_objcode; //начало call, вставка на +1
                int diff = addr_finish - index; //пропускаем длину кода call
             
                index -= 4;
                SetNum(&index, objcodebody, diff, 4);
            
        }
    }
}

int FindCallFinish(loops_t* loops_struct, int call_num)
{
    assert(loops_struct != nullptr);

    for (int i = 0; i < loops_struct->last_index; i++)
    {
        if (!loops_struct->arr[i].addr_start && loops_struct->arr[i].func_struct.num_of_func == call_num)
            return loops_struct->arr[i].index_in_objcode;
    }
    fprintf(stderr, "call finish error\n");
    return 0;
}