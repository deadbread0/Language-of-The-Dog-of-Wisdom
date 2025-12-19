#ifndef FOR_TYPES
#include "types.h"
#endif
// #include "const.h"
#include <assert.h>
#ifndef LEX
#include "lex_analysis.h"
#endif
#include "functions.h"



node_t* GetNodeComb(node_t* tokens, int* pos);
node_t* GetNodeNum(node_t* tokens, int* pos);
node_t* GetNodeAddOrSub(node_t* tokens, int* pos);
node_t* GetNodeMulOrDiv(node_t* tokens, int* pos);
node_t* GetNodeBracket(node_t* tokens, int* pos);
node_t* GetNodeM(node_t* tokens, int* pos);
node_t* GetNodeVar(node_t* tokens, int* pos);
node_t* GetNodeFunction(node_t* tokens, int* pos);
node_t* GetNodeA(node_t* tokens, int* pos);
node_t* GetNodeIF(node_t* tokens, int* pos);
node_t* MakeNode();
node_t* NewOpNode(char* operation, node_t* left_node, node_t* right_node);
node_t* NewNumNode(double num, node_t* left_node, node_t* right_node);
node_t* NewVarNode(char* var, node_t* left_node, node_t* right_node);
