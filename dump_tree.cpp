#include "headers/dump_tree.h"

void DumpTree(node_t* node, FILE* filee)
{
    assert(node != nullptr);
    assert(filee != nullptr);

    fprintf(filee, "digraph {\nrankdir=HR;\n");
    DumpGraphNode(node, filee);
    // printf("2");
    MakeCommunicationBetweenNodes(node, filee);
    fprintf(filee, "}\n");

}

void DumpGraphNode(node_t* node, FILE* filee)
{
    assert(node != nullptr);
    assert(filee != nullptr);

    if (node->left)
        DumpGraphNode(node->left, filee);
    if (node->right)
        DumpGraphNode(node->right, filee);

    switch (node->type)
    {
        case OP: fprintf(filee, "%u [shape=record; style = filled; fillcolor = \"#c0f2f2ff\"; color = \"#4682B4\"; label = \"{<f2>op|%s}|{<f0>left \\n %x}|{<f1>right\\n %x}|{prev\\n %x}|{%x}\"];\n", node, (node->value).op_name, node->left, node->right, node->prev, node); break;
        case VAR: fprintf(filee, "%u [shape=record; style = filled; fillcolor = \"#c6c0f2ff\"; color = \"#6746b4ff\"; label = \"{<f2>var|%s}|{<f0>left \\n %x}|{<f1>right\\n %x}|{prev\\n %x}|{%x}\"];\n", node, (node->value).op_name, node->left, node->right, node->prev, node); break;
        case NUM: fprintf(filee, "%u [shape=record; style = filled; fillcolor = \"#e5f2c0ff\"; color = \"#a9b446ff\"; label = \"{<f2>num|%lg}|{<f0>left \\n %x}|{<f1>right\\n %x}|{prev\\n %x}|{%x}\"];\n", node, (node->value).op_num, node->left, node->right, node->prev, node); break;
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