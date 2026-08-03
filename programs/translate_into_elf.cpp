#ifndef ASM
#define ASM
#endif

#include "../headers/translate_into_elf.h"

int main()
{
    FILE* file_elf = fopen("files/file_elf.c", "w");

    int objcode_size = 0;
    loops_t loops_struct = {};
    int* objcode = TranslatefObjcode(file_elf, &objcode_size, &loops_struct);
    FillJMPs(objcode, &loops_struct);

    FprintfCodeBeforeObjcode(file_elf, objcode_size);
    FprintfObjcode(file_elf, objcode, objcode_size);
    FprintfCodeAfterObjcode(file_elf);  

    FreeMem(objcode);
    fclose(file_elf);
    return 0;
}
