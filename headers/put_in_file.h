#ifndef FOR_TYPES
#include "types.h"
#endif

#include <assert.h>

#ifndef TREE_F
#include "tree_functions.h"
#endif

#ifndef PUT_IN_FILE
static const char* NILNIL = "nil nil ";
#define PUT_IN_FILE
#endif

void PutTreeInFile(node_t* node, FILE* filee);
void PutTreeInFileBody(node_t* node, FILE* filee);
node_t* ReadNode(int* pos, char* buffer);
char* ReturnDataOfNode(int* pos, char* buffer);

