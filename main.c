#include "common.h"
#include "find_match.h"
#include "input_format.h"
#include "output_print.h"

void GracefullTermination(Lines* PrintInfo, Input* CommandInfo, Lines* lines, Expression* expression_disassembled)
{
  DeleteExpression(expression_disassembled);
  DeleteLines(PrintInfo);
  DeleteLines(lines);
  DeleteInput(CommandInfo);

  return;
}

int main(int argc, char* argv[])
{
  Input* input = NULL;
  Expression* expression_disassembled = NULL;
  Lines* lines = NULL;
  Lines* current_line = NULL;
  Lines* PrintInfo = NULL;
  Lines* current_match = NULL;

  if (NULL == (input = ParseArguments(argc, argv))) {
    return EXIT_FAILURE;
  }
  if (NULL == (lines = ReadLinesFromStream(input))) {
    GracefullTermination(EMPTY, input, EMPTY, EMPTY);
    return EXIT_FAILURE;
  }

  current_line = lines->next;
  if (NULL == (expression_disassembled = ExpressionAnalyze(input))) {
    GracefullTermination(EMPTY, input, lines, EMPTY);
    return EXIT_FAILURE;
  }
  if (NULL == (PrintInfo = (Lines*)malloc(sizeof(Lines)))) {
    GracefullTermination(EMPTY, input, lines, expression_disassembled);
    return EXIT_FAILURE;
  }
  PrintInfo->next = NULL;
  PrintInfo->line = NULL;
  PrintInfo->num_of_line = 0;
  PrintInfo->num_of_bytes = 0;
  current_match = PrintInfo;
  current_line = lines->next;

  while (NULL != current_line) {
    if (NULL != (current_match->next = is_match_in_line(input, expression_disassembled, current_line->line,
                                                current_line->num_of_line, lines, current_line->num_of_bytes))) {
      current_match = current_match->next;
    }
    current_line = current_line->next;
  }

  PrintRoutine(PrintInfo, input, lines);
  GracefullTermination(PrintInfo, input, lines, expression_disassembled);

  return EXIT_SUCCESS;
}
