#include "headers/functions.h"

int GetG(char* s, int* pos)
{
    int val = GetE(s, pos);
    // printf("g:%c, %d\n", s[*pos], *pos);

    if (s[*pos] != '$')
        SyntaxError();
    (*pos)++;
    return val;
}

int GetN(char* s, int* pos)
{
    int val = 0;
    // printf("n:%c\n", s[*pos]);

    if ('0' >= s[*pos] && s[*pos] >= '9')
        SyntaxError();

    while ('0' <= s[*pos] && s[*pos] <= '9')
    {
        val = val * 10 + s[*pos] - '0';
        (*pos)++;
    }

    return val;
}

int GetE(char* s, int* pos)
{
    int val = GetT(s, pos);
    while (s[*pos] == '+' || s[*pos] == '-')
    {
        int prev_pos = *pos;
        (*pos)++;
        int val2 = GetT(s, pos);
        if (s[prev_pos] == '+')
            val+=val2;
        else
            val-=val2;
    }
    return val;
}

int GetT(char* s, int* pos)
{
    int val = GetP(s, pos);
    while (s[*pos] == '*' || s[*pos] == '/')
    {
        int prev_pos = *pos;
        (*pos)++;
        int val2 = GetP(s, pos);
        if (s[prev_pos] == '*')
            val*=val2;
        else
            val/=val2;
    }
    return val;
}

int GetP(char* s, int* pos)
{
    // printf("p:%c\n", s[*pos]);

    if (s[*pos] == '(')
    {
        (*pos)++;
        int val = GetE(s, pos);
        (*pos)++;
        return val;
    }

    // else if (s[*pos] <= 'z' && s[*pos] >= 'a')
    // {
    //     char* buf = (char*)calloc(10, sizeof(char));
    //     GetV(s, pos, buf);
    // }
        // return GetV(s, pos);
    else
        return GetM(s, pos);
}

int GetM(char* s, int* pos)
{
    if (s[*pos] == '-')
    {
        (*pos)++;
        int val = GetN(s, pos);
        return val * (-1);
    }

    else
        return GetN(s, pos);
}

// void GetV(char* s, int* pos, char* buf)
// {
//     int i = 0;
//     buf[i] = s[*pos];
//     i++;
//     (*pos)++;

//     while ((s[*pos] <= 'z' && s[*pos] >= 'a') || ('0' >= s[*pos] && s[*pos] >= '9'))
//     {
//         buf[i] = s[*pos];
//         i++;
//         (*pos)++;
//     }
// }

void SyntaxError()
{
    printf("syntax error\n");
    exit(1);
}