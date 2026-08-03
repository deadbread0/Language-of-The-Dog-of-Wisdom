#include <stdio.h>
#define FOR_TYPES
typedef int (*func_ptr)(int, int);

#ifndef ASM
enum typee 
{
    OP,
    VAR, 
    NUM, 
    UOP,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    FBRACKET_OPEN,
    FBRACKET_CLOSE,
    OP_IF,
    OP_EQUAL,
    END,
    MINUS,
    PLUS,
    MULT,
    DIVN,
    OP_FUNC,
    OP_WHILE,
    DEGREE,
    COMP,
    OP_PRINTF,
    MAIN_FUNC,
    OP_RETURN
};
#endif

#ifdef ASM
enum typee 
{
    NUM, 
    COMMA_T,
    MINUS,
    PLUS,
    MULT,
    DIVN,
    OP_FUNC,
    OP_PRINTF,
    OP_RETURN,
    REG,
    QBRACKET_OPEN_T,
    QBRACKET_CLOSE_T,
    OP_MOV,
    OP_POINTX2,
    MEM,
    CALL_T,
    SYSCALL_T,
    SUB_T,
    ADD_T,
    MUL_T,
    DIV_T,
    POP_T,
    PUSH_T,
    XOR_T,
    TEST_T,
    JE_T,
    JNE_T,
    JMP_T,
    CQTO_T,
    CMP_T
};
#endif

static const int MAX_LEN_OF_OPERATION = 10;

struct typee_t
{
    char* t_name;
    func_ptr t_func;
};

union operation_t
{
    double op_num;
    char* op_name;
};

struct names_t
{
    int num_of_name;
    char* var;
    int value;
    int num_in_stack; //показывает, сколько всякого закинули после этого элемента в стек
};

#ifndef ASM
struct node_t
{
    typee type;
    operation_t value;
    node_t* left;
    node_t* right;
    node_t* prev;
};  
#endif

#ifdef ASM
struct node_t
{
    typee type;
    operation_t value;
};  
#endif


struct operation_opt_t
{
    typee op_name;
    func_ptr ptr;
};