#include "../headers/frontend.h"
//warning если исп только один main, к нему надо объявление добавить 
int main()
{
    int pos = 0;
    char* data = PutDataFromFileToBuffer(input_file_path);
    int size = GetSizeOfInputFile(input_file_path);
    node_t* new_tokens = (node_t*)calloc(size, sizeof(node_t));
    
    LexAnalysis(data, &pos, new_tokens);

    FILE* output_filee = fopen(tree_dump_path, "w");
    pos = 0;
    
    node_t* tree = GetNodeComb(new_tokens, &pos);
    FreeMem(new_tokens);

    DumpTree(tree, output_filee);
    fclose(output_filee);

    FILE* filee_for_tree = fopen(tree_path, "w");
    PutTreeInFile(tree, filee_for_tree);

    fclose(filee_for_tree);
    
    return 0;
}