#ifndef FOR_TYPES
#include "types.h"
#endif
#ifndef FOR_CONST
#include "const.h"
#endif
#include <assert.h>
#include <cstring>
#include <math.h>
#define LEFT_NUM ((node->left)->value).op_num
#define RIGHT_NUM ((node->right)->value).op_num
#define LEFT_TYPE (node->left)->type
#define RIGHT_TYPE (node->right)->type

void FindAmountOfNodes(int* amount, node_t* node);
node_t* ConstantOptimization(node_t* node);
node_t* BodyOfConstantOptimisation(node_t* node);

int AddNum(int num1, int num2);
int MulNum(int num1, int num2);
int SubNum(int num1, int num2);
int DivNum(int num1, int num2);
int DegNum(int num1, int num2);

static operation_opt_t arr_of_op_for_optimisation[] = {{(char*)ADD, AddNum}, {(char*)MUL, MulNum}, {(char*)SUB, SubNum}, {(char*)DIV, DivNum}, {(char*)DEG, DegNum}};