#include "headers/translate_into_asm.h"
#include <ctype.h>


void TranslateIntoAsm(node_t* node, FILE* file_asm, names_t* nametable)
{
    if (node->left)
        TranslateIntoAsm(node->left, file_asm, nametable);
    if (node->right)
        TranslateIntoAsm(node->right, file_asm, nametable);
    if (node->type == NUM)
    {
        fprintf(file_asm, "push %lg\n", node->value.op_num, nametable);
    }

    else if (node->type == VAR)
    {
        int i = nametable->last_i;//последняя свободная ячейка
        (nametable + i)->var = node->value.op_name;
        (nametable + i)->num = i;

        for (int j = 0; j < i; j++)
        {
            int pos = 0;
            if (CompareWords(node->value.op_name, &pos, (nametable + j)->var))
            {
                (nametable + i)->num = (nametable + j)->num;
                break;
            }
        }

        fprintf(file_asm, "push %d\n", (nametable + i)->num);
        fprintf(file_asm, "pushm %d\n", (nametable + i)->num);
        nametable->last_i++;
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
    
}

node_t* FillTypesInTree(node_t* node)
{
    if (node->left)
        FillTypesInTree(node->left);
    if (node->right)
        FillTypesInTree(node->right);

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

