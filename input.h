#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include "functions.h"
#ifndef INPUT_FILE
static const char* input_file_path = "files/input_file.txt";
#define INPUT_FILE
#endif

char* PutDataFromFileToBuffer();
int GetSizeOfInputFile(const char* file_name);
