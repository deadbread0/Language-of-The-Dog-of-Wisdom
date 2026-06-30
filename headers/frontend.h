#include "../headers/input.h"
#ifndef FOR_FUNC
#include "../headers/functions.h"
#endif
#include "../headers/dump_tree.h"
#include "../headers/tree_functions.h"
#include "../headers/tree_optimisation.h"
#include "../headers/lex_analysis.h"
#include "../headers/put_in_file.h"
#include "../headers/translate_into_x86.h"
#ifndef FRONTEND
static const char* tree_dump_path = "files/tree_dump.dot";
static const char* tree_path = "files/tree.txt";
#define FRONTEND
#endif
