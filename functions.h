#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int GetAll(char* s, int* pos);
int GetNum(char* s, int* pos);
int GetExpressionWithAddOrSub(char* s, int* pos);
int GetExpressionWithMulOrDiv(char* s, int* pos);
int GetExpressionInBrackets(char* s, int* pos);
int GetNumBelowZero(char* s, int* pos);

void SyntaxError();
void SkipSpace(char* s, int* pos);
