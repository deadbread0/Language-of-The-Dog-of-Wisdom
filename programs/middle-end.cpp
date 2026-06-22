
#include "../headers/middlend.h"

int main()
{
    int new_size = GetSizeOfInputFile(tree_path);
    char* buf = PutTreeFromFileToBuffer(new_size);

    int pos = 0;

    node_t* new_tree = ReadNode(&pos, buf);
    new_tree = FillTypesInTree(new_tree);
    new_tree = FillUsersFunc(new_tree);
    new_tree = ConstantOptimization(new_tree);

    FILE* new_output_file = fopen(new_tree_dump_path, "w");
    DumpTree(new_tree, new_output_file);
    fclose(new_output_file);

    FILE* filee_for_tree = fopen(tree_path, "w");
    PutTreeInFile(new_tree, filee_for_tree);
    free(new_tree);
    fclose(filee_for_tree);
    
    return 0;
}

#include "headers/lex_analysis.h"
#include <ctype.h>

// size_t LexAnalysis(char* s, int* pos, node_t* tokens)
// {
//     size_t n = 0;

//     while (s[*pos] != '\0')
//     {
            
//         if (CompareWords(s, pos, (char*)"/*"))
//         {
//             while (!CompareWords(s, pos, (char*)"*/"))
//                 (*pos)++;

//             continue;
//         }

//         if (s[*pos] == '(')
//         {
//             tokens[n].type = BRACKET_OPEN;
//             (tokens + n)->value.op_name = (char*)"(";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == ')')
//         {
//             tokens[n].type = BRACKET_CLOSE;
//             (tokens + n)->value.op_name = (char*)")";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == '{')
//         {
//             tokens[n].type = FBRACKET_OPEN;
//             (tokens + n)->value.op_name = (char*)"{";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == '}')
//         {
//             tokens[n].type = FBRACKET_CLOSE;
//             (tokens + n)->value.op_name = (char*)"}";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (CompareWords(s, pos, (char*)PRINTF))
//         {
//             (*pos)--;
//             while (isspace(s[*pos]))
//             {
//                 (*pos)--;
//             } 
//             tokens[n].type = OP_PRINTF;
//             (tokens + n)->value.op_name = (char*)PRINTF;
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (CompareWords(s, pos, (char*)WHILE))
//         {
//             (*pos)--;
//             while (isspace(s[*pos]))
//             {
//                 (*pos)--;
//             } 
//             tokens[n].type = OP_WHILE;
//             (tokens + n)->value.op_name = (char*)WHILE;
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (CompareWords(s, pos, (char*)MAIN))
//         {
//             (*pos)--;
//             while (isspace(s[*pos]))
//             {
//                 (*pos)--;
//             } 
//             tokens[n].type = MAIN_FUNC;
//             (tokens + n)->value.op_name = (char*)MAIN;
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (CompareWords(s, pos, (char*)IF))
//         {
//             (*pos)--;
//             while (isspace(s[*pos]))
//             {
//                 (*pos)--;
//             } 
//             tokens[n].type = OP_IF;
//             (tokens + n)->value.op_name = (char*)IF;
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (CompareWords(s, pos, (char*)RETURN))
//         {
//             (*pos)--;
//             while (isspace(s[*pos]))
//             {
//                 (*pos)--;
//             } 
//             tokens[n].type = OP_RETURN;
//             (tokens + n)->value.op_name = (char*)RETURN;
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (CompareWords(s, pos, (char*)EQUAL))
//         {
//             if (s[*pos] == '=')
//             {
//                 tokens[n].type = COMP;
//                 (tokens + n)->value.op_name = (char*)"==";
//                 n++;
//                 (*pos)++;
//                 continue;
//             }

//             (*pos)--;

//             while (isspace(s[*pos]))
//             {
//                 (*pos)--;
//             } 

//             tokens[n].type = OP_EQUAL;
//             (tokens + n)->value.op_name = (char*)"=";
//             n++;
//             (*pos)++;
//             continue;
//         }


//         if ('0' <= s[*pos] && s[*pos] <= '9')
//         {
//             int num = 0;

//             while ('0' <= s[*pos] && s[*pos] <= '9')
//             {
//                 num = num * 10 + (int)s[*pos] - '0';
//                 (*pos)++;
//             }

//             tokens[n].type = NUM;
//             (tokens + n)->value.op_num = num;///
//             // fprintf(stderr, "num %d ", num);
//             n++;

//             continue;
//         }

//         if (s[*pos] == '?')
//         {
//             tokens[n].type = UOP;
//             (tokens + n)->value.op_name = (char*)"?";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == '-')
//         {
//             tokens[n].type = MINUS;
//             (tokens + n)->value.op_name = (char*)"-";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == '+')
//         {
//             tokens[n].type = PLUS;
//             (tokens + n)->value.op_name = (char*)"+";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == '*')
//         {
//             tokens[n].type = MULT;
//             (tokens + n)->value.op_name = (char*)"*";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (s[*pos] == '/')
//         {
//             tokens[n].type = DIVN;
//             (tokens + n)->value.op_name = (char*)"/";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         int amount_of_const_func = sizeof(arr_of_const_func) / sizeof(arr_of_const_func[0]);

//         for (int i = 0; i < amount_of_const_func; i++)
//         {
//             if (CompareWords(s, pos, (char*)arr_of_const_func[i]))
//             {
//                 (tokens + n)->type = OP_FUNC;
//                 (tokens + n)->value.op_name = (char*)arr_of_const_func[i];
//                 n++;

//                 continue;
//             }
//         }

//         if (isalnum(s[*pos]))
//         {
//             (tokens + n)->value.op_name = (char*)calloc(MAX_LEN_OF_OPERATION, sizeof(char));//проверочку бы
//             int j = 0;

//             while (isalnum(s[*pos]))
//             {
//                 // fprintf(stderr, "%c ", s[*pos]);
//                 (tokens + n)->value.op_name[j] = s[*pos];
//                 // fprintf(stderr, "%c ", (tokens + n)->value.op_name[j]);
//                 j++;
//                 (*pos)++;
//             }
//             (tokens + n)->value.op_name[j] = '\0';
//             (tokens + n)->type = VAR;

//             int c = 0;

//             if (isspace(s[(*pos) + c]))
//             {
//                 while (isspace(s[(*pos) + c]))
//                 {
//                     c++;
//                 }
//             }

//             if (s[(*pos) + c] == '(')
//                 (tokens + n)->type = OP_FUNC;

//             // (tokens + n)->value.op_name = arr; 
//             n++;
//             continue;
//         }
        
//         if (s[*pos] == '$')
//         {
//             (tokens + n)->type = END;
//             (tokens + n)->value.op_name = (char*)"$";
//             n++;
//             (*pos)++;
//             continue;
//         }

//         if (isspace(s[*pos]))
//         {
//             while (isspace(s[*pos]))
//             {
//                 (*pos)++;
//             }
//             continue;
//         }
//     }
//     return n;
// }

// bool CompareWords(char* s, int* pos, char* word)
// {
//     int len_of_if = strlen(word), counter = 0;

//     for (int i = 0; i < len_of_if; i++)
//     {
//         if (s[(*pos) + i] == word[i])
//             counter++;
//         else
//             break;
//     }

//     if (counter == len_of_if)//
//     {
//         (*pos) += len_of_if;
//         SkipSpace(s, pos);
//         return true;
//     }

//     return false;
// }

#include "headers/tree_functions.h"
#include <cstring>

// node_t* GetNodeComb(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
//     val->type = UOP;
//     node_t* prev_val = nullptr;

//     do
//     {
//         if (tokens[*pos].type == OP_IF || tokens[*pos].type == OP_WHILE || tokens[*pos].type == MAIN_FUNC || tokens[*pos].type == OP_FUNC)
//             val->right = GetNodeIF(tokens, pos);

//         else
//             val->right = GetNodeA(tokens, pos);

//         (*pos)++;

//         prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
//         prev_val = val;
//         val = NewOpNode((char*)COMB, prev_val, nullptr);

//         if (tokens[*pos].type == UOP)
//             (*pos)++;

//     } while (tokens[*pos].type != END);

//     if (tokens[*pos].type != END)
//         SyntaxError();

//     (*pos)++;

//     return prev_val;
// }


// node_t* GetNodeNum(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     int val = 0;

//     if (tokens[*pos].type != NUM)
//         SyntaxError();

//     (*pos)++;

//     return tokens + (*pos) - 1;
// }

// node_t* GetNodeAddOrSub(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     node_t* val = GetNodeMulOrDiv(tokens, pos);

//     while (tokens[*pos].type == PLUS || tokens[*pos].type == MINUS)
//     {
//         int prev_pos = *pos;
//         (*pos)++;

//         node_t* val2 = GetNodeMulOrDiv(tokens, pos);
//         (tokens + prev_pos)->left = val;
//         (tokens + prev_pos)->right = val2;
//         val = tokens + prev_pos;
//     }

//     return val;
// }

// node_t* GetNodeMulOrDiv(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     node_t* val = GetNodeBracket(tokens, pos);

//     while (tokens[*pos].type == MULT || tokens[*pos].type == DIVN)
//     {
//         int prev_pos = *pos;
//         (*pos)++;

//         node_t* val2 = GetNodeBracket(tokens, pos);
//         (tokens + prev_pos)->left = val;
//         (tokens + prev_pos)->right = val2;
//         val = tokens + prev_pos;
//     }

//     return val;
// }

// node_t* GetNodeBracket(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     if (tokens[*pos].type == BRACKET_OPEN)
//     {
//         (*pos)++;
//         node_t* val = GetNodeAddOrSub(tokens, pos);
//         (*pos)++;
//         return val;
//     }

//     else if (tokens[*pos].type == VAR || tokens[*pos].type == OP_FUNC)
//         return GetNodeVar(tokens, pos);

//     else
//         return GetNodeM(tokens, pos);
// }

// node_t* GetNodeM(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     if (tokens[*pos].type == MINUS)
//     {
//         (*pos)++;
//         node_t* node = GetNodeNum(tokens, pos);
//         node->value.op_num = node->value.op_num * (-1);
//         return node;
//     }

//     else
//         return GetNodeNum(tokens, pos);
// }

// node_t* GetNodeVar(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     // fprintf(stderr, "%d %s", tokens[(*pos)].type, tokens[(*pos)].value.op_name);

//     if (tokens[(*pos)].type == OP_FUNC || tokens[*pos].type == OP_PRINTF || tokens[*pos].type == OP_RETURN)
//         return GetNodeFunction(tokens, pos);
    
//     (*pos)++;

//     return tokens + (*pos) - 1;
// }

// node_t* GetNodeFunction(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     int fpos = (*pos);

//     (*pos)++;
//     (tokens + fpos)->right = GetNodeBracket(tokens, pos);
//     return tokens + fpos;
// }

// node_t* GetNodeA(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//     node_t* node = GetNodeVar(tokens, pos);
    
//     if (tokens[*pos].type == OP_EQUAL)
//     {
//         int prev_pos = *pos;
//         (*pos)++;
//         fprintf(stderr, "%d %d ", tokens[(*pos)].type, tokens[(*pos)].value.op_num);
//         (tokens + prev_pos)->left = node;
//         (tokens + prev_pos)->right = GetNodeAddOrSub(tokens, pos);
//         return tokens + prev_pos;
//     }

//     return node;
// }

// node_t* GetNodeIF(node_t* tokens, int* pos)
// {
//     assert(tokens != nullptr);
//     assert(pos != nullptr);

//         // fprintf(stderr, "%d %s ", tokens[(*pos)].type, tokens[(*pos)].value.op_name);
//     int if_pos = *pos;
//     (*pos)++;

//     node_t* new_node = nullptr;

//     if (tokens[*pos].type == BRACKET_OPEN)
//         new_node = GetNodeAddOrSub(tokens, pos);
//         // fprintf(stderr, "%d %d ", new_node->type, new_node->value.op_num);

//     node_t* val = NewOpNode((char*)COMB, nullptr, nullptr);
//     node_t* prev_val = nullptr;

//     if (tokens[*pos].type == UOP)
//         return nullptr;

//     if (tokens[*pos].type != FBRACKET_OPEN)
//     {
//         node_t* new_new_node = nullptr;
//         new_new_node = GetNodeA(tokens, pos);
//         (tokens + if_pos)->left = new_node;
//         (tokens + if_pos)->right = new_new_node;
//         return tokens + if_pos;
//     }

//     (*pos)++;
//     int counter_of_fbracket = 1, counter_of_fbracket_closed = 0;

//     while (counter_of_fbracket != counter_of_fbracket_closed)
//     {
//         if (tokens[*pos].type == FBRACKET_CLOSE)
//             (*pos)++;
//     do
//     {
//         if (tokens[*pos].type == OP_IF || tokens[*pos].type == OP_WHILE)
//         {
//             counter_of_fbracket++;
//             val->right = GetNodeIF(tokens, pos);
//         }

//         else
//             val->right = GetNodeA(tokens, pos);
        
//         if (tokens[*pos].type == UOP)
//             (*pos)++;

//         fprintf(stderr, "%d %s ", tokens[(*pos)].type, tokens[(*pos)].value.op_name);
//         if (tokens[*pos].type == FBRACKET_CLOSE)
//             counter_of_fbracket_closed++;

//         prev_val = NewOpNode((char*)COMB, nullptr, nullptr);
//         prev_val = val;
//         val = NewOpNode((char*)COMB, prev_val, nullptr);

//     } while (tokens[*pos].type != FBRACKET_CLOSE);
//     }

//     while (tokens[*pos].type == FBRACKET_CLOSE)
//         (*pos)++;
//     (*pos)--;
    
//     (tokens + if_pos)->left = new_node;
//     (tokens + if_pos)->right = prev_val;
//         fprintf(stderr, "%d %s ", (tokens + if_pos)->right->left->type, (tokens + if_pos)->right->left->value.op_name);
//     return tokens + if_pos;

// }

// node_t* MakeNode()
// {
//     node_t* new_node = (node_t*)calloc(1, sizeof(node_t));

//     if (!new_node)
//     {
//         MemoryAllocationError();
//         return nullptr;
//     }

//     new_node->left = nullptr;
//     new_node->right = nullptr;
//     new_node->prev = nullptr;

//     return new_node;
// }

// node_t* NewOpNode(char* operation, node_t* left_node, node_t* right_node)
// {
//     assert(operation != nullptr);

//     node_t* new_node = MakeNode();
//     new_node->type = OP;

//     if (!strncmp((char*)COMB, operation, MAX_LEN_OF_OPERATION))
//         new_node->type = UOP;

//     (new_node->value).op_name = operation;
//     new_node->left = left_node;
//     new_node->right = right_node;
//     return new_node;
// }

// node_t* NewNumNode(double num, node_t* left_node, node_t* right_node)
// {
//     node_t* new_node = MakeNode();

//     new_node->type = NUM;
//     (new_node->value).op_num = num;
//     new_node->left = left_node;
//     new_node->right = right_node;

//     return new_node;
// }

// node_t* NewVarNode(char* var, node_t* left_node, node_t* right_node)
// {
//     assert(var != nullptr);

//     node_t* new_node = MakeNode();

//     new_node->type = VAR;
//     (new_node->value).op_name = var;
//     new_node->left = left_node;
//     new_node->right = right_node;

//     return new_node;
// }

// node_t* FindMainFunc(node_t* node, node_t* main_node)
// {
//     assert(node != nullptr);

//     if (node->type == MAIN_FUNC)
//         main_node = node;

//     if (node->left)
//         main_node = FindMainFunc(node->left, main_node);
//     if (node->right)
//         main_node = FindMainFunc(node->right, main_node);
//     return main_node;
// }

