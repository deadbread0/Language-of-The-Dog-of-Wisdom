#include <stdio.h>
#define FOR_TYPES
typedef double (*func_ptr)(double, double);

enum typee 
{
    OP,
    VAR, 
    NUM
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