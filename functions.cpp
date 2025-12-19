#include "headers/functions.h"

int GetAll(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    int val = GetExpressionWithAddOrSub(s, pos);

    if (s[*pos] != '$')
        SyntaxError();
    (*pos)++;
    return val;
}

int GetNum(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    int val = 0;

    if ('0' >= s[*pos] && s[*pos] >= '9')
        SyntaxError();

    while ('0' <= s[*pos] && s[*pos] <= '9')
    {
        val = val * 10 + s[*pos] - '0';
        (*pos)++;
    }

    return val;
}

int GetExpressionWithAddOrSub(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    int val = GetExpressionWithMulOrDiv(s, pos);
    while (s[*pos] == '+' || s[*pos] == '-')
    {
        int prev_pos = *pos;
        (*pos)++;
        int val2 = GetExpressionWithMulOrDiv(s, pos);
        if (s[prev_pos] == '+')
            val+=val2;
        else
            val-=val2;
    }
    return val;
}

int GetExpressionWithMulOrDiv(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    int val = GetExpressionInBrackets(s, pos);
    while (s[*pos] == '*' || s[*pos] == '/')
    {
        int prev_pos = *pos;
        (*pos)++;
        int val2 = GetExpressionInBrackets(s, pos);
        if (s[prev_pos] == '*')
            val*=val2;
        else
            val/=val2;
    }
    return val;
}

int GetExpressionInBrackets(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    if (s[*pos] == '(')
    {
        (*pos)++;
        int val = GetExpressionWithAddOrSub(s, pos);
        (*pos)++;
        return val;
    }
    else
        return GetNumBelowZero(s, pos);
}

int GetNumBelowZero(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    if (s[*pos] == '-')
    {
        (*pos)++;
        int val = GetNum(s, pos);
        return val * (-1);
    }

    else
        return GetNum(s, pos);
}

void SyntaxError()
{
    printf("syntax error\n");
    exit(1);
}

void MemoryAllocationError()
{
    printf("Memory allocation error, try again pls\n");
}

void SkipSpace(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    while (s[*pos] == ' ' || s[*pos] == '\n')
        (*pos)++;
}