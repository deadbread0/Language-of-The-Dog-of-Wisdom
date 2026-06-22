#include "headers/translate_into_x86.h"
#include <ctype.h>

void TranslateIntoAsm(node_t* node, FILE* file_asm)
{
    int num_of_labels = 0;
    int num_of_nametable = 0;
    int counter_of_commands = 0;
    node_t* first_node = node;
    names_t* nametable = ReturnEmptyNametable();

    fprintf(file_asm, ";nasm -f elf64 -l files/file_asm.lst files/file_asm.asm\n\n"
                      ";ld -s -o files/file_asm.exe files/file_asm.o\n\n"
                      "global _start\nsection .bss\nBUF_FOR_PRINTF: resb 4\nsection .data\n");
    // for (int i = 0; i < last_index_in_nametable; i++)
    //     fprintf(file_asm, "%s equ 0\n", nametable->arr_of_names);//по дефолту все переменные = 0
    fprintf(file_asm, "section .text\n_start:\n\n");

    TranslateIntoAsmBody(node, file_asm, nametable, &num_of_labels, &num_of_nametable, &counter_of_commands, first_node);
    fprintf(file_asm, "\nmov rax, 0x3C      ; exit64 (rdi)\nxor rdi, rdi\nsyscall\n");
}

void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable, int* counter_of_commands, node_t* first_node)
{
    bool main_func_starts = false;

    if (node->type == MAIN_FUNC)
    {
        main_func_starts = true;
        fprintf(file_asm, "push rbp\nmov rbp, rsp\t\t\t\t;start main func\n");
        nametable = ReturnEmptyNametable(); //amount of prms = 32
    }

    if (node->type == OP_IF)
    {
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        int new_counter = CountLabels(node, first_node);

        fprintf(file_asm, "push 0\n");
        fprintf(file_asm, "je %d\n", new_counter);

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;

        return;
    }

    if (node->type == OP_WHILE)
    {
        int old_counter = *num_of_labels;
        int ended_func = CountAmountOfEndFunc(first_node, node);

        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        int new_counter = CountLabels(first_node, node);
        int new_new_counter = 0;
        CountLabelsToTheEnd(node, &new_new_counter);

        fprintf(file_asm, "push 0\n");
        int first_jump = new_counter + new_new_counter + ended_func;

        fprintf(file_asm, "je %d\n", first_jump);

        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;
        
        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        fprintf(file_asm, "push 0\njne %d\n", old_counter);

        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;

        return;
    }
    
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        
        int index = -1, last_j = 0;
        for (int j = 0; j < NT_INITIAL_SIZE; j++)
        {
            // int pos = 0;
            // fprintf(stderr, "%d\n", (nametable + j)->num_in_stack);
            if ((nametable + j)->num_in_stack != -1 && strncmp((nametable + j)->var, node->left->value.op_name, MAX_LEN_OF_OPERATION) == 0)
            {
                index = j; //показывает номер переменной в таблице имен
                break;
            }

            last_j++;
        }
        // fprintf(stderr, "hh");
        if (index == -1) //элемента еще нет в таблице имен
        {
            (nametable + last_j)->var = node->left->value.op_name;
            // fprintf(file_asm, "push 0\t\t\t\t;new prm\n");
            // add stkcounter;
            // return;
            index = last_j;
        }
        // fprintf(stderr, "hh");
            // fprintf(stderr, "%d ", (nametable + index)->num_in_stack);
        // в index лежит индекс переменной в таблице имен, ща я буду закидывать в стек (или искать в стеке)
        if ((nametable + index)->num_in_stack == 0) //по дефолту это 0, но если значение переменной уже присваивалось, то уже не 0
        {
            int num = ReturnLastStackNum(nametable);
            // fprintf(stderr, "%d ", num);
            // if (num < 0)
            //     (nametable + index)->num_in_stack = 1;
            // else
            (nametable + index)->num_in_stack = num + 1;
        }

        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node); //
        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        // // fprintf(file_asm, "push rbp\npush rax\nmov rbp, rsp\nmov rax, [rbp + 16]\nmov [%s], rax\npop rax\npop rbp\nadd rsp, 8\n", (nametable->arr_of_names + index)->var);
        // fprintf(file_asm, "\npop rax\t\t\t\t;var = last element from stack\nmov rbp, rsp\nmov [rbp + 8 * %d], rax\n", (nametable + index)->num_in_stack);
        // return;
        // и тут уже по номеру в стеке записываем 
        // fprintf(file_asm, "pop rax\t\t\t\t;node = var\n");
        fprintf(file_asm, "mov [rbp - 8 * %d], rax\t\t\t\t;var = %s\n", (nametable + index)->num_in_stack, node->left->value.op_name);

        // fprintf(stderr, "hh");
        return;
    }

    int amount_of_func_prm = 0;

    if (!node->right && node->type == OP_FUNC)
    {
        int counter_of_labels_before_func_declaration = 0;

        node_t* decl_node = ReturnFuncDeclaration(first_node, node);
        int l_counter_before_func_declaration = CountLabels(first_node, decl_node);/////// 
        
        if (node->left)
        {
            TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);
            amount_of_func_prm++;//push prm
        }

        fprintf(file_asm, "push 0\npush 0\n");

        fprintf(file_asm, "je %d\n:%d\npop\npop\n", l_counter_before_func_declaration, *num_of_labels);
        (*num_of_labels)++;
    }

    bool func_starts = false;

    if (node->right && node->type == OP_FUNC)
    {
        fprintf(file_asm, "push rbp\nmov rbp, rsp\t\t\t\t;start main func\n");
        nametable = ReturnEmptyNametable(); //amount of prms = 32
        func_starts = true;
    }

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
            fprintf(file_asm, "\npop rbp ;from main\n");
        }    
    }

    else
    {
        fprintf(file_asm, ":%d\npop\npop\n", *num_of_labels);
        (*num_of_labels)++;

        if (node->left->type == VAR)
        {
            int index = 0;
            for (int j = 0; j < NT_INITIAL_SIZE; j++)
            {
                if (strncmp((nametable + j)->var, node->left->value.op_name, MAX_LEN_OF_OPERATION) == 0)
                {
                    // index = j;
                    break;
                }
                index++;
            }
            // (nametable + index)->var = node->left->value.op_name; ??
            // (nametable + index)->value
            // uu

            fprintf(file_asm, "popm %d\n", (nametable + index)->num_of_name);//тут типа переменная объявления функции приняла значение из вызова функции
            (*counter_of_commands) += 2;
        }

        if (node->right)
            TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_commands, first_node);

        node_t* call_node = nullptr;
        node_t* func_node = ReturnFuncCall(first_node, node, call_node);
        int l_counter = CountLabels(first_node, func_node);

        fprintf(file_asm, "push 0\npush 0\n");
        fprintf(file_asm, "je %d\n", l_counter);
        if (func_starts)
        {
            fprintf(file_asm, "\npop rbp ;from func\n");
        }  
    }
    
    if (node->type == NUM)
    {
        fprintf(file_asm, "push %lg\t\t\t\t;num\n", node->value.op_num);
        // add stkcounter
        return;
    }

    if (node->type == VAR)
    {
        // int index = -1, last_j = 0;
        // for (int j = 0; j < NT_INITIAL_SIZE; j++)
        // {
        //     // int pos = 0;
        //     // fprintf(stderr, "%d\n", (nametable + j)->num_in_stack);
        //     if ((nametable + j)->num_in_stack != -1 && strncmp((nametable + j)->var, node->value.op_name, MAX_LEN_OF_OPERATION) == 0)
        //     {
        //         index = j; //показывает номер переменной в таблице имен
        //         break;
        //     }

        //     last_j++;
        // }
        // // fprintf(stderr, "hh");
        // if (index == -1) //элемента еще нет в таблице имен
        // {
        //     (nametable + last_j)->var = node->value.op_name;
        //     // fprintf(file_asm, "push 0\t\t\t\t;new prm\n");
        //     // add stkcounter;
        //     // return;
        //     index = last_j;
        // }
        // // fprintf(stderr, "hh");
        // // в index лежит индекс переменной в таблице имен, ща я буду закидывать в стек (или искать в стеке)
        // if ((nametable + index)->num_in_stack == -1) //по дефолту это -1, но если значение переменной уже присваивалось, то уже не -1
        // {
        //     int num = ReturnLastStackNum(nametable);
        //     if (num < 0)
        //         (nametable + index)->num_in_stack = 1;
        //     else
        //         (nametable + index)->num_in_stack = num + 1;
        // }
        // // и тут уже по номеру в стеке записываем 
        // // fprintf(file_asm, "pop rax\t\t\t\t;node = var\n");
        // fprintf(file_asm, "mov [rbp - 8 * %d], rax\t\t\t\t;var = %s\n", (nametable + index)->num_in_stack, node->value.op_name);

        // // fprintf(stderr, "hh");
        // return;
    }

    if (node->type == MINUS)
    {
        fprintf(file_asm, "sub\n");
        return;

    }

    if (node->type == PLUS)
    {
        fprintf(file_asm, "add\n");
        return;

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
        fprintf(file_asm, "\npop rax\nadd rax, 48\t\t\t\t;make symb from num\nmov [BUF_FOR_PRINTF], rax\n"
                          "\nmov rax, 0x01\nmov rdi, 1\nmov rsi, BUF_FOR_PRINTF\nmov rdx, 1\nsyscall\n"
                          "\nmov rax, [BUF_FOR_PRINTF]\nsub rax, 48\t\t\t\t;make num from symb\nmov [BUF_FOR_PRINTF], rax\n\n");

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


node_t* ReturnFuncDeclaration(node_t* first_node, node_t* node)//
{
    if (first_node->type == OP_FUNC && first_node->right && strncmp(node->value.op_name, first_node->value.op_name, MAX_LEN_OF_FUNC_NAME) == 0)
        node = first_node;

    if (first_node->left)
        node = ReturnFuncDeclaration(first_node->left, node);
    if (first_node->right)
        node = ReturnFuncDeclaration(first_node->right, node);

    return node;
}

node_t* ReturnFuncCall(node_t* first_node, node_t* node, node_t* call_node)
{
    if (first_node->type == OP_FUNC && !first_node->right && strncmp(node->value.op_name, first_node->value.op_name, MAX_LEN_OF_FUNC_NAME) == 0)
    {
        call_node = first_node;
    }
    if (first_node->left)
        call_node = ReturnFuncCall(first_node->left, node, call_node);
    if (first_node->right)
        call_node = ReturnFuncCall(first_node->right, node, call_node);

    return call_node;
}

int CountLabels(node_t* first_node, node_t* node)
{
    if (first_node == node) {
        return 0;
    }

    else if (first_node->type == OP_FUNC || first_node->type == OP_IF || first_node->type == OP_WHILE)
    {
        if (first_node->type == OP_FUNC && first_node->right && strncmp(node->value.op_name, first_node->value.op_name, MAX_LEN_OF_FUNC_NAME) == 0)
            return 0;
        return 1;
    }

    if (first_node->left)
    {
        int d1 = CountLabels(first_node->left, node);
        if (d1 != 0)
            return d1;
    }

    if (first_node->right)
    {
        int d2 = CountLabels(first_node->right, node);
        if (d2 != 0)
            return d2;
    }

    return 0;
}
void CountLabelsToTheEnd(node_t* node, int* counter)
{
    if (node->type == OP_FUNC || node->type == OP_IF || node->type == OP_WHILE)
    {
        (*counter)++;
    }

    if (node->left)
    {
        CountLabelsToTheEnd(node->left, counter);
    }

    if (node->right)
    {
        CountLabelsToTheEnd(node->right, counter);
    }
}

int CountAmountOfEndFunc(node_t* first_node, node_t* node)
{
    if (first_node->left)
    {
        int d1 = CountLabels(first_node->left, node);
        if (d1 != 0)
            return d1;
    }

    if (first_node == node) {
        return 0;
    }

    else if (first_node->type == OP_FUNC || first_node->type == OP_IF || first_node->type == OP_WHILE)
    {
        if (first_node->type == OP_FUNC && first_node->right && strncmp(node->value.op_name, first_node->value.op_name, MAX_LEN_OF_FUNC_NAME) == 0)
            return 0;
        return 1;
    }

    if (first_node->right)
    {
        int d2 = CountLabels(first_node->right, node);
        if (d2 != 0)
            return d2;
    }

    return 0;
}