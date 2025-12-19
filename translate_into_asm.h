#ifndef FOR_TYPES
#include "types.h"
#endif
#ifndef FOR_CONST
#include "const.h"
#endif
#include "input.h"
#include <assert.h>
#include "lex_analysis.h"
char* PutTreeFromFileToBuffer(int size);
node_t* FillTypesInTree(node_t* node);
void TranslateIntoAsm(node_t* node, FILE* file_asm, names_t* nametable);
void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable);
