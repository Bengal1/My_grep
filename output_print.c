#include "output_print.h"
#include "common.h"
#include <string.h>

#define DELIMITER_1ST ":"
#define DELIMITER_2ND "-"

bool CheckIfLineInPrintList(const int num_of_line, Lines* print_list)
{
  Lines* current_line = print_list->next;

  while (NULL != current_line) {
    if (current_line->num_of_line == num_of_line) {
      return true;
    } else {
      current_line = current_line->next;
    }
  }
  return false;
}

int CountNumberOfMatches(Lines* PrintInfo)
{
  Lines* current_print = PrintInfo;
  int counter = 0;

  while (NULL != current_print) {
    counter = counter + 1;
    current_print = current_print->next;
  }
  return counter;
}

char* AssignMemoryToLine(int line_number, int num_of_bytes, char* app_line)
{
  int length;
  char* line;

  length = snprintf(NULL, 0, "%d", num_of_bytes) + snprintf(NULL, 0, "%d", line_number);
  if (NULL != app_line) {
    length = length + 1 + strlen(app_line);
  }
  if (NULL == (line = (char*)calloc(1, (EXTRA_SPACE + length) * sizeof(char)))) {
    exit(EXIT_FAILURE);
  }
  return line;
}

char* ItoA(int number)
{
  int length;
  char* line;

  length = snprintf(NULL, 0, "%d", number);
  if (NULL == (line = (char*)malloc((2 + length) * sizeof(char)))) {
    exit(0);
  }
  snprintf(line, 1 + length, "%d", number);

  return line;
}

void CreateLine(char* line, int number_from_line, char* delimiter)
{
  char* new_line;

  new_line = ItoA(number_from_line);
  strcpy(line, new_line);
  strcat(line, delimiter);
  free(new_line);

  return;
}

Lines* PrintRestOfLines(const Input* input, Lines* stream_bookmark, Lines* current_print, Lines* PrintInfo)
{
  int counter = input->A;
  char *delimiter = NULL, *line_to_print = NULL;
  Lines* current_line = stream_bookmark;

  while (0 != counter && NULL != current_line && NULL != current_print) {
    line_to_print = AssignMemoryToLine(current_line->num_of_line, current_line->num_of_bytes, current_line->line);
    if (CheckIfLineInPrintList(current_line->num_of_line, PrintInfo->next)) {
      delimiter = DELIMITER_1ST;
      counter = input->A;
      current_print = current_print->next;
    } else {
      delimiter = DELIMITER_2ND;
      counter = counter - 1;
    }
    if (input->n) {
      CreateLine(line_to_print, current_line->num_of_line, delimiter);
    }
    if (input->b) {
      CreateLine(line_to_print, current_line->num_of_bytes, delimiter);
    }
    if (0 == strcmp(line_to_print, EMPTY_LINE)) {
      strcpy(line_to_print, current_line->line);
    } else {
      strcat(line_to_print, current_line->line);
    }
    printf("%s", line_to_print);
    current_line = current_line->next;
    free(line_to_print);
  }
  return current_print;
}

void PrintRoutine(Lines* PrintInfo, Input* input, Lines* lines)
{
  char *line = NULL, *line_number = NULL, *line_bytes = NULL, *delimiter = NULL;
  Lines* current_print = NULL;
  Lines* current_line_in_file = lines->next;

  current_print = PrintInfo->next;
  if (input->c) {
    printf("%d\n", CountNumberOfMatches(PrintInfo->next));
    return;
  }
  while (current_print != NULL) {
    line = AssignMemoryToLine(current_print->num_of_line, current_print->num_of_bytes, current_print->line);
    delimiter = DELIMITER_1ST;
    if (input->n) {
      CreateLine(line, current_print->num_of_line, delimiter);
    }
    if (input->b) {
      line_bytes = ItoA(current_print->num_of_bytes);
      if (input->n) {
        strcat(line, line_bytes);
      } else {
        strcpy(line, line_bytes);
      }
      strcat(line, delimiter);
      free(line_bytes);
    }
    if (0 == strcmp(line, EMPTY_LINE)) {
      strcpy(line, current_print->line);
    } else {
      strcat(line, current_print->line);
    }
    printf("%s", line);  // done with first line
    while (((current_line_in_file->num_of_line) != (current_print->num_of_line + 1))) {
      if (NULL != current_line_in_file->next) {
        current_line_in_file = current_line_in_file->next;
      } else {
        current_line_in_file = current_line_in_file->next;
        break;
      }
    }
    free(line);
    current_print = PrintRestOfLines(input, current_line_in_file, current_print, PrintInfo);
    current_print = current_print->next;
    if ((EMPTY != input->A) && (current_print != NULL)) {
      printf("--\n");
    }
  }
  return;
}
