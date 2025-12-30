#include "headers/translate_into_asm.h"
#include <ctype.h>

void TranslateIntoAsm(node_t* node, FILE* file_asm, names_t* nametable)
{
    int num_of_labels = 0;
    int num_of_nametable = 2;//??
    int counter_of_commands = 0;
    TranslateIntoAsmBody(node, file_asm, nametable, &num_of_labels, &num_of_nametable, &counter_of_commands);
    fprintf(file_asm, "hlt\n");
}

void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable, int* counter_of_commands)
{
    if (node->type == OP_IF)
    {
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);

        int new_counter = 0;
        CountAmountOfSteps(node, &new_counter);

        fprintf(file_asm, "push 0\nje %d\n", new_counter + *counter_of_commands + 2);///???
        (*counter_of_commands) += 4;

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);

        fprintf(file_asm, ":%d\n", *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 2;

        return;
    }

    if (node->type == OP_WHILE)
    {
        int new_counter = 0;
        CountAmountOfSteps(node, &new_counter);

        int first_jump = new_counter + *counter_of_commands + 1;
        (*counter_of_commands) += 4;

        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);

        fprintf(file_asm, "push 0\nje %d\n", new_counter + *counter_of_commands - 1);///???
        int second_jump = *counter_of_commands;
        fprintf(file_asm, ":%d\n", *num_of_labels);
        (*num_of_labels)++;
        
        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);

        fprintf(file_asm, "push 0\njne %d\n", second_jump);
        (*counter_of_commands) += 4;

        fprintf(file_asm, ":%d\n", *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 4;

        return;
    }
    
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        
        int index = 0;
        for (int j = 0; j < *num_of_nametable; j++)
        {
            if (strncmp((nametable + j)->var, node->left->value.op_name, MAX_LEN_OF_OPERATION) == 0)////
            {
                index = j;
                break;
            }
        }

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);

        fprintf(file_asm, "popm %d\n", (nametable + index)->num_of_name);
        (*counter_of_commands) += 2;
        return;
    }

    if (node->left)
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);
    if (node->right)
        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands);
    if (node->type == NUM)
    {
        // printf("%x\n", node);
        fprintf(file_asm, "push %lg\n", node->value.op_num);
        (*counter_of_commands) += 2;

        return;
    }

    

    if (node->type == VAR)
    {
        int index = 0;
        for (int j = 0; j < *num_of_nametable; j++)
        {
            int pos = 0;
            // printf("\n%s - %s\n", (nametable + j)->var, node->value.op_name);
            if (strncmp((nametable + j)->var, node->value.op_name, MAX_LEN_OF_OPERATION) == 0)////
            {
                index = j;
                break;
            }
        }

        // printf("%d %d\n", node->left, node->right);
        fprintf(file_asm, "pushm %d\n", index);
        (*counter_of_commands) += 2;
        return;
        // printf("%d\n", index);

    }

    if (node->type == MINUS)
    {
        fprintf(file_asm, "sub\n");
        (*counter_of_commands)++;
        return;

    }

    if (node->type == PLUS)
    {
        fprintf(file_asm, "add\n");
        (*counter_of_commands)++;
        return;

    }

    else if (node->type == DIVN)
    {
        fprintf(file_asm, "div\n");
        (*counter_of_commands)++;

    }

    else if (node->type == MULT)
    {
        fprintf(file_asm, "mul\n");
        (*counter_of_commands)++;

    }

    else if (node->type == OP_PRINTF)
    {
        fprintf(file_asm, "out\n");
        (*counter_of_commands)++;

    }
    return;
}

node_t* FillTypesInTree(node_t* node)
{
    if (node->left)
        FillTypesInTree(node->left);
    if (node->right)
        FillTypesInTree(node->right);

    int pos = 0;

    if (node->value.op_name[0] == '-')
    {
        // printf(" %s ", node->value.op_name);
        if (strlen(node->value.op_name) > 1 && node->value.op_name[1] <= '9' && node->value.op_name[1] >= '0')
        {
            // printf(" %s ", node->value.op_name);
            int i = 1;
            int n = 0;

            while (node->value.op_name[i] <= '9' && node->value.op_name[i] >= '0')
            {
                n = n * 10 + (int)node->value.op_name[i] - '0';
                i++;
            }

            node->type = NUM;
            node->value.op_num = n * (-1);
            
            return node;
        }
        node->type = MINUS;
    }

    else if (node->value.op_name[0] <= '9' && node->value.op_name[0] >= '0')
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

    // else if (node->value.op_name[0] == '-')
    // {
    //     node->type = MINUS;
    // }

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

    else if (CompareWords(node->value.op_name, &pos, (char*)WHILE))
    {
        node->type = OP_WHILE;
    }

    else if (CompareWords(node->value.op_name, &pos, (char*)PRINTF))
    {
        node->type = OP_PRINTF;
    }

    else if (CompareWords(node->value.op_name, &pos, (char*)IF))
    {
        node->type = OP_IF;
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
        MemoryAllocationError();
    //     *error = PROBLEMS_WITH_READING_FILE;

    fclose(filee);
    return buffer;
}

void CountAmountOfSteps(node_t* node, int* counter)
{
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        CountAmountOfSteps(node->right, counter);
        (*counter) += 2;
    }

    // if (node->type == OP_WHILE)
    // {
    //     (*counter) += 12;
    // }

    // if (node->type == OP_IF)
    // {
    //     (*counter) += 6;
    // }

    if (node->left)
        CountAmountOfSteps(node->left, counter);
    if (node->right)
        CountAmountOfSteps(node->right, counter);

    if (node->type == NUM || node->type == VAR)
    {
        (*counter) += 2;
        return;
    }

    if (node->type == MINUS || node->type == PLUS || node->type == DIVN || node->type == MULT || node->type == OP_PRINTF)
    {
        (*counter)++;
        return;
    }
}
