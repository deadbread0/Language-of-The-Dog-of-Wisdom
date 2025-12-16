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
    int new_size = GetSizeOfInputFile(tree_path);
    char* buf = PutTreeFromFileToBuffer(new_size);
    int pos = 0;
    node_t* new_tree = ReadNode(&pos, buf);
    new_tree = FillTypesInTree(new_tree);
    FILE* file_asm = fopen(file_asm_path, "w");
    names_t nametable[100] = {};//
    TranslateIntoAsm(new_tree, file_asm, nametable);
    fclose(file_asm);
    for (int i = 0; i < nametable->last_i; i++)
    {
        printf("%d - %s\n", (nametable + i)->num, (nametable + i)->var);
    }
    return 0;
}