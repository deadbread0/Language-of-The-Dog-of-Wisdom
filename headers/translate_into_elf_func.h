#include <stdio.h>
#include "input.h"
#include "free_memory.h"

#ifndef LEX_ASM
#include "lex_analysis_asm.h"
#endif

static const char* lib_name = "files/lib_asm.asm";

void FprintfCodeBeforeObjcode(FILE* file_elf, int objcode_size);
void FprintfCodeAfterObjcode(FILE* file_elf);
int* TranslatefObjcode(FILE* file_elf, int* countobj, loops_t* loops_struct);
int FindRegId(char* reg);
void FprintfObjcode(FILE* file_elf, int* objcode, int obj_size);
int FindCallFinish(loops_t* loops_struct, int call_num);
void FillJMPs(int* objcodebody, loops_t* loops_struct);
void SetNum(int* countobj, int* objcodebody, int num, int amount_of_bytes);

static const char* REGS[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"};
static int amount_of_regs = sizeof(REGS) / sizeof(REGS[0]);

static const char* input_asm_file_path = "files/file_asm.asm";
static const int ADDR_ELF = 0x400000, ADDR_TEXT = 0x401000, ADDR_BUF = 0x402000;