#include "C:\Users\kirak\c+-\headers\functions.h"
#include "C:\Users\kirak\c+-\headers\input.h"
#include "C:\Users\kirak\c+-\headers\dump_tree.h"
#include "C:\Users\kirak\c+-\headers\tree_functions.h"
#include "C:\Users\kirak\c+-\headers\tree_optimisation.h"
#include "C:\Users\kirak\c+-\headers\lex_analysis.h"
#include "C:\Users\kirak\c+-\headers\put_in_file.h"
#include "C:\Users\kirak\c+-\headers\translate_into_asm.h"
#ifndef FRONTEND
static const char* tree_dump_path = "files/tree_dump.dot";
static const char* tree_path = "files/tree.txt";
#define FRONTEND
#endif
