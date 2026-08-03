#include "headers/lex_analysis_asm.h"

size_t LexAnalysisAsm(char* s, int* pos, int* amount_of_nodes, node_t* tokens, loops_t* loops_struct, bool* start)
{
    assert(s != nullptr);
    assert(pos != nullptr);
    assert(amount_of_nodes != nullptr);
    assert(tokens != nullptr);
    assert(loops_struct != nullptr);
    assert(start != nullptr);

    size_t n = *amount_of_nodes;

    while (s[*pos] != '\0')
    {
        if (!(*start))
        {
            while (!CompareWords(s, pos, (char*)"_start:"))
            {
                (*pos)++;
            }
            *start = true;
            continue;
        }

        if (s[*pos] == ';')
        {
            while (s[*pos] != '\n')
            {
                (*pos)++;
            }
        }

        if (s[*pos] == '[')
        {
            tokens[n].type = QBRACKET_OPEN_T;
            (tokens + n)->value.op_name = (char*)"[";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == ']')
        {
            tokens[n].type = QBRACKET_CLOSE_T;
            (tokens + n)->value.op_name = (char*)"]";
            n++;
            (*pos)++;
            continue;
        } 

        if (CompareWords(s, pos, (char*)"BUF_FOR_PRINTF"))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = MEM;
            (tokens + n)->value.op_name = (char*)"BUF_FOR_PRINTF";
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)PUSH))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = PUSH_T;
            (tokens + n)->value.op_name = (char*)PUSH;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)TEST))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = TEST_T;
            (tokens + n)->value.op_name = (char*)TEST;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)CMP))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = CMP_T;
            (tokens + n)->value.op_name = (char*)CMP;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)JE))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = JE_T;
            (tokens + n)->value.op_name = (char*)JE;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)JNE))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = JNE_T;
            (tokens + n)->value.op_name = (char*)JNE;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)JMP))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = JMP_T;
            (tokens + n)->value.op_name = (char*)JMP;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)CQTO))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = CQTO_T;
            (tokens + n)->value.op_name = (char*)CQTO;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)REMAKE10))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_PRINTF;
            (tokens + n)->value.op_name = (char*)REMAKE10;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)START))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)POP))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = POP_T;
            (tokens + n)->value.op_name = (char*)POP;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)XOR))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = XOR_T;
            (tokens + n)->value.op_name = (char*)XOR;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RAX))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RAX;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RBX))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RBX;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RCX))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RCX;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RDX))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RDX;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RBP))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RBP;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RSP))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RSP;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RDI))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RDI;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RSI))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = REG;
            (tokens + n)->value.op_name = (char*)RSI;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)MOV))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_MOV;
            (tokens + n)->value.op_name = (char*)MOV;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)POINTX2))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_POINTX2;
            (tokens + n)->value.op_name = (char*)POINTX2;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)CALL))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = CALL_T;
            (tokens + n)->value.op_name = (char*)CALL;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)SYSCALL))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = SYSCALL_T;
            (tokens + n)->value.op_name = (char*)SYSCALL;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)POINTX2))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_POINTX2;
            (tokens + n)->value.op_name = (char*)POINTX2;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)ADD))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = ADD_T;
            (tokens + n)->value.op_name = (char*)ADD;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)SUB))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = SUB_T;
            (tokens + n)->value.op_name = (char*)SUB;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)MUL))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = MUL_T;
            (tokens + n)->value.op_name = (char*)MUL;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)DIV))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = DIV_T;
            (tokens + n)->value.op_name = (char*)DIV;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)RET))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_RETURN;
            (tokens + n)->value.op_name = (char*)RET;
            n++;
            (*pos)++;
            continue;
        }

        if ('0' <= s[*pos] && s[*pos] <= '9')
        {
            int num = 0;

            while ('0' <= s[*pos] && s[*pos] <= '9')
            {
                num = num * 10 + (int)s[*pos] - '0';
                (*pos)++;
            }

            tokens[n].type = NUM;
            (tokens + n)->value.op_num = num;///
            n++;

            continue;
        }

        if (s[*pos] == '-')
        {
            tokens[n].type = MINUS;
            (tokens + n)->value.op_name = (char*)"-";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == '+')
        {
            tokens[n].type = PLUS;
            (tokens + n)->value.op_name = (char*)"+";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == '*')
        {
            tokens[n].type = MULT;
            (tokens + n)->value.op_name = (char*)"*";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == '/')
        {
            tokens[n].type = DIVN;
            (tokens + n)->value.op_name = (char*)"/";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == ',')
        {
            tokens[n].type = COMMA_T; //
            (tokens + n)->value.op_name = (char*)",";
            n++;
            (*pos)++;
            continue;
        }

        // int amount_of_const_func = sizeof(arr_of_const_func) / sizeof(arr_of_const_func[0]);

        // for (int i = 0; i < amount_of_const_func; i++)
        // {
        //     if (CompareWords(s, pos, (char*)arr_of_const_func[i]))
        //     {
        //         (tokens + n)->type = OP_FUNC;
        //         (tokens + n)->value.op_name = (char*)arr_of_const_func[i];
        //         n++;

        //         continue;
        //     }
        // }

        if (isalnum(s[*pos]) || s[*pos] == '_') // 
        {
            char* arr = (char*)calloc(MAX_LEN_OF_OPERATION, sizeof(char));
            int j = 0;

            while (isalnum(s[*pos]) || s[*pos] == '_')
            {
                arr[j] = s[*pos];
                j++;
                (*pos)++;
            }
            int last_in_struct = loops_struct->last_index;
            if (s[*pos] == ':')
                loops_struct->arr[last_in_struct].addr_start = false; //узлы, на которые прыгать
            
            else
                loops_struct->arr[last_in_struct].addr_start = true; //узлы, с которых прыгать

            loops_struct->arr[last_in_struct].func_struct.name_of_func = arr;
            loops_struct->arr[last_in_struct].func_struct.num_of_func = FindNumOfFunc(arr, loops_struct);
            loops_struct->last_index++;

            (tokens + n)->type = OP_FUNC;
            (tokens + n)->value.op_name = arr;
            n++;

            int c = 0;

            if (isspace(s[(*pos) + c]))
            {
                while (isspace(s[(*pos) + c]))
                    c++;
            }

            continue;
        }

        if (isspace(s[*pos]))
        {
            while (isspace(s[*pos]))
            {
                (*pos)++;
            }
            continue;
        }
    }

    *amount_of_nodes = n;
    return n;
}

bool CompareWords(char* s, int* pos, char* word)
{
    int len_of_if = strlen(word), counter = 0;

    for (int i = 0; i < len_of_if; i++)
    {
        if (s[(*pos) + i] == word[i])
            counter++;
        else
            break;
    }

    if (counter == len_of_if)
    {
        (*pos) += len_of_if;
        SkipSpace(s, pos);
        return true;
    }

    return false;
}

int FindNumOfFunc(char* func_name, loops_t* loop_struct)
{
    int last_i = loop_struct->last_index;

    for (int i = 0; i < last_i; i++)
    {
        if (loop_struct->arr[i].func_struct.name_of_func != nullptr)
        {
            if (strncmp(func_name, loop_struct->arr[i].func_struct.name_of_func, strlen(func_name)) == 0)
                return loop_struct->arr[i].func_struct.num_of_func;
        }
        
    }

    loop_struct->last_func_num++;
    return loop_struct->last_func_num - 1;
}