#include "headers/translate_into_asm.h"
#include <ctype.h>

void TranslateIntoAsm(node_t* node, FILE* file_asm, names_t* nametable)
{
    int num_of_labels = 0;
    int num_of_nametable = 0;//??
    TranslateIntoAsmBody(node, file_asm, nametable, &num_of_labels, &num_of_nametable);
    fprintf(file_asm, "hlt\n");
}

void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable)
{
    if (node->left)
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable);
    if (node->right)
        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable);
    if (node->type == NUM)
    {
        fprintf(file_asm, "push %lg\n", node->value.op_num, nametable);
    }

    else if (node->type == VAR)
    {
        // int i = nametable->last_i;//последняя свободная ячейка теперь это num_of_nametable
        (nametable + *num_of_nametable)->var = node->value.op_name;
        (nametable + *num_of_nametable)->num_of_name = *num_of_nametable;

        for (int j = 0; j < *num_of_nametable; j++)
        {
            int pos = 0;
            if (CompareWords(node->value.op_name, &pos, (nametable + j)->var))
            {
                (nametable + *num_of_nametable)->num_of_name = (nametable + j)->num_of_name;
                break;
            }
        }

        fprintf(file_asm, "push %d\n", (nametable + *num_of_nametable)->num_of_name);
        fprintf(file_asm, "pushm %d\n", (nametable + *num_of_nametable)->num_of_name);
        // nametable->last_i++;
        (*num_of_nametable)++;
    }

    else if (node->type == MINUS)
    {
        fprintf(file_asm, "sub\n");
    }

    else if (node->type == PLUS)
    {
        fprintf(file_asm, "add\n");
    }

    else if (node->type == DIVN)
    {
        fprintf(file_asm, "div\n");
    }

    else if (node->type == MULT)
    {
        fprintf(file_asm, "mul\n");
    }

    else if (node->type == OP_PRINTF)
    {
        fprintf(file_asm, "out\n");
    }

    // else if (node->type == OP_IF)
    // {
    //     fprintf(file_asm, "push 0\n");
    //     fprintf(file_asm, ":%d\n", *num_of_labels);
    //     TranslateIntoAsm(node->right, file_asm, nametable);
    //     fprintf(file_asm, ":%d\n", *num_of_labels);
    // }

    // else if (node->type == OP_WHILE)
    // {

    // }
    
}

node_t* FillTypesInTree(node_t* node)
{
    if (node->left)
        FillTypesInTree(node->left);
    if (node->right)
        FillTypesInTree(node->right);

    int pos = 0;

    if (node->value.op_name[0] <= '9' && node->value.op_name[0] >= '0')
    {
        int n = 0;
        int i = 0;

        while (node->value.op_name[i] <= '9' && node->value.op_name[i] >= '0')
        {
            n = n * 10 + (int)node->value.op_name[i] - '0';
            i++;
        }

        node->value.op_name = nullptr;
        node->value.op_num = n;
        node->type = NUM;
    }

    else if (node->value.op_name[0] == '*')
    {
        node->type = MULT;
    }

    else if (node->value.op_name[0] == '+')
    {
        node->type = PLUS;
    }

    else if (node->value.op_name[0] == '-')
    {
        node->type = MINUS;
    }

    else if (node->value.op_name[0] == '/')
    {
        node->type = DIVN;
    }

    else if (node->value.op_name[0] == '=')
    {
        node->type = OP_EQUAL;
    }

    else if (node->value.op_name[0] == ';')
    {
        node->type = UOP;
    }

    else if (CompareWords(node->value.op_name, &pos, (char*)PRINTF))
    {
        node->type = OP_PRINTF;
    }

    else 
    {
        int counter_func = 0;
        int amount_of_const_func = sizeof(arr_of_const_func) / sizeof(arr_of_const_func[0]);
        for (int i = 0; i < amount_of_const_func; i++)
        {
            // printf("@");
            int p = 0;
            if (CompareWords(node->value.op_name, &p, (char*)arr_of_const_func[i]))
            {
                // printf("~%c~", s[*pos]);
                node->type = OP_FUNC;
                node->value.op_name = (char*)arr_of_const_func[i];
                counter_func++;
                // nametable[amount_of_num].var = (char*)arr_of_const_func[i];
                // amount_of_num++;
                // n++;

                // continue;
            }
        }
        if (counter_func == 0)
        {
            //
        }
    }
    return node;
}

char* PutTreeFromFileToBuffer(int size)
{
    // assert(error != nullptr);
    char* buffer = (char*)calloc(size, sizeof(char));//

    // if (!buffer)
    //     *error = MEMORY_ALLOCATION;


    FILE* filee = fopen("files/tree.txt", "r");
    buffer = fgets(buffer, size, filee);

    if (!buffer)
        printf("null");
    //     *error = PROBLEMS_WITH_READING_FILE;

    fclose(filee);
    return buffer;
}

