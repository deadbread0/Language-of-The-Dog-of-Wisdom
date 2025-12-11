#ifndef FOR_TYPES
#include "types.h"
#endif
// #include "const.h"
#include <assert.h>
#ifndef LEX
#include "lex_analysis.h"
#endif
#include "functions.h"



node_t* GetNodeG(char* s, int* pos);
node_t* GetNodeN(char* s, int* pos);
node_t* GetNodeE(char* s, int* pos);
node_t* GetNodeT(char* s, int* pos);
node_t* GetNodeP(char* s, int* pos);
node_t* GetNodeM(char* s, int* pos);
node_t* GetNodeV(char* s, int* pos);
node_t* GetNodeF(char* s, int* pos, char* buf);
node_t* GetNodeA(char* s, int* pos);
node_t* GetNodeIF(char* s, int* pos);
void GetNodeOP(char* s, int* pos, node_t* val, node_t* prev_val);
node_t* MakeNode();
node_t* NewOpNode(char* operation, node_t* left_node, node_t* right_node);
node_t* NewNumNode(double num, node_t* left_node, node_t* right_node);
node_t* NewVarNode(char* var, node_t* left_node, node_t* right_node);
