#include "headers/input.h"

char* PutDataFromFileToBuffer()
{
    char* input_file = (char*)"files/input_file.txt";
    int size = GetSizeOfInputFile(input_file);
    // printf("size: %d\n", size);

    char* buffer = (char*)calloc(size + 1, sizeof(char));

    if (!buffer)
    {
        printf("memory allocation error, try again\n");
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