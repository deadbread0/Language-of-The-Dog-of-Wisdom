#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include "functions.h"
#ifndef FOR_CONST
#include "const.h"
#endif

char* PutDataFromFileToBuffer();
int GetSizeOfInputFile(const char* file_name);
