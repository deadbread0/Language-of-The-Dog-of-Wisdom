#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <cstring>

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
static const char* CQO = "cqo";
static const char* CMP = "cmp";
static const char* COMMA = ",";
static const char* INC = "inc";
static const char* DEC = "dec";
static const char* LOOP = "loop";
static const char* JE = "je";
static const char* JNE = "jne";
static const char* RET = "ret";
static const char* REMAKE10 = "remake_nums10";
#define LEX_ASM
#endif

size_t LexAnalysisAsm(char* s, int* pos, node_t* tokens);
bool CompareWords(char* s, int* pos, char* word);