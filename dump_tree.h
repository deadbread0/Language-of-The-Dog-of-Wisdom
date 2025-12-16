#ifndef FOR_TYPES
#include "types.h"
#endif
#ifndef FOR_CONST
#include "const.h"
#endif
#include <assert.h>

void DumpTree(node_t* node, FILE* filee);
void DumpGraphNode(node_t* node, FILE* filee, int* n);
void MakeCommunicationBetweenNodes(node_t* node, FILE* filee);
