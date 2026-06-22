#ifndef FOR_TYPES
#include "types.h"
#endif

#include <assert.h>

#ifndef LEX
#include "lex_analysis.h"
#endif

#ifndef FOR_FUNC
#include "functions.h"
#endif

#ifndef TREE_F
static const char* COMB = ";";
#define TREE_F
#endif

// #ifndef MAX_LEN_OP
// static const int MAX_LEN_OF_OPERATION = 10;
// #define MAX_LEN_OP
// #endif

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
node_t* FindMainFunc(node_t* node, node_t* main_node);
