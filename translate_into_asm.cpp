#include "headers/translate_into_asm.h"
#include <ctype.h>

void TranslateIntoAsm(node_t* node, FILE* file_asm, names_t* nametable, int last_index_in_nametable)
{
    int num_of_labels = 0;
    int num_of_nametable = last_index_in_nametable;
    int counter_of_commands = 0;
    node_t* first_node = node;

    TranslateIntoAsmBody(node, file_asm, nametable, &num_of_labels, &num_of_nametable, &counter_of_commands, first_node);
}

void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable, int* counter_of_commands, node_t* first_node)
{
    bool main_func_starts = false;

    if (node->type == MAIN_FUNC)
        main_func_starts = true;

    if (node->type == OP_IF)
    {
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        int new_counter = 0;
        CountAmountOfSteps(node->right, &new_counter);

        fprintf(file_asm, "push 0\n");
        (*counter_of_commands) += 2;//counter_of_commands правильно считает от начала до je включительно
        fprintf(file_asm, "je %d\n", new_counter + *counter_of_commands + 2);///??? +2 для перехода к двоеточию и перехода к номеру команды после je
        (*counter_of_commands) += 2;

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 4;

        return;
    }

    if (node->type == OP_WHILE)
    {
        int new_counter = 0;
        CountAmountOfSteps(node, &new_counter);

        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        fprintf(file_asm, "push 0\n");
        (*counter_of_commands) += 2;
        int first_jump = new_counter + *counter_of_commands;

        fprintf(file_asm, "je %d\n", first_jump);
        (*counter_of_commands) += 2;
        int second_jump = *counter_of_commands;
        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 4;
        
        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        fprintf(file_asm, "push 0\njne %d\n", second_jump);
        (*counter_of_commands) += 4;

        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 4;

        return;
    }
    
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        
        int index = 0;
        for (int j = 0; j < *num_of_nametable; j++)
        {
            if (strncmp((nametable + j)->var, node->left->value.op_name, MAX_LEN_OF_OPERATION) == 0)
            {
                index = j;
                break;
            }
        }

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        fprintf(file_asm, "popm %d\n", (nametable + index)->num_of_name);
        (*counter_of_commands) += 2;
        return;
    }

    int amount_of_func_prm = 0;

    if (!node->right && node->type == OP_FUNC)
    {
        int counter_of_all_nodes = 0, counter_of_nodes_before_func_declaration = 0;
        CountAmountOfStepsForFunctionDeclaration(first_node, &counter_of_all_nodes, node->value.op_name, &counter_of_nodes_before_func_declaration);
        
        if (node->left)
        {
            TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);
            amount_of_func_prm++;//push prm
        }

        fprintf(file_asm, "push 0\npush 0\n");
        (*counter_of_commands) += 4;

        fprintf(file_asm, "je %d\n:%d\npop\npop\n", counter_of_nodes_before_func_declaration, *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 6;
    }

    bool func_starts = false;

    if (node->right && node->type == OP_FUNC)
        func_starts = true;

    if (!func_starts)
    {
        if (!amount_of_func_prm)
        {
            if (node->left)
                TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);
        }

        if (node->right)
            TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        if (main_func_starts)
        {
            fprintf(file_asm, "hlt\n");
            (*counter_of_commands)++;
        }    
    }

    else
    {
        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;
        (*counter_of_commands) += 4;

        if (node->left->type == VAR)
        {
            int index = 0;
            for (int j = 0; j < *num_of_nametable; j++)
            {
                if (strncmp((nametable + j)->var, node->left->value.op_name, MAX_LEN_OF_OPERATION) == 0)
                {
                    index = j;
                    break;
                }
            }

            fprintf(file_asm, "popm %d\n", (nametable + index)->num_of_name);//тут типа переменная объявления функции приняла значение из вызова функции
            (*counter_of_commands) += 2;
        }

        if (node->right)
            TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        int counter_of_all_nodes = 0, fake_counter_dont_use_it = 0, really_useful_counter = 0;
        CountAmountOfStepsForFunctionCall(first_node, &counter_of_all_nodes, node->value.op_name, &fake_counter_dont_use_it, nametable, num_of_nametable, &really_useful_counter);

        fprintf(file_asm, "push 0\npush 0\n");
        (*counter_of_commands) += 4;
        fprintf(file_asm, "je %d\n", really_useful_counter - 2);//подгооон
        (*counter_of_commands) += 2;
    }
    
    if (node->type == NUM)
    {
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
            if (strncmp((nametable + j)->var, node->value.op_name, MAX_LEN_OF_OPERATION) == 0)
            {
                index = j;
                break;
            }
        }

        fprintf(file_asm, "pushm %d\n", index);
        (*counter_of_commands) += 2;
        return;
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
        if (strlen(node->value.op_name) > 1 && node->value.op_name[1] <= '9' && node->value.op_name[1] >= '0')
        {
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

    else if (CompareWords(node->value.op_name, &pos, (char*)MAIN))
    {
        node->type = MAIN_FUNC;
    }

    else if (CompareWords(node->value.op_name, &pos, (char*)IF))
    {
        node->type = OP_IF;
    }

    else if (CompareWords(node->value.op_name, &pos, (char*)RETURN))
    {
        node->type = OP_RETURN;
    }

    else 
    {
        int counter_func = 0;
        int amount_of_const_func = sizeof(arr_of_const_func) / sizeof(arr_of_const_func[0]);

        for (int i = 0; i < amount_of_const_func; i++)
        {
            int prm_for_comp = 0;
            if (CompareWords(node->value.op_name, &prm_for_comp, (char*)arr_of_const_func[i]))
            {
                node->type = OP_FUNC;
                node->value.op_name = (char*)arr_of_const_func[i];
                counter_func++;
            }
        }
        if (counter_func == 0)
        {
            FillUsersFunc(node);
        }
    }
    return node;
}

node_t* FillUsersFunc(node_t* node)
{

    if (node->left)
        FillUsersFunc(node->left);
    if (node->right)
        FillUsersFunc(node->right);

    if (node->left && node->type == VAR && (node->left->type == NUM || node->left->type == VAR))
        node->type = OP_FUNC;
    
    return node;
}

char* PutTreeFromFileToBuffer(int size)
{
    char* buffer = (char*)calloc(size, sizeof(char));//

    FILE* filee = fopen("files/tree.txt", "r");
    buffer = fgets(buffer, size, filee);

    if (!buffer)
        MemoryAllocationError();

    fclose(filee);
    return buffer;
}


// дисклеймер!! все то, что будет дальше похоже на подгон, ну так да, частично это правда, так что мб
// на каких-то программах это не будет работать(( я просто слишком поздно поняла, что проще не дебажить эту фигню а
// написать норм прыжки в процессоре


void CountAmountOfSteps(node_t* node, int* counter)
{
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        (*counter) += 2;
    }

    if (node->type == OP_WHILE && (*counter) != 0)
    {
        (*counter) += 10;
    }

    if (node->type == OP_WHILE && (*counter) == 0)
    {
        (*counter) += 14;//12
    }


    if (node->type == OP_IF)
    {
        (*counter) += 6;//6
    }

    if (!node->right && node->type == OP_FUNC)
    {
        (*counter) += 8;//8
    }

    if (node->right && node->type == OP_FUNC)
    {
        (*counter) += 6;//8
    }

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

    if (node->left)
        CountAmountOfSteps(node->left, counter);
    if (node->right)
        CountAmountOfSteps(node->right, counter);

}


void CountAmountOfStepsForFunctionDeclaration(node_t* node, int* counter, char* node_name, int* total)
{
    
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        (*counter) += 2;
    }

    if (!node->right && node->type == OP_FUNC)
    {
        (*counter) += 8;//8
    }

    if (node->type == OP_WHILE)
    {
        (*counter) += 10;//4
    }

    if (node->type == OP_IF)
    {
        (*counter) += 6;//2
    }

    if (node->type == NUM || node->type == VAR)
    {
        (*counter) += 2;
    }

    if (node->type == MINUS || node->type == PLUS || node->type == DIVN || node->type == MULT || node->type == OP_PRINTF)
    {
        (*counter)++;
    }

    if (node->right && strncmp(node->value.op_name, node_name, MAX_LEN_OF_FUNC_NAME) == 0 &&  node->type == OP_FUNC && node->right->type == UOP)
    {
        *total = (*counter) + 1;// +1 тк hlt в main 
        return;
    }

    if (node->right && node->type == OP_FUNC)
    {
        (*counter) += 6;//8
    }

    if (node->left)
        CountAmountOfStepsForFunctionDeclaration(node->left, counter, node_name, total);
    if (node->right)
        CountAmountOfStepsForFunctionDeclaration(node->right, counter, node_name, total);

}

void CountAmountOfStepsForFunctionCall(node_t* node, int* counter, char* node_name, int* total, names_t* nametable, int* num_of_nametable, int* realtotal)
{
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        (*total) += 2;
    }

    else if (node->type == OP_WHILE)
    {
        (*total) += 10;//4
    }

    else if (node->type == OP_IF)
    {
        (*total) += 6;//2
    }

    else if (node->right && node->type == OP_FUNC)
    {
        (*total) += 6;//8
    }

    else if (node->type == NUM || node->type == VAR)
    {
        (*total) += 2;
    }

    else if (node->type == MINUS || node->type == PLUS || node->type == DIVN || node->type == MULT || node->type == OP_PRINTF)
    {
        (*total)++;
    }

    else if (!node->right && node->type == OP_FUNC && strncmp(node->value.op_name, node_name, MAX_LEN_OF_FUNC_NAME) == 0)
    {
        (*counter)++;
        (*total) += 6;
        for (int i = 0; i < *num_of_nametable; i++)
        {
            if (strncmp(node_name, (nametable + i)->var, MAX_LEN_OF_FUNC_NAME) == 0 && (nametable + i)->value == *counter)
            {
                (nametable + i)->value = 0;
                (*realtotal) = (*total);
                return;
            }
        }
        
    }


    if (node->left)
        CountAmountOfStepsForFunctionCall(node->left, counter, node_name, total, nametable, num_of_nametable, realtotal);
    if (node->right)
        CountAmountOfStepsForFunctionCall(node->right, counter, node_name, total, nametable, num_of_nametable, realtotal);

}