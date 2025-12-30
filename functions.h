#ifndef FOR_TYPES
#include "types.h"
#endif
#define FOR_FUNC
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lex_analysis.h"

int GetCode(char* s, int* pos);
int GetNum(char* s, int* pos);
int GetExpressionWithAddOrSub(char* s, int* pos);
int GetExpressionWithMulOrDiv(char* s, int* pos);
int GetExpressionInBrackets(char* s, int* pos);
int GetNumBelowZero(char* s, int* pos);

void SyntaxError();
void SkipSpace(char* s, int* pos);
void MemoryAllocationError();
void FillNametable(names_t* nametable, node_t* node, int* last_index_in_nametable);
