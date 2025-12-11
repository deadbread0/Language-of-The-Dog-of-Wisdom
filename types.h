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
    OP_IF,
    OP_EQUAL,
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
    char* op_name;
    func_ptr ptr;
};