
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

    names_t arr_of_names[SIZE_OF_NAMETABLE] = {};
    // int nametable_size = 0;
    // names_t nametable = {arr_of_names, nametable_size};

    int last_index_in_nametable = 0;
    // FillNametable(arr_of_names, new_tree, &last_index_in_nametable);/////ну короче нельзя локальные переменные задавать, потому что я так решила
    // nametable.size = last_index_in_nametable;

    TranslateIntoAsm(new_tree, file_asm);
    fclose(file_asm);
    return 0;
}