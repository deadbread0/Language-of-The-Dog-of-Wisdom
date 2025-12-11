#ifndef FOR_TYPES
#include "types.h"
#endif
#include "functions.h"
#ifndef FOR_CONST
#include "const.h"
#endif
#include <cstring>
#define LEX
size_t LexAnalysis(char* s, int* pos, node_t* tokens);
bool CompareWords(char* s, int* pos, char* word);