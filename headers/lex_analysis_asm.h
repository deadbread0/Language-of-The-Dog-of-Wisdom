#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <cstring>

#ifndef ASM
#define ASM
#endif

#ifndef FOR_TYPES
#include "types.h"
#endif

#ifndef FOR_FUNC
#include "functions.h"
#endif 

#ifndef LEX_ASM
static const char* PUSH = "push";
static const char* POP = "pop";
static const char* RAX = "rax";
static const char* RBX = "rbx";
static const char* RCX = "rcx";
static const char* RDX = "rdx";
static const char* DL = "dl";
static const char* RBP = "rbp";
static const char* RSP = "rsp";
static const char* RDI = "rdi";
static const char* RSI = "rsi";
static const char* MOV = "mov";
static const char* QBRACKET_OPEN = "[";
static const char* QBRACKET_CLOSE = "]";
static const char* POINTX2 = ":";
static const char* CALL = "call";
static const char* SYSCALL = "syscall";
static const char* ADD = "add";
static const char* SUB = "sub";
static const char* DIV = "div";
static const char* MUL = "mul";
static const char* ADD_SHORT = "+";
static const char* SUB_SHORT = "-";
static const char* DIV_SHORT = "/";
static const char* MUL_SHORT = "*";
static const char* BUF_FOR_PRINTF = "BUF_FOR_PRINTF";
static const char* XOR = "xor";
static const char* CQTO = "cqto";
static const char* CMP = "cmp";
static const char* COMMA = ",";
static const char* INC = "inc";
static const char* DEC = "dec";
static const char* JE = "je";
static const char* JNE = "jne";
static const char* JMP = "jmp";
static const char* TEST = "test";
static const char* RET = "ret";
static const char* REMAKE10 = "remake_nums10";
static const char* START = "_start";
#define LEX_ASM
#endif

struct func_t {
    char* name_of_func;
    int num_of_func;
};

struct loop_t {
    bool addr_start; 
    int index_in_objcode; 
    func_t func_struct;
};

struct loops_t {
    loop_t* arr; //тк мб из 2х разных мест прыжок в одно
    int last_index;
    int last_func_num;
};

size_t LexAnalysisAsm(char* s, int* pos, int* amount_of_nodes, node_t* tokens, loops_t* loop_num, bool* start);
bool CompareWords(char* s, int* pos, char* word);
int FindNumOfFunc(char* func_name, loops_t* loop_struct);
