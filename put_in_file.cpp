#include "headers/put_in_file.h"

void PutTreeInFile(node_t* node, FILE* filee)
{
    assert(node != nullptr);
    assert(filee != nullptr);

    fprintf(filee, "(");

    if (node->type == NUM)
        fprintf(filee, " \"%lg\" ", node->value.op_num);
    else
        fprintf(filee, " \"%s\" ", node->value.op_name);

    if (!node->left && !node->right)
        fprintf(filee, NILNIL);
        
    if (node->left)
        PutTreeInFile(node->left, filee);
        
    if (node->right)
        PutTreeInFile(node->right, filee);
        
    fprintf(filee, ")");
}

node_t* ReadNode(int* pos, char* buffer)
{
    assert(pos != nullptr);
    assert(buffer != nullptr);

    if (buffer[*pos] == ' ')
        (*pos)++;

    if (buffer[*pos] == '(')
    {
        (*pos)++;

        while(buffer[*pos] != '\"')
            (*pos)++;

        (*pos)++;
        node_t* new_node = NewVarNode(ReturnDataOfNode(pos, buffer), nullptr, nullptr);
        (*pos)++;

        new_node->left = ReadNode(pos, buffer);
        new_node->right = ReadNode(pos, buffer);

        while(buffer[*pos] != ')')
            (*pos)++;

        (*pos)++;

        return new_node;
    }

    if (buffer[*pos] == 'n' && buffer[(*pos) + 1] == 'i' && buffer[(*pos) + 2] == 'l')
        (*pos) += strlen("nil");

    return nullptr;
}

char* ReturnDataOfNode(int* pos, char* buffer)
{
    assert(pos != nullptr);
    assert(buffer != nullptr);

    char* node_data = (char*)calloc(MAX_LEN_OF_OPERATION, sizeof(char));

    if (!node_data)
        MemoryAllocationError();

    int i = 0;

    while(buffer[*pos] != '\"')
    {        
        node_data[i] = buffer[*pos];
        i++;
        (*pos)++;
    }

    (*pos)++;
    return node_data;
}