#include "headers/tree_functions.h"
#include <cstring>

node_t* GetNodeComb(node_t* tokens, int* pos)///name
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
    val->type = UOP;
    node_t* prev_val = nullptr;

    do
    {
        if (tokens[*pos].type == OP_IF || tokens[*pos].type == OP_WHILE)
        {
            val->right = GetNodeIF(tokens, pos);
        }

        else
            val->right = GetNodeA(tokens, pos);

        (*pos)++;
        prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
        prev_val = val;
        val = NewOpNode((char*)COMB, prev_val, nullptr);

        if (tokens[*pos].type == UOP)
            (*pos)++;

    } while (tokens[*pos].type != END);

    if (tokens[*pos].type != END)
        SyntaxError();

    (*pos)++;

    return prev_val;
}


node_t* GetNodeNum(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    int val = 0;

    if (tokens[*pos].type != NUM)
        SyntaxError();

    (*pos)++;
    return tokens + (*pos) - 1;
}

node_t* GetNodeAddOrSub(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    node_t* val = GetNodeMulOrDiv(tokens, pos);

    while (tokens[*pos].type == PLUS || tokens[*pos].type == MINUS)
    {
        int prev_pos = *pos;
        (*pos)++;
        node_t* val2 = GetNodeMulOrDiv(tokens, pos);
        (tokens + prev_pos)->left = val;
        (tokens + prev_pos)->right = val2;
        val = tokens + prev_pos;
    }
    return val;
}

node_t* GetNodeMulOrDiv(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    node_t* val = GetNodeBracket(tokens, pos);
    while (tokens[*pos].type == MULT || tokens[*pos].type == DIVN)
    {
        int prev_pos = *pos;
        (*pos)++;
        node_t* val2 = GetNodeBracket(tokens, pos);
        (tokens + prev_pos)->left = val;
        (tokens + prev_pos)->right = val2;
        val = tokens + prev_pos;
    }
    return val;
}

node_t* GetNodeBracket(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    if (tokens[*pos].type == BRACKET_OPEN)
    {
        (*pos)++;
        node_t* val = GetNodeAddOrSub(tokens, pos);
        (*pos)++;
        return val;
    }

    else if (tokens[*pos].type == VAR || tokens[*pos].type == OP_FUNC)
        return GetNodeVar(tokens, pos);

    else
        return GetNodeNum(tokens, pos);
}

// node_t* GetNodeM(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     SkipSpace(s, pos);

//     if (s[*pos] == '-')
//     {
//         (*pos)++;
//         int val = GetNum(s, pos);
//         return NewNumNode(val * (-1), nullptr, nullptr);
//     }

//     else
//         return GetNodeNum(s, pos);
// }

node_t* GetNodeVar(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    if (tokens[(*pos)].type == OP_FUNC)
        return GetNodeFunction(tokens, pos);
    
    (*pos)++;

    return tokens + (*pos) - 1;
}

node_t* GetNodeFunction(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    int fpos = (*pos);

    (*pos)++;
    (tokens + fpos)->right = GetNodeBracket(tokens, pos);
    return tokens + fpos;
}

node_t* GetNodeA(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    node_t* node = GetNodeVar(tokens, pos);
    
    if (tokens[*pos].type == OP_EQUAL)
    {
        int prev_pos = *pos;
        (*pos)++;
        (tokens + prev_pos)->left = node;
        (tokens + prev_pos)->right = GetNodeAddOrSub(tokens, pos);
        return tokens + prev_pos;
    }

    return node;
}

node_t* GetNodeIF(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);
    int if_pos = *pos;
    (*pos)++;

    node_t* new_node = nullptr;

    if (tokens[*pos].type == BRACKET_OPEN)
    {
        new_node = GetNodeAddOrSub(tokens, pos);
    }

    node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
    node_t* prev_val = nullptr;

    if (tokens[*pos].type != FBRACKET_OPEN)
    {
        node_t* new_new_node = GetNodeA(tokens, pos);
        (tokens + if_pos)->left = new_node;
        (tokens + if_pos)->right = new_new_node;
        return tokens + if_pos;
    }

    (*pos)++;

    do
    {
        val->right = GetNodeA(tokens, pos);

        if (tokens[*pos].type != FBRACKET_CLOSE)
            (*pos)++;

        prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
        prev_val = val;
        val = NewOpNode((char*)COMB, prev_val, nullptr);

    } while (tokens[*pos].type != FBRACKET_CLOSE);

    (tokens + if_pos)->left = new_node;
    (tokens + if_pos)->right = prev_val;
    return tokens + if_pos;

}


node_t* MakeNode()
{
    node_t* new_node = (node_t*)calloc(1, sizeof(node_t));

    if (!new_node)
    {
        MemoryAllocationError();
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

    if (!strncmp((char*)COMB, operation, MAX_LEN_OF_OPERATION))
        new_node->type = UOP;

    (new_node->value).op_name = operation;
    new_node->left = left_node;
    new_node->right = right_node;
    return new_node;
}

node_t* NewNumNode(double num, node_t* left_node, node_t* right_node)
{
    node_t* new_node = MakeNode();
    new_node->type = NUM;
    (new_node->value).op_num = num;
    new_node->left = left_node;
    new_node->right = right_node;
    return new_node;
}

node_t* NewVarNode(char* var, node_t* left_node, node_t* right_node)
{
    assert(var != nullptr);

    node_t* new_node = MakeNode();
    new_node->type = VAR;
    (new_node->value).op_name = var;
    new_node->left = left_node;
    new_node->right = right_node;
    return new_node;
}