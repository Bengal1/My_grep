#ifndef FIND_MATCH_H
#define FIND_MATCH_H

#include "input_format.h"

char* handle_i(char* exp);

Lines* IsMatchInLine(Input* input, Expression* expression_disassembled, char* line, int line_number, Lines* lines, int num_of_bytes);

#endif
