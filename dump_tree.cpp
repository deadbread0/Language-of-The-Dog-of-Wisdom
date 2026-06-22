#include "headers/dump_tree.h"

void DumpTree(node_t* node, FILE* filee)
{
    assert(node != nullptr);
    assert(filee != nullptr);

    fprintf(filee, "digraph {\nrankdir=HR;\n");
    int n = 0;
    DumpGraphNode(node, filee, &n);
    MakeCommunicationBetweenNodes(node, filee);
    fprintf(filee, "}\n");
}

void DumpGraphNode(node_t* node, FILE* filee, int* n)
{
    assert(node != nullptr);
    assert(filee != nullptr);

    if (node->left)
    {
        DumpGraphNode(node->left, filee, n);
    }
    if (node->right)
    {
        DumpGraphNode(node->right, filee, n);
    }

    switch (node->type)
    {
        case VAR: PrintNodeInFile(node, filee, VAR_COLOR, VAR_LINE_COLOR); break;
        case NUM: PrintNodeInFile(node, filee, NUM_COLOR, NUM_LINE_COLOR); break;
        case UOP: PrintNodeInFile(node, filee, UOP_COLOR, UOP_LINE_COLOR); break;
        default: PrintNodeInFile(node, filee, DEF_COLOR, DEF_LINE_COLOR); break;
    }
}

void MakeCommunicationBetweenNodes(node_t* node, FILE* filee)
{
    assert(node != nullptr);
    assert(filee != nullptr);

    if (node->left)
    {
        fprintf(filee, "%u: <f0> -> %u: <f2> ", node, node->left);
        MakeCommunicationBetweenNodes(node->left, filee);
    }

    if (node->right)
    {
        fprintf(filee, "%u: <f1> -> %u: <f2> ", node, node->right);
        MakeCommunicationBetweenNodes(node->right, filee);
    }
}

void PrintNodeInFile(node_t* node, FILE* filee, const char* fillcolor, const char* color)
{
    if (node->type == NUM)
        fprintf(filee, "%u [shape=record; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{<f2>num|%lg}|{<f0>left \\n %x}|{<f1>right\\n %x}|{prev\\n %x}|{%x}\"];\n", node, fillcolor, color, node->value.op_num, node->left, node->right, node->prev, node);

    else if (node->type == VAR)
        fprintf(filee, "%u [shape=record; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{<f2>var|%s}|{<f0>left \\n %x}|{<f1>right\\n %x}|{prev\\n %x}|{%x}\"];\n", node, fillcolor, color, node->value.op_name, node->left, node->right, node->prev, node);
    
    else
        fprintf(filee, "%u [shape=record; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{<f2>op|%s}|{<f0>left \\n %x}|{<f1>right\\n %x}|{prev\\n %x}|{%x}\"];\n", node, fillcolor, color, node->value.op_name, node->left, node->right, node->prev, node);


}