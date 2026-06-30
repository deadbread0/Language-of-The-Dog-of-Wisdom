#include "headers/input.h"

char* PutDataFromFileToBuffer(const char* input_file)
{
    int size = GetSizeOfInputFile(input_file);

    char* buffer = (char*)calloc(size + 1, sizeof(char));

    if (!buffer)
    {
        MemoryAllocationError();
        return nullptr;
    }

    FILE* filee = fopen(input_file, "r");
    fread(buffer, sizeof(char), size + 1, filee);

    if (!buffer)
    {
        printf("problems with reading file, try again\n");
        return nullptr;
    }

    fclose(filee);
    return buffer;
}

int GetSizeOfInputFile(const char* file_name)
{
    assert(file_name != nullptr);

    struct stat StructStat;
    stat(file_name, &StructStat);
    return StructStat.st_size;
}