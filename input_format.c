#include "input_format.h"
#include "common.h"
#include "find_match.h"
#include <string.h>

Input* InitInput()
{
  Input* input;

  if (NULL == (input = (Input*)malloc(sizeof(Input)))) {
    return NULL;
  }
  input->A = 0;
  input->E = false;
  input->b = false;
  input->c = false;
  input->i = false;
  input->n = false;
  input->v = false;
  input->x = false;
  input->file_name = NULL;
  input->exp = NULL;
  return input;
}

void GetStream(Input* input, char* argv[], int argc, int j)
{
  if (NULL == input->exp) {
    if (NULL == (input->exp = (char*)calloc(1, (1 + strlen(argv[argc - 1])) * sizeof(char)))) {
      exit(EXIT_FAILURE);
    }
    strcpy(input->exp, argv[argc - 1]);
    input->file_name = stdin;
    return;
  } else {
    if (NULL == (input->file_name = fopen(argv[j], "r"))) {
      exit(EXIT_FAILURE);
    }
    return;
  }
}

Input* ParseArguments(int argc, char* argv[])
{
  Input* input = NULL;

  if (NULL == (input = InitInput())) {
    exit(EXIT_FAILURE);
  }
  int j = 1;
  for (j = 1; j < argc; j++) {
    if (0 == strcmp(argv[j], "-b")) {
      input->b = true;
    } else if (0 == strcmp(argv[j], "-c")) {
      input->c = true;
    } else if (0 == strcmp(argv[j], "-i")) {
      input->i = true;
    } else if (0 == strcmp(argv[j], "-n")) {
      input->n = true;
    } else if (0 == strcmp(argv[j], "-v")) {
      input->v = true;
    } else if (0 == strcmp(argv[j], "-x")) {
      input->x = true;
    } else if (0 == strcmp(argv[j], "-E")) {
      j++;
      input->E = true;
      if (NULL == (input->exp = (char*)malloc((1 + strlen(argv[j])) * sizeof(char)))) {
        return NULL;
      }
      strcpy(input->exp, argv[j]);
      *(input->exp + strlen(input->exp)) = '\0';
    } else if (0 == strcmp(argv[j], "-A")) {
      j++;
      input->A = atoi(argv[j]);
    } else if (j == argc - 1) {
      GetStream(input, argv, argc, j);
    } else {
      if (NULL == (input->exp = (char*)calloc(1, (1 + strlen(argv[j])) * sizeof(char)))) {
        return NULL;
      }
      strcpy(input->exp, argv[j]);
      if (input->exp[strlen(input->exp) - 1] == '\n') {
        input->exp[strlen(input->exp) - 1] = '\0';
      }
    }
  }
  return input;
}

Lines* ReadLinesFromStream(Input* input)
{
  Lines* lines;
  Lines* current_line;
  Lines* new_line;
  char* line = NULL;
  size_t len = EMPTY;

  if (NULL == (lines = (Lines*)malloc(sizeof(Lines)))) {
    exit(EXIT_FAILURE);
  }
  lines->line = NULL;
  lines->next = NULL;
  lines->num_of_line = 0;
  lines->num_of_bytes = 0;
  current_line = lines;

  while ((NULL != input->file_name) && (getline(&line, &len, input->file_name) != EOF)) {
    if (EMPTY_LINE == line || NULL == line) {
      break;
    }
    if (NULL == (new_line = (Lines*)malloc(sizeof(Lines)))) {
      return NULL;
    }
    if (NULL == (new_line->line = (char*)malloc((EXTRA_SPACE + strlen(line)) * sizeof(char)))) {
      return NULL;
    }
    strcpy(new_line->line, line);
    new_line->next = NULL;
    new_line->num_of_line = current_line->num_of_line + 1;
    if (current_line != lines) {
      new_line->num_of_bytes = current_line->num_of_bytes + strlen(current_line->line);
    } else {
      new_line->num_of_bytes = current_line->num_of_bytes;
    }
    current_line->next = new_line;
    current_line = new_line;
  }
  if (input->file_name != EMPTY) {
    fclose(input->file_name);
  }
  free(line);

  return lines;
}

void UpdateSegment(Expression* current_segment, const char* position, const int cnt, const int exp_type)
{
  strncpy(current_segment->segment, position, cnt);
  *(current_segment->segment + cnt) = '\0';
  current_segment->expression_type = exp_type;

  return;
}

int GetSequence(const char* position, Expression* current_segment)
{

  int cnt = 0;
  int real_cnt = 0;

  while (true) {
    if (*(position + real_cnt) == '\\') {
      real_cnt += 2;
      cnt++;
    } else if ((*(position + real_cnt) == '.') || (*(position + real_cnt) == '[') || (*(position + real_cnt) == '(') ||
               (*(position + real_cnt) == '\0')) {  // end sequence
      break;
    } else {  // keep sequence
      cnt++, real_cnt++;
    }
  }
  if (NULL == (current_segment->segment = (char*)malloc((2 + cnt) * sizeof(char)))) {
    exit(EXIT_FAILURE);
  }
  cnt = 0;
  int i = 0;
  for (i = 0; i < real_cnt; i++) {
    if (*(position + i) == '\\') {
      cnt++, i++;
      *(current_segment->segment + i - cnt) = *(position + i);
    } else {
      *(current_segment->segment + i - cnt) = *(position + i);
    }
  }
  *(current_segment->segment + real_cnt - cnt) = '\0';
  current_segment->expression_type = TYPE_CHAR_SEQUENCE;

  return real_cnt;
}

int SegmentAnalyze(Expression* segment, const char* position)
{
  int cnt = 0;
  int real_cnt = 0;
  Expression* current_segment = NULL;

  current_segment = segment;

  if (*position == '(') {  // (str1|str2)
    while (*(position + cnt) != ')') {
      cnt++;
    }
    if (NULL == (current_segment->segment = (char*)malloc((2 + cnt) * sizeof(char)))) {
      exit(EXIT_FAILURE);
    }
    cnt--;
    position++;
    UpdateSegment(current_segment, position, cnt, TYPE_STRING_PAIR);
    real_cnt = cnt + 2;
  } else if (*position == '[') {  // [x-y]
    position++;
    while (*(position + cnt) != ']') {
      cnt++;
    }
    if (NULL == (current_segment->segment = (char*)malloc((2 + cnt) * sizeof(char)))) {
      exit(EXIT_FAILURE);
    }
    UpdateSegment(current_segment, position, cnt, TYPE_VALUE_RANGE);
    real_cnt = cnt + 2;
  } else if (*position == '.') {  // any character
    if (NULL == (current_segment->segment = (char*)malloc(sizeof(char) * 2))) {
      exit(EXIT_FAILURE);
    }
    strcpy(current_segment->segment, ".");
    current_segment->expression_type = TYPE_DOT;
    real_cnt = 1;
    current_segment->next = NULL;
  } else if (*position == '\0')  // end of expression
  {
    if (NULL == (current_segment->segment = (char*)malloc(sizeof(char) * 2))) {
      exit(EXIT_FAILURE);
    }
    *(current_segment->segment) = '\0';
    current_segment->expression_type = TYPE_END_OF_EXP;
    current_segment->next = NULL;
    real_cnt = EMPTY;
  } else {  // characters sequence
    real_cnt = GetSequence(position, current_segment);
  }
  return real_cnt;
}

Expression* ExpressionAnalyze(Input* input)
{

  int cnt_nextstep = 0;
  int bookmark = 0;
  Expression* exp_head = NULL;
  Expression* current_exp = NULL;
  char* expression = NULL;

  if (true == input->i)  // i switch
  {
    expression = handle_i(input->exp);
  } else {
    bookmark = strlen(input->exp);
    if (NULL == (expression = (char*)malloc(2 + bookmark * sizeof(char)))) {
      exit(EXIT_FAILURE);
    }
    strcpy(expression, input->exp);
    bookmark = 0;
  }
  if (NULL == (exp_head = (Expression*)malloc(sizeof(Expression)))) {
    exit(EXIT_FAILURE);
  }
  current_exp = exp_head;
  while (true) {
    cnt_nextstep = SegmentAnalyze(current_exp, expression + bookmark);
    if (cnt_nextstep == EMPTY) {  // end of expression
      break;
    } else {  // prepare data for next expression segment
      bookmark = bookmark + cnt_nextstep;
      if (NULL == (current_exp->next = (Expression*)malloc(sizeof(Expression)))) {
        exit(EXIT_FAILURE);
      }
      current_exp = current_exp->next;
    }
  }
  if (NULL != expression) {
    free(expression);
  }
  return exp_head;
}

void DeleteLines(Lines* target)
{
  Lines* current = NULL;
  Lines* previous = NULL;

  if (target == NULL) {
    return;
  } else {
    previous = target;
    while (NULL != previous) {
      current = previous->next;
      if (NULL != previous->line) {
        free(previous->line);
      }
      if (NULL != previous) {
        free(previous);
      }
      previous = current;
    }
    return;
  }
}

void DeleteInput(Input* target)
{
  if (NULL == target) {
    return;
  } else {
    if (NULL != target->exp) {
      free(target->exp);
    }
    free(target);
    return;
  }
}

void DeleteExpression(Expression* target)
{

  Expression* current = NULL;
  Expression* next = NULL;

  if (target != NULL) {
    current = target;

    while (NULL != current) {
      next = current->next;
      if (NULL != (current->segment)) {
        free(current->segment);
      }
      free(current);
      current = next;
    }
  }
  return;
}
