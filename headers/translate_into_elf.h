#include <stdio.h>
#include "input.h"
#include "lex_analysis_asm.h"

void FprintfCodeBeforeObjcode(FILE* file_elf, int objcode_size);
void FprintfCodeAfterObjcode(FILE* file_elf);
int* TranslatefObjcode(FILE* file_elf, int* countobj);
int FindRegId(char* reg);
void FprintfObjcode(FILE* file_elf, int* objcode, int obj_size);

static const char* REGS[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"};
static int amount_of_regs = sizeof(REGS) / sizeof(REGS[0]);

static const char* input_asm_file_path = "files/file_asm.asm";