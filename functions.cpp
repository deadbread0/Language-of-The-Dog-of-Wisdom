#include "headers/functions.h"

int GetCode(char* s, int* pos)
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

names_t* ReturnEmptyNametable()
{
    names_t* nametable = (names_t*)calloc(NT_INITIAL_SIZE, sizeof(names_t));
    if (!nametable)
        printf("could not allocate memory for the table, delete brawl stars to continue\n");
    for (int i = 0; i < NT_INITIAL_SIZE; i++)
        (nametable + i)->num_in_stack = -1;
    
    return nametable;
}

int ReturnLastStackNum(names_t* nametable)
{
    int s = 0;
    for (int i = 0; i < NT_INITIAL_SIZE; i++)
    {
        if ((nametable + i)->num_in_stack > s)
            s = nametable->num_in_stack;
    }

    return s;
}

// void FillNametable(names_t* nametable, node_t* node, int* last_index_in_nametable)
// {
//     if (node->type == OP_FUNC)
//     {
//         int counterr = 0, num_of_name = 0;
//         for (int i = 0; i < *last_index_in_nametable; i++)
//         {
//             int p = 0;
//             if (CompareWords(node->value.op_name, &p, (nametable + i)->var))
//             {
//                 counterr++;
//                 num_of_name = (nametable + i)->num_of_name;
//             }
//         }

//         if (!counterr)//если функция встречается в первый раз
//             (nametable + *last_index_in_nametable)->num_of_name = *last_index_in_nametable;

//         else
//             (nametable + *last_index_in_nametable)->num_of_name = num_of_name;
        
//         (nametable + *last_index_in_nametable)->var = node->value.op_name;
//         (nametable + *last_index_in_nametable)->value = counterr + 1;//типа который раз эта функция встречается в коде
//         (*last_index_in_nametable)++;
//     }

//     if (node->type == VAR)
//     {
//         int counter = 0;

//         for (int i = 0; i < *last_index_in_nametable; i++)
//         {
//             int p = 0;

//             if (CompareWords((nametable + i)->var, &p, node->value.op_name))
//             {
//                 counter++;
//                 break;
//             }
//         }

//         if (!counter)
//         {
//             (nametable + *last_index_in_nametable)->num_of_name = *last_index_in_nametable;
//             (nametable + *last_index_in_nametable)->var = node->value.op_name;
//             (nametable + *last_index_in_nametable)->num_in_stack = -1; //-1 значит, что переменной еще нет в стеке
//             (*last_index_in_nametable)++;
//             // SetElemNametable(nametable, node->value.op_name, last_index_in_nametable);
//         }
//     }

//     if (node->left)
//         FillNametable(nametable, node->left, last_index_in_nametable);
//     if (node->right)
//         FillNametable(nametable, node->right, last_index_in_nametable);
    
// }

// int GetElemNametable(names_t* nametable, char* node_name, int* last_index_in_nametable)
// {
//     for (int i = 0; i < *last_index_in_nametable; i++)
//         {
//             int p = 0;

//             if (CompareWords((nametable + i)->var, &p, node_name))
//             {
//                 return 1;
//                 break;
//             }
//         }

//     return 0;
// }

// void SetElemNametable(names_t* nametable, char* node_name, int* last_index_in_nametable, int counterr)
// {
//             (nametable + *last_index_in_nametable)->num_of_name = *last_index_in_nametable;
//             (nametable + *last_index_in_nametable)->var = node_name;
//             (*last_index_in_nametable)++;
// }