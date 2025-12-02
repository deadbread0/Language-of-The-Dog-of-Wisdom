#ifndef FOR_TYPES
#include "types.h"
#endif
#include <assert.h>

void DumpTree(node_t* node, FILE* filee);
void DumpGraphNode(node_t* node, FILE* filee);
void MakeCommunicationBetweenNodes(node_t* node, FILE* filee);
