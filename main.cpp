#include "headers/functions.h"
#include "headers/input.h"
#include "headers/dump_tree.h"
#include "headers/tree_functions.h"

int main()
{
    int pos = 0;
    char* data = PutDataFromFileToBuffer();
    FILE* output_filee = fopen("files/tree_dump.dot", "w");
    // printf("1");
    DumpTree(GetNodeG(data, &pos), output_filee);
    // printf("2");
    pos = 0;
    // printf("%d", GetG(data, &pos));
    return 0;
}