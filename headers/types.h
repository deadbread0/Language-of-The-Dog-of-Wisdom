#include <stdio.h>
#define FOR_TYPES
typedef int (*func_ptr)(int, int);

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
    OP_RETURN,

    OP_STACK,
    REG,
    QBRACKET_OPEN_T,
    QBRACKET_CLOSE_T,
    OP_MOV,
    OP_POINTX2,
    OP_NO_ARGS,
    OP_1_ARG,
    OP_2_ARGS,
    MEM
};
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


struct node_t
{
    typee type;
    operation_t value;
    node_t* left;
    node_t* right;
    node_t* prev;
};  


struct operation_opt_t
{
    typee op_name;
    func_ptr ptr;
};