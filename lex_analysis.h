#ifndef FOR_TYPES
#include "types.h"
#endif
#ifndef FOR_FUNC
#include "functions.h"
#endif
// #ifndef FOR_CONST
// #include "const.h"
// #endif
#include <cstring>
#ifndef LEX
static const char* PRINTF = "hadebadega";
static const char* WHILE = "hawawaw";
static const char* IF = "ha";
static const char* EQUAL = "=";
static const char* SIN = "haswaiwana";
static const char* COS = "hacwaowasa";
static const char* TG = "hatwaga";
static const char* CTG = "hacwatwaga";
static const char* LN = "ln";
static const char* arr_of_const_func[] = {SIN, COS, TG, CTG, LN, PRINTF};
#define LEX
#endif

#ifndef MAX_LEN_OP
static const int MAX_LEN_OF_OPERATION = 10;
#define MAX_LEN_OP
#endif

size_t LexAnalysis(char* s, int* pos, node_t* tokens);
bool CompareWords(char* s, int* pos, char* word);