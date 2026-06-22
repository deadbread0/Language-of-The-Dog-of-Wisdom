

#include "functions.h"
#include "input.h"
#include "dump_tree.h"
#include "tree_functions.h"
#include "tree_optimisation.h"
#include "lex_analysis.h"
#include "put_in_file.h"
#include "translate_into_x86.h"

static const char* file_asm_path = "files/file_asm.asm";
static const char* tree_path = "files/tree.txt";
const int SIZE_OF_NAMETABLE = 100;