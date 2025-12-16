#include "C:\Users\kirak\c+-\headers\functions.h"
#include "C:\Users\kirak\c+-\headers\input.h"
#include "C:\Users\kirak\c+-\headers\dump_tree.h"
#include "C:\Users\kirak\c+-\headers\tree_functions.h"
#include "C:\Users\kirak\c+-\headers\tree_optimisation.h"
#include "C:\Users\kirak\c+-\headers\lex_analysis.h"
#include "C:\Users\kirak\c+-\headers\put_in_file.h"
#include "C:\Users\kirak\c+-\headers\translate_into_asm.h"

int main()
{
    int pos = 0;
    char* data = PutDataFromFileToBuffer();
    int size = GetSizeOfInputFile(input_file_path);
    node_t* new_tokens = (node_t*)calloc(size, sizeof(node_t));
    
    LexAnalysis(data, &pos, new_tokens);

    FILE* output_filee = fopen(tree_dump_path, "w");
    pos = 0;
    node_t* tree = GetNodeG(new_tokens, &pos);
    DumpTree(tree, output_filee);
    fclose(output_filee);

    FILE* filee_for_tree = fopen(tree_path, "w");
    PutTreeInFile(tree, filee_for_tree);
    free(tree);
    fclose(filee_for_tree);
    
    return 0;
}