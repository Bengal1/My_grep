#ifndef INPUT_FORMAT_H
#define INPUT_FORMAT_H

#include <stdbool.h>
#include <stdio.h>

enum exprssion_type { TYPE_CHAR_SEQUENCE = 1, TYPE_DOT, TYPE_STRING_PAIR, TYPE_VALUE_RANGE, TYPE_END_OF_EXP };

typedef struct input {
  int A;
  bool b;
  bool c;
  bool i;
  bool n;
  bool v;
  bool x;
  bool E;
  FILE* file_name;
  char* exp;
} Input;

typedef struct lines {
  char* line;
  int num_of_bytes;
  int num_of_line;
  struct lines* next;
} Lines;

typedef struct expression {
  char* segment;
  int expression_type;
  struct expression* next;
} Expression;

Input* ParseArguments(int argc, char* argv[]);

Lines* ReadLinesFromStream(Input* input);

Expression* ExpressionAnalyze(Input* input);

void DeleteLines(Lines* target);

void DeleteInput(Input* target);

void DeleteExpression(Expression* target);

#endif
