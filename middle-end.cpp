#include "C:\Users\kirak\c+-\headers\types.h"
#include "C:\Users\kirak\c+-\headers\input.h"
#include "C:\Users\kirak\c+-\headers\tree_optimisation.h"
#include "C:\Users\kirak\c+-\headers\translate_into_asm.h"
#include "C:\Users\kirak\c+-\headers\put_in_file.h"
#include "C:\Users\kirak\c+-\headers\dump_tree.h"

int main()
{
    int new_size = GetSizeOfInputFile(tree_path);
    char* buf = PutTreeFromFileToBuffer(new_size);
    int pos = 0;
    node_t* new_tree = ReadNode(&pos, buf);//чзх
    new_tree = ConstantOptimization(FillTypesInTree(new_tree));
    FILE* new_output_file = fopen(new_tree_dump_path, "w");
    DumpTree(new_tree, new_output_file);
    fclose(new_output_file);

    FILE* filee_for_tree = fopen(tree_path, "w");
    PutTreeInFile(new_tree, filee_for_tree);
    free(new_tree);
    fclose(filee_for_tree);
    return 0;
}