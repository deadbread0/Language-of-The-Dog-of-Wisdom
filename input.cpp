#include "headers/input.h"

char* PutDataFromFileToBuffer()
{
    int size = GetSizeOfInputFile(input_file_path);

    char* buffer = (char*)calloc(size + 1, sizeof(char));

    if (!buffer)
    {
        MemoryAllocationError();
        return nullptr;
    }

    FILE* filee = fopen(input_file_path, "r");
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