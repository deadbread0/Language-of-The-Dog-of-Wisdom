#ifndef FOR_TYPES
#include "types.h"
#endif
// #include "const.h"
#include <assert.h>
#ifndef LEX
#include "lex_analysis.h"
#endif
#include "functions.h"



node_t* GetNodeG(node_t* tokens, int* pos);
node_t* GetNodeN(node_t* tokens, int* pos);
node_t* GetNodeE(node_t* tokens, int* pos);
node_t* GetNodeT(node_t* tokens, int* pos);
node_t* GetNodeP(node_t* tokens, int* pos);
node_t* GetNodeM(node_t* tokens, int* pos);
node_t* GetNodeV(node_t* tokens, int* pos);
node_t* GetNodeF(node_t* tokens, int* pos);
node_t* GetNodeA(node_t* tokens, int* pos);
node_t* GetNodeIF(node_t* tokens, int* pos);
// void GetNodeOP(char* s, int* pos, node_t* val, node_t* prev_val);
node_t* MakeNode();
node_t* NewOpNode(char* operation, node_t* left_node, node_t* right_node);
node_t* NewNumNode(double num, node_t* left_node, node_t* right_node);
node_t* NewVarNode(char* var, node_t* left_node, node_t* right_node);
