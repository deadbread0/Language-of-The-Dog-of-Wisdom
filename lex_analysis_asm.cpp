#include "headers/lex_analysis_asm.h"

size_t LexAnalysisAsm(char* s, int* pos, node_t* tokens)
{
    size_t n = 0;

    while (s[*pos] != '\0')
    {

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

        if (CompareWords(s, pos, (char*)PUSH))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_STACK;
            (tokens + n)->value.op_name = (char*)PUSH;
            n++;
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
            tokens[n].type = OP_STACK;
            (tokens + n)->value.op_name = (char*)POP;
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
            tokens[n].type = OP_1_ARG;
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
            tokens[n].type = OP_NO_ARGS;
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
            tokens[n].type = OP_2_ARGS;
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
            tokens[n].type = OP_2_ARGS;
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
            tokens[n].type = OP_2_ARGS;
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
            tokens[n].type = OP_2_ARGS;
            (tokens + n)->value.op_name = (char*)DIV;
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

        if (isalnum(s[*pos]))
        {
            char* arr = (char*)calloc(MAX_LEN_OF_OPERATION, sizeof(char));
            int j = 0;

            while (isalnum(s[*pos]))
            {
                arr[j] = s[*pos];
                j++;
                (*pos)++;
            }
            (tokens + n)->type = OP_FUNC;

            int c = 0;

            if (isspace(s[(*pos) + c]))
            {
                while (isspace(s[(*pos) + c]))
                {
                    c++;
                }
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

    if (counter == len_of_if)//
    {
        (*pos) += len_of_if;
        SkipSpace(s, pos);
        return true;
    }

    return false;
}