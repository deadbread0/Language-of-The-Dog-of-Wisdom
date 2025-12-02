#include "headers/tree_functions.h"
#include "headers/functions.h"
#include "headers/const.h"
#include <cstring>

node_t* GetNodeG(char* s, int* pos)
{
    node_t* val = GetNodeE(s, pos);
    // printf("g:%c, %d\n", s[*pos], *pos);

    if (s[*pos] != '$')
        SyntaxError();
    (*pos)++;

    return val;
}

node_t* GetNodeN(char* s, int* pos)
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

    return NewNumNode(val, NULL, NULL);
}

node_t* GetNodeE(char* s, int* pos)
{
    node_t* val = GetNodeT(s, pos);
    while (s[*pos] == '+' || s[*pos] == '-')
    {
        int prev_pos = *pos;
        (*pos)++;
        node_t* val2 = GetNodeT(s, pos);
        if (s[prev_pos] == '+')
            val = NewOpNode((char*)ADD, val, val2);
        else
            val = NewOpNode((char*)SUB, val, val2);
    }
    return val;
}

node_t* GetNodeT(char* s, int* pos)
{
    node_t* val = GetNodeP(s, pos);
    while (s[*pos] == '*' || s[*pos] == '/')
    {
        int prev_pos = *pos;
        (*pos)++;
        node_t* val2 = GetNodeP(s, pos);
        if (s[prev_pos] == '*')
            val = NewOpNode((char*)MUL, val, val2);
        else
            val = NewOpNode((char*)DIV, val, val2);
    }
    return val;
}

node_t* GetNodeP(char* s, int* pos)
{
    // printf("p:%c\n", s[*pos]);

    if (s[*pos] == '(')
    {
        (*pos)++;
        node_t* val = GetNodeE(s, pos);
        (*pos)++;
        return val;
    }

    else if (s[*pos] <= 'z' && s[*pos] >= 'a')
        return GetNodeV(s, pos);

    else
        return GetNodeM(s, pos);
}

node_t* GetNodeM(char* s, int* pos)
{
    if (s[*pos] == '-')
    {
        (*pos)++;
        int val = GetN(s, pos);
        return NewNumNode(val * (-1), nullptr, nullptr);
    }

    else
        return GetNodeN(s, pos);
}

node_t* GetNodeV(char* s, int* pos)
{
    char* buf = (char*)calloc(10, sizeof(char));
    int i = 0;
    buf[i] = s[*pos];
    i++;
    (*pos)++;

    while ((s[*pos] <= 'z' && s[*pos] >= 'a') || ('0' >= s[*pos] && s[*pos] >= '9'))
    {
        buf[i] = s[*pos];
        i++;
        (*pos)++;
    }

    int amount_of_func = sizeof(arr_of_const_func) / sizeof(arr_of_const_func[0]);
    for (int j = 0; j < amount_of_func; j++)
    {
        if (!strncmp(buf, arr_of_const_func[j], 10))
        {
            return GetNodeF(s, pos, buf);
        }
    }
    
    return NewVarNode(buf, nullptr, nullptr);
}

node_t* GetNodeF(char* s, int* pos, char* buf)
{
    return NewOpNode(buf, nullptr, GetNodeP(s, pos));
}

node_t* MakeNode()
{
    node_t* new_node = (node_t*)calloc(1, sizeof(node_t));

    if (!new_node)
    {
        printf("memory allocation error, try again\n");
        return nullptr;
    }

    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->prev = nullptr;

    return new_node;
}

node_t* NewOpNode(char* operation, node_t* left_node, node_t* right_node)
{
    assert(operation != nullptr);

    node_t* new_node = MakeNode();
    new_node->type = OP;
    (new_node->value).op_name = operation;
    new_node->left = left_node;
    new_node->right = right_node;
    return new_node;
}

node_t* NewNumNode(double num, node_t* left_node, node_t* right_node)
{
    // assert(operation != nullptr);

    node_t* new_node = MakeNode();
    new_node->type = NUM;
    (new_node->value).op_num = num;
    new_node->left = left_node;
    new_node->right = right_node;
    return new_node;
}

node_t* NewVarNode(char* var, node_t* left_node, node_t* right_node)
{
    // assert(operation != nullptr);

    node_t* new_node = MakeNode();
    new_node->type = VAR;
    (new_node->value).op_name = var;
    new_node->left = left_node;
    new_node->right = right_node;
    return new_node;
}