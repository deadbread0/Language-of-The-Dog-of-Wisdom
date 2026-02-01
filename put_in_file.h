#ifndef FOR_TYPES
#include "types.h"
#endif
#include <assert.h>
#include "tree_functions.h"
#ifndef PUT_IN_FILE
static const char* NILNIL = "nil nil ";
#define PUT_IN_FILE
#endif

#ifndef MAX_LEN_OP
static const int MAX_LEN_OF_OPERATION = 10;
#define MAX_LEN_OP
#endif

void PutTreeInFile(node_t* node, FILE* filee);
void PutTreeInFileBody(node_t* node, FILE* filee);
node_t* ReadNode(int* pos, char* buffer);
char* ReturnDataOfNode(int* pos, char* buffer);

