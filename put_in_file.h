#ifndef FOR_TYPES
#include "types.h"
#endif
#include <assert.h>
#include "tree_functions.h"
void PutTreeInFile(node_t* node, FILE* filee);
node_t* ReadNode(int* pos, char* buffer);
char* ReturnDataOfNode(int* pos, char* buffer);

