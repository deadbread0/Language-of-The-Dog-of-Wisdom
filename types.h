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
    COMP
};

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
    int num;
    char* var;
    int last_i;
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