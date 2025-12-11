#include "headers\lex_analysis.h"
#include <ctype.h>

size_t LexAnalysis(char* s, int* pos, node_t* tokens)
{
    size_t n = 0;
    while (s[*pos] != '\0')
    {
        if (s[*pos] == '(')
        {
            tokens[n].type = BRACKET_OPEN;
            tokens[n].value.op_name = (char*)s[*pos];
            n++;
            (*pos)++;
            continue;
        }

        if (s[*pos] == ')')
        {
            tokens[n].type = BRACKET_CLOSE;
            tokens[n].value.op_name = (char*)s[*pos];
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)IF))
        {
            tokens[n].type = OP;
            tokens[n].value.op_name = (char*)IF;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)EQUAL))
        {
            tokens[n].type = OP;
            tokens[n].value.op_name = (char*)EQUAL;
            n++;
            (*pos)++;
            continue;
        }

        if (CompareWords(s, pos, (char*)"/*"))
        {
            (*pos)++;
            while (!CompareWords(s, pos, (char*)"*/"))
            {
                (*pos)++;
            }
            (*pos)++;
            continue;
        }

        if ('0' <= s[*pos] && s[*pos] <= '9')
        {
            int num = 0;

            while ('0' <= s[*pos] && s[*pos] <= '9')
            {
                num = num * 10 + (int)s[*pos] - '0';
            }

            tokens[n].type = NUM;
            tokens[n].value.op_num = num;

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