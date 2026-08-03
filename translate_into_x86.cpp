#include "headers/translate_into_x86.h"
#include <ctype.h>

void TranslateIntoAsm(node_t* node, FILE* file_asm)
{
    assert(node != nullptr);
    assert(file_asm != nullptr);

    int num_of_labels = 0;
    int num_of_nametable = 0;
    int counter_of_loops = 0;
    node_t* first_node = node;
    names_t* nametable = ReturnEmptyNametable();

    fprintf(file_asm, "; nasm -f elf64 -l files/file_asm.lst files/file_asm.asm\n"
                      "; ld -s -o files/file_asm.exe files/file_asm.o\n"
                      "; ./files/file_asm.exe\n\n"
                      "global _start\nsection .bss\nBUF_FOR_PRINTF: resb 32\nsection .data\n");
    fprintf(file_asm, "section .text\n");
    fprintf(file_asm, "\n_start:\n\n");

    TranslateIntoAsmBody(node, file_asm, nametable, &num_of_labels, &num_of_nametable, &counter_of_loops, first_node);
    free(nametable);

}

void TranslateIntoAsmBody(node_t* node, FILE* file_asm, names_t* nametable, int* num_of_labels, int* num_of_nametable, int* counter_of_loops, node_t* first_node)
{
    bool main_func_starts = false;

    if (node->type == MAIN_FUNC)
    {
        main_func_starts = true;
        fprintf(file_asm, "\npush rbp\nmov rbp, rsp\t\t\t\t;start main func\nsub rsp, 256\n\n");
        nametable = ReturnEmptyNametable(); //amount of prms = 32
    }

    if (node->type == OP_IF)
    {
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);

        fprintf(file_asm, "\npop rax\ntest rax, rax\n");
        fprintf(file_asm, "je loop%d\n", *counter_of_loops);
        int old_loop = *counter_of_loops;
        (*counter_of_loops)++;

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);

        fprintf(file_asm, "\nloop%d:\n", old_loop);

        return;
    }

    if (node->type == OP_WHILE)
    {
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);

        fprintf(file_asm, "\npop rax\ntest rax, rax\n");
        fprintf(file_asm, "je loop%d\n", *counter_of_loops);
        int old_loop = *counter_of_loops;
        (*counter_of_loops)++;
        fprintf(file_asm, "loop%d:\n", *counter_of_loops);
        (*counter_of_loops)++;

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);
        fprintf(file_asm, "\npop rax\ntest rax, rax\njne loop%d\n", old_loop + 1);

        fprintf(file_asm, "\nloop%d:\n", old_loop);

        return;
    }
    
    if (node->type == OP_EQUAL && node->left->type == VAR)
    {
        int index = -1, last_j = 0;
        for (int j = 0; j < NT_INITIAL_SIZE; j++)
        {

            if ((nametable + j)->var && (nametable + j)->num_in_stack != -1 && strncmp((nametable + j)->var, node->left->value.op_name, MAX_LEN_OF_OPERATION) == 0)
            {
                index = j; //показывает номер переменной в таблице имен
                break;
            }

            if ((nametable + j)->num_in_stack != -1)
                last_j++;

        }

        if (index == -1) //элемента еще нет в таблице имен
        {
            (nametable + last_j)->var = node->left->value.op_name;
            index = last_j;
        }
        // в index лежит индекс переменной в таблице имен, ща я буду закидывать в стек (или искать в стеке)
        if ((nametable + index)->num_in_stack == -1) //по дефолту это -1, но если значение переменной уже присваивалось, то уже не -1
        {
            int num = ReturnLastStackNum(nametable);
            (nametable + index)->num_in_stack = num + 1;
        }

        TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);

        // и тут уже по номеру в стеке записываем 
        fprintf(file_asm, "pop rax\nmov [rbp - %d], rax\t\t\t\t;var = %s\n", (nametable + index)->num_in_stack * 8, node->left->value.op_name);

        return;
    }

    int amount_of_func_prm = 0;

    if (!node->right && node->type == OP_FUNC) //это типа когда вызваем функцию
    {
        TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node); //это параметры передаем (пока только 1)
        fprintf(file_asm, "\npop rax\t\t\t\t;забрать параметр\ncall %s\n", node->value.op_name);
        fprintf(file_asm, "\npush rax\t\t\t\t;функция с возвращаемым значением\n");
   
        return;
    }

    bool func_starts = false;

    if (node->right && node->type == OP_FUNC)
    {
        fprintf(file_asm, "\n%s:\npush rbp\nmov rbp, rsp\nsub rsp, 256\n\n", node->value.op_name); //ради локальных переменных новый rbp
        fprintf(file_asm, "mov [rbp - 8], rax\t\t\t\t;параметр сюда\n"); 
        names_t* new_nametable = ReturnEmptyNametable();
        TranslateIntoAsmBody(node->left, file_asm, new_nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node); //это параметры передаем
        TranslateIntoAsmBody(node->right, file_asm, new_nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node); //тело функции


        // тут уже выход из функции, возвращаем наш rbp и удаляем параметр из стека
        fprintf(file_asm, "\npop rbx\nadd rsp, 256\npop rbp\nret\n");  
        func_starts = true;       
    }

    if (!func_starts)
    {
        if (!amount_of_func_prm)
        {
            if (node->left)
                TranslateIntoAsmBody(node->left, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);
        }

        if (node->right)
            TranslateIntoAsmBody(node->right, file_asm, nametable, num_of_labels, num_of_nametable, counter_of_loops, first_node);

        if (main_func_starts)
        {
            fprintf(file_asm, "\npop rax\npop rbp ;from main\n");
            fprintf(file_asm, "\nmov rax, 60      ; exit64 (rdi)\nxor rdi, rdi\nsyscall\n");
            return;
        }    
    }

    if (node->type == NUM)
    {
        fprintf(file_asm, "push %lg\t\t\t\t;num\n", node->value.op_num);
        return;
    }

    if (node->type == VAR)
    {
        int index = -1, last_j = 0;
        for (int j = 0; j < NT_INITIAL_SIZE; j++)
        {

            if ((nametable + j)->var && (nametable + j)->num_in_stack != -1 && strncmp((nametable + j)->var, node->value.op_name, MAX_LEN_OF_OPERATION) == 0)
            {
                index = j; //показывает номер переменной в таблице имен
                break;
            }

            if ((nametable + j)->num_in_stack != -1)
                last_j++;

        }

        if (index == -1) //элемента еще нет в таблице имен
        {
            (nametable + last_j)->var = node->value.op_name;
            index = last_j;
        }
        // в index лежит индекс переменной в таблице имен, ща я буду закидывать в стек (или искать в стеке)
        if ((nametable + index)->num_in_stack == -1) //по дефолту это -1, но если значение переменной уже присваивалось, то уже не -1
        {
            int num = ReturnLastStackNum(nametable);
            (nametable + index)->num_in_stack = num + 1;
        }

        fprintf(file_asm, "mov rax, [rbp - %d]\t\t\t\t;var = %s\npush rax\n", (nametable + index)->num_in_stack * 8, node->value.op_name);

        return;
    }

    if (node->type == OP_RETURN) //короче если prm = func(), то вылезает лишний pop, надо к нему push сделать
    {
        fprintf(file_asm, "pop rax\t\t\t\t;возвращаемый параметр\n");
        fprintf(file_asm, "\npop rbx\nadd rsp, 256\npop rbp\nret\n");  
        return;
    }


    if (node->type == MINUS)
    {
        fprintf(file_asm, "\npop rbx\npop rax\nsub rax, rbx\npush rax\n\n");
        return;

    }

    if (node->type == PLUS)
    {
        fprintf(file_asm, "\npop rax\npop rbx\nadd rax, rbx\npush rax\n\n");
        return;

    }

    else if (node->type == DIVN)
    {
        fprintf(file_asm, "\npop rbx\npop rax\ndiv rbx\npush rax\n\n");

    }

    else if (node->type == MULT)
    {
        fprintf(file_asm, "\npop rax\npop rbx\nmul rbx\npush rax\n\n");

    }

    else if (node->type == OP_PRINTF)
    {
        fprintf(file_asm, "\npop rax\t\t\t\t;все передаем через стек ага\nmov rdi, rax\t\t\t\t;передача аргумента на печать\ncall printf_num\n");
    }

    return;
}

node_t* FillTypesInTree(node_t* node)
{
    assert(node != nullptr);

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

    else if (CompareWords(node->value.op_name, &pos, (char*)RETURN)) //когда параметр возвращаем
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
    assert(node != nullptr);

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


node_t* ReturnFuncDeclaration(node_t* first_node, node_t* node)
{
    assert(node != nullptr);
    assert(first_node != nullptr);

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
    assert(node != nullptr);
    assert(first_node != nullptr);
    assert(call_node != nullptr);

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
    assert(first_node != nullptr);
    assert(node != nullptr);

    if (first_node == node) 
    {
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
    assert(node != nullptr);
    assert(counter != nullptr);

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
    assert(first_node != nullptr);
    assert(node != nullptr);
    
    if (first_node->left)
    {
        int d1 = CountLabels(first_node->left, node);
        if (d1 != 0)
            return d1;
    }

    if (first_node == node) 
    {
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