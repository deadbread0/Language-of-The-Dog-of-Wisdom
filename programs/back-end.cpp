
#include "../headers/backend.h"

int main()
{

    int new_size = GetSizeOfInputFile(tree_path);
    char* buf = PutTreeFromFileToBuffer(new_size);
    int pos = 0;

    node_t* new_tree = ReadNode(&pos, buf);
    new_tree = FillTypesInTree(new_tree);

    FILE* filee = fopen("files/endtree.dot", "w");
    DumpTree(new_tree, filee);

    FILE* file_asm = fopen(file_asm_path, "w");

    // names_t arr_of_names[SIZE_OF_NAMETABLE] = {};
    // int last_index_in_nametable = 0;

    TranslateIntoAsm(new_tree, file_asm);
    fclose(file_asm);

    FreeMem(buf);
    DestroyTree(new_tree);
    return 0;
}