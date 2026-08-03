#include "../headers/types.h"
#include "../headers/input.h"
#include "../headers/tree_optimisation.h"
#include "../headers/translate_into_x86.h"
#include "../headers/put_in_file.h"
#include "../headers/dump_tree.h"
#ifndef FOR_FUNC
#include "functions.h"
#endif
#include "../headers/lex_analysis.h"
#include "../headers/tree_functions.h"
#include "free_memory.h"

static const char* tree_path = "files/tree.txt";
static const char* new_tree_dump_path = "files/new_tree_dump.dot";