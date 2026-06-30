
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
