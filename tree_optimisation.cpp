#include "headers/tree_optimisation.h"

node_t* ConstantOptimization(node_t* node)
{
    assert(node != nullptr);

    // printf("\nstart tree optimisation\n");
    int amount_of_nodes = 0, new_amount_of_nodes = 0;
    FindAmountOfNodes(&amount_of_nodes, node);
    // printf("%d\n", amount_of_nodes);
    while (amount_of_nodes != new_amount_of_nodes)
    {
        // printf("q");
        amount_of_nodes = new_amount_of_nodes;
        new_amount_of_nodes = 0;
        BodyOfConstantOptimisation(node);
        FindAmountOfNodes(&new_amount_of_nodes, node);
    }

    return node;
}

node_t* BodyOfConstantOptimisation(node_t* node)
{
    assert(node != nullptr);

    int amount_of_operations = sizeof(arr_of_op_for_optimisation) / sizeof(arr_of_op_for_optimisation[0]);

    if (node->left && node->right && LEFT_TYPE == NUM && RIGHT_TYPE == NUM)
    {
        // printf("\nfirst if\n");
        for (int i = 0; i < amount_of_operations; i++)
        {
            if (arr_of_op_for_optimisation[i].op_name == node->type)
            {
                (node->value).op_num = (arr_of_op_for_optimisation[i].ptr)(LEFT_NUM, RIGHT_NUM);
                // printf("%d\n", (node->value).op_num);
                break;
            }
        }

        node->type = NUM; 
        // free(node->left);//что ж делать чтож делать..
        // free(node->right);
        node->left = nullptr;
        node->right = nullptr;
    }

    else if (node->left && node->right && 
            (((LEFT_TYPE == NUM || LEFT_TYPE == VAR) && ((node->left)->value).op_num == 0) || 
            ((RIGHT_TYPE == NUM || RIGHT_TYPE == VAR) && ((node->right)->value).op_num == 0)))
    {
        for (int i = 0; i < amount_of_operations; i++)
        {
            if (!strncmp(MUL, (node->value).op_name, MAX_LEN_OF_OPERATION))
            {
                node->type = NUM;
                (node->value).op_num = 0;
                node->left = nullptr;
                node->right = nullptr;
                break;
            }
        } 
    }

    if (node->left)
        BodyOfConstantOptimisation(node->left);

    if (node->right)
        BodyOfConstantOptimisation(node->right);
    
    return node;
}

void FindAmountOfNodes(int* amount, node_t* node)
{
    assert(amount != nullptr);
    assert(node != nullptr);
    
    (*amount)++;

    if (node->left)
        FindAmountOfNodes(amount, node->left);

    if (node->right)
        FindAmountOfNodes(amount, node->right);
}

int AddNum(int num1, int num2)
{
    return num1 + num2;
}

int MulNum(int num1, int num2)
{
    return num1 * num2;
}

int SubNum(int num1, int num2)
{
    return num1 - num2;
}

int DivNum(int num1, int num2)
{
    return num1 / num2;
}

int DegNum(int num1, int num2)
{
    return pow(num1, num2);
}