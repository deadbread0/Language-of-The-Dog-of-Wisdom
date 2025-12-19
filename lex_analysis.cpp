#include "headers\lex_analysis.h"
#include <ctype.h>

size_t LexAnalysis(char* s, int* pos, node_t* tokens)
{
    size_t n = 0;

    while (s[*pos] != '\0')
    {
        if (CompareWords(s, pos, (char*)"/*"))
        {
            while (!CompareWords(s, pos, (char*)"*/"))
                (*pos)++;

            continue;
        }

        if (s[*pos] == '(')
        {
            tokens[n].type = BRACKET_OPEN;
            (tokens + n)->value.op_name = (char*)"(";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == ')')
        {
            tokens[n].type = BRACKET_CLOSE;
            (tokens + n)->value.op_name = (char*)")";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == '{')
        {
            tokens[n].type = FBRACKET_OPEN;
            (tokens + n)->value.op_name = (char*)"{";
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == '}')
        {
            tokens[n].type = FBRACKET_CLOSE;
            (tokens + n)->value.op_name = (char*)"}";
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)IF))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_IF;
            (tokens + n)->value.op_name = (char*)IF;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)WHILE))
        {
            (*pos)--;
            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 
            tokens[n].type = OP_WHILE;
            (tokens + n)->value.op_name = (char*)WHILE;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)EQUAL))
        {
            if (s[*pos] == '=')
            {
                tokens[n].type = COMP;
                (tokens + n)->value.op_name = (char*)"==";
                n++;
                (*pos)++;
                continue;
            }

            (*pos)--;

            while (isspace(s[*pos]))
            {
                (*pos)--;
            } 

            tokens[n].type = OP_EQUAL;
            (tokens + n)->value.op_name = (char*)"=";
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

        if (s[*pos] == ';')
        {
            tokens[n].type = UOP;
            (tokens + n)->value.op_name = (char*)";";
            n++;
            (*pos)++;
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

        int amount_of_const_func = sizeof(arr_of_const_func) / sizeof(arr_of_const_func[0]);

        for (int i = 0; i < amount_of_const_func; i++)
        {
            if (CompareWords(s, pos, (char*)arr_of_const_func[i]))
            {
                (tokens + n)->type = OP_FUNC;
                (tokens + n)->value.op_name = (char*)arr_of_const_func[i];
                n++;

                continue;
            }
        }

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
            (tokens + n)->type = VAR;

            int c = 0;

            if (isspace(s[(*pos) + c]))
            {
                while (isspace(s[(*pos) + c]))
                {
                    c++;
                }
            }

            if (s[(*pos) + c] == '(')
                (tokens + n)->type = OP_FUNC;

            (tokens + n)->value.op_name = arr; 
            n++;
            continue;
        }
        
        if (s[*pos] == '$')
        {
            (tokens + n)->type = END;
            (tokens + n)->value.op_name = (char*)"$";
            n++;
            (*pos)++;
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