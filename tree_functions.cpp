#include "headers/tree_functions.h"
#include <cstring>

node_t* GetNodeG(char* s, int* pos)///name
{
    assert(s != nullptr);
    assert(pos != nullptr);

    node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
    node_t* prev_val = nullptr;

    do
    {
        SkipSpace(s, pos);

        int len_of_if = strlen(IF), counter = 0;
        for (int i = 0; i < len_of_if; i++)
        {
            if (s[(*pos) + i] == IF[i])
                counter++;
            else
                break;
        }

        if (counter == len_of_if)//
        {
            (*pos) += len_of_if;
            SkipSpace(s, pos);
            val->right = GetNodeIF(s, pos);
        }

        else
            val->right = GetNodeA(s, pos);

        (*pos)++;
        prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
        prev_val = val;
        val = NewOpNode((char*)COMB, prev_val, nullptr);

        SkipSpace(s, pos);

    } while (s[*pos] != '$');

    if (s[*pos] != '$')
        SyntaxError();
    (*pos)++;

    return prev_val;
}

// void GetNodeOP(char* s, int* pos, node_t* val, node_t* prev_val)
// {
//     // node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
//     // node_t* prev_val = nullptr;

//     SkipSpace(s, pos);

//     if (s[*pos] == 'i' && s[(*pos) + 1] == 'f')
//     {
//         (*pos) += 2;
//         SkipSpace(s, pos);
//         // if (s[*pos] == '{')
//         // {}
//         val->right = GetNodeIF(s, pos);
//     }

//     else
//         val->right = GetNodeA(s, pos);

//     (*pos)++;
//     prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
//     prev_val = val;
//     val = NewOpNode((char*)COMB, prev_val, nullptr);

//     SkipSpace(s, pos);
//     // return prev_val;
// }

node_t* GetNodeN(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);
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
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

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
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

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
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);
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
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    if (s[*pos] == '-')
    {
        (*pos)++;
        int val = GetNum(s, pos);
        return NewNumNode(val * (-1), nullptr, nullptr);
    }

    else
        return GetNodeN(s, pos);
}

node_t* GetNodeV(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

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
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    return NewOpNode(buf, nullptr, GetNodeP(s, pos));
}

node_t* GetNodeA(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);

    node_t* node = GetNodeV(s, pos);
    
    SkipSpace(s, pos);
    // printf("a: %c %d\n", s[*pos], *pos);
    if (s[*pos] == '=')
    {
        (*pos)++;
        return NewOpNode((char*)EQUAL, node, GetNodeE(s, pos));
    }

    return node;
}

node_t* GetNodeIF(char* s, int* pos)
{
    assert(s != nullptr);
    assert(pos != nullptr);

    SkipSpace(s, pos);
    node_t* new_node = nullptr;

    if (s[*pos] == '(')
    {
        new_node = GetNodeE(s, pos);
        SkipSpace(s, pos);
    }

    node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
    node_t* prev_val = nullptr;

    if (s[*pos] != '{')
    {
        node_t* new_new_node = GetNodeA(s, pos);
        return NewOpNode((char*)IF, new_node, new_new_node);
    }

    SkipSpace(s, pos);
        
    (*pos)++;
    do
    {
        SkipSpace(s, pos);

        val->right = GetNodeA(s, pos);

        if (s[*pos] != '}')
            (*pos)++;
            // printf(",%c,", s[*pos]);
        prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
        prev_val = val;
        val = NewOpNode((char*)COMB, prev_val, nullptr);

        SkipSpace(s, pos);

    } while (s[*pos] != '}');

    return NewOpNode((char*)IF, new_node, prev_val);///

}

// node_t* GetNodeIF(char* s, int* pos)
// {
//     assert(s != nullptr);
//     assert(pos != nullptr);

//     SkipSpace(s, pos);

//     // node_t* node = NewOpNode((char*)IF, nullptr, nullptr);

//     node_t* val1 = NewOpNode((char*)COMB, nullptr, nullptr);
//     node_t* prev_val1 = nullptr;
//     node_t* new_node = nullptr;

//     if (s[*pos] == '(')
//     {
//         new_node = GetNodeE(s, pos);
//             printf("%c", s[*pos]);

//         SkipSpace(s, pos);
//     }
//         // if (s[*pos] == '{')

//         // int len_of_and = strlen(AND), counter = 0;
//         // do{
//         // for (int i = 0; i < len_of_and; i++)
//         // {
//         //     if (s[(*pos) + i] == AND[i])
//         //         counter++;
//         //     else
//         //         break;
//         // }

//         // if (counter == len_of_and)//
//         // {
//         //     (*pos) += len_of_and;
//         //     SkipSpace(s, pos);
//         //     val1->right = GetNodeE(s, pos);

//         //     SkipSpace(s, pos);

//         //     // val->right = GetNodeA(s, pos);

//         //     if (s[*pos] != ')')
//         //         (*pos)++;
//         //     // printf(",%c,", s[*pos]);
//         //     prev_val1 = NewOpNode((char*)AND, nullptr, nullptr);
//         //     prev_val1 = val1;
//         //     val1 = NewOpNode((char*)AND, prev_val1, nullptr);

//         //     SkipSpace(s, pos);
//         // }

//         // else
//         // {
//         //     SkipSpace(s, pos);
//         //     printf("%c", s[*pos]);

//         //     val1->right = GetNodeE(s, pos);
//         //     // (*pos)++;
//             // printf("%c", s[*pos]);
//         //     SkipSpace(s, pos);
//         //     break;
//         // }

//             // printf("%c", s[*pos]);

//         // } while (s[*pos] != ')');
//         //     printf("%c", s[*pos]);

//         (*pos)++;
//         SkipSpace(s, pos);

//         node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
//         node_t* prev_val = nullptr;

//         if (s[*pos] != '{')
//             {
//                 // (*pos)++;
//                 node_t* new_new_node = GetNodeA(s, pos);
//                 return NewOpNode((char*)IF, new_node, new_new_node);
//             }

//         SkipSpace(s, pos);
        
//         (*pos)++;
//         do
//         {
//             SkipSpace(s, pos);

//             val->right = GetNodeA(s, pos);

//             if (s[*pos] != '}')
//                 (*pos)++;
//             // printf(",%c,", s[*pos]);
//             prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
//             prev_val = val;
//             val = NewOpNode((char*)COMB, prev_val, nullptr);

//             SkipSpace(s, pos);

//         } while (s[*pos] != '}');

//         // // node_t* new_new_node = GetNodeA(s, pos);

//         // return NewOpNode((char*)"{}", new_node, NewOpNode((char*)IF, new_node, );
//         return NewOpNode((char*)IF, new_node, prev_val);///
    
//     return nullptr;
// }

// node_t* GetNodeIF(char* s, int* pos)
// {
//     assert(s != nullptr);
//     assert(pos != nullptr);
    
//     SkipSpace(s, pos);

//     if (s[*pos] == '(')
//     {
//         int val = GetE(s, pos);
//         (*pos)++;

//         SkipSpace(s, pos);

//         if (val)
//             return GetNodeA(s, pos);
//     }
//     return nullptr;
// }

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

    if (!strncmp((char*)";", operation, MAX_LEN_OF_OPERATION))
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
