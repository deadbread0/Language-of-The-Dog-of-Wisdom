#include "headers/tree_functions.h"
#include <cstring>

node_t* GetNodeComb(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    node_t* root = NewOpNode((char*)COMB, nullptr, nullptr);
    node_t* val = root;
    node_t* maiin = nullptr; 
    int fopened = 0, fclosed = 0;

    do
    {
        if (tokens[*pos].type == OP_IF || tokens[*pos].type == OP_WHILE)
            val->left = GetNodeIF(tokens, pos);

        else if (tokens[*pos].type == OP_FUNC)
        {
            if (maiin && fclosed != fopened)
            {
                node_t* prevval = val;
                maiin->right = NewOpNode((char*)COMB, nullptr, nullptr);
                val = maiin->right;
                val->left = GetNodeFunction(tokens, pos);
                val->right = NewOpNode((char*)COMB, nullptr, nullptr);
                val = val->right;
                maiin = val;
                val = prevval;
            }
            else
                val->left = GetNodeIF(tokens, pos);
        }

        else if (tokens[*pos].type == MAIN_FUNC)
        {
            val->left = GetNodeIF(tokens, pos);
            node_t* prevval = val;
            if (maiin)
            {
                val = val->left;
                val->prev = prevval;
            }

            maiin = prevval->left;
        }

        else
        {
            if (tokens[*pos].type != FBRACKET_CLOSE)
                val->left = GetNodeA(tokens, pos);
        }

        if (tokens[*pos].type == FBRACKET_CLOSE)
            fclosed++;
        if (tokens[*pos].type == FBRACKET_OPEN)
            fopened++;
        if (tokens[*pos].type != END)
            (*pos)++;

        val->right = NewOpNode((char*)COMB, nullptr, nullptr);
        val = val->right;

        if (tokens[*pos].type == UOP)
            (*pos)++;

    } while (tokens[*pos].type != END);

    if (tokens[*pos].type != END)
        SyntaxError();

    (*pos)++;

    return root;
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
        return GetNodeM(tokens, pos);
}

node_t* GetNodeM(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    if (tokens[*pos].type == MINUS)
    {
        (*pos)++;
        node_t* node = GetNodeNum(tokens, pos);
        node->value.op_num = node->value.op_num * (-1);
        return node;
    }

    else
        return GetNodeNum(tokens, pos);
}

node_t* GetNodeVar(node_t* tokens, int* pos)
{
    assert(tokens != nullptr);
    assert(pos != nullptr);

    if (tokens[(*pos)].type == OP_FUNC || tokens[*pos].type == OP_PRINTF || tokens[*pos].type == OP_RETURN)
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
        new_node = GetNodeAddOrSub(tokens, pos);

    if (tokens[*pos].type == UOP)
        return nullptr;

    if (tokens[*pos].type != FBRACKET_OPEN)
    {
        node_t* new_new_node = nullptr;
        new_new_node = GetNodeA(tokens, pos);
        (tokens + if_pos)->left = new_node;
        (tokens + if_pos)->right = new_new_node;
        return tokens + if_pos;
    }
    if ((tokens + if_pos)->type == MAIN_FUNC)
    {
        (tokens + if_pos)->left = new_node;
        return (tokens + if_pos);
    }

    (*pos)++;
    int counter_of_fbracket = 1, counter_of_fbracket_closed = 0;

    (tokens + if_pos)->left = new_node;

    node_t* body_root = NewOpNode((char*)COMB, nullptr, nullptr);
    node_t* current_comb = body_root;
    (tokens + if_pos)->prev = body_root;

    while (counter_of_fbracket != counter_of_fbracket_closed)
    {
        if (tokens[*pos].type == FBRACKET_CLOSE)
            (*pos)++;
    do
    {
        if (tokens[*pos].type == OP_IF || tokens[*pos].type == OP_WHILE || tokens[*pos].type == OP_FUNC)
        {
            if (tokens[(*pos + 1)].type == FBRACKET_OPEN)
                counter_of_fbracket++;
            current_comb->left = GetNodeIF(tokens, pos);
        }

        else
            current_comb->left = GetNodeA(tokens, pos);
        
        if (tokens[*pos].type == UOP)
            (*pos)++;

        if (tokens[*pos].type == FBRACKET_CLOSE)
            counter_of_fbracket_closed++;

        current_comb->right = NewOpNode((char*)COMB, nullptr, nullptr);
        current_comb = current_comb->right;

    } while (tokens[*pos].type != FBRACKET_CLOSE);
    }

    (tokens + if_pos)->right = body_root;

    return (tokens + if_pos);

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

node_t* FindMainFunc(node_t* node, node_t* main_node)
{
    assert(node != nullptr);

    if (node->type == MAIN_FUNC)
        main_node = node;

    if (node->left)
        main_node = FindMainFunc(node->left, main_node);
    if (node->right)
        main_node = FindMainFunc(node->right, main_node);
    return main_node;
}

