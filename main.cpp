#include "headers/functions.h"
#include "headers/input.h"
#include "headers/dump_tree.h"
#include "headers/tree_functions.h"
#include "headers/tree_optimisation.h"

int main()
{
    int pos = 0;
    char* data = PutDataFromFileToBuffer();
    FILE* output_filee = fopen("files/tree_dump.dot", "w");
    DumpTree(ConstantOptimization(GetNodeG(data, &pos)), output_filee);
    
    pos = 0;
    // printf("%d", GetAll(data, &pos));
    return 0;
}