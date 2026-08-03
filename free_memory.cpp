#include "headers/free_memory.h"

void FreeMem(void* memory)
{
    free(memory);
}