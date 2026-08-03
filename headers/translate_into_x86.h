#ifndef FOR_TYPES
#include "types.h"
#endif

#include "input.h"
#include <assert.h>
#include "lex_analysis.h"

const int MAX_LEN_OF_FUNC_NAME = 20;

char* PutTreeFromFileToBuffer(int size);
node_t* FillTypesInTree(node_t* node);
void TranslateIntoAsm(node_t* node, FILE* file_asm);
void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable, int* counter_of_commands, node_t* first_node);
node_t* FillUsersFunc(node_t* node);
node_t* ReturnFuncDeclaration(node_t* first_node, node_t* node);
node_t* ReturnFuncCall(node_t* first_node, node_t* node, node_t* call_node);
int CountLabels(node_t* first_node, node_t* node);
void CountLabelsToTheEnd(node_t* node, int* counter);
int CountAmountOfEndFunc(node_t* first_node, node_t* node);
