#include "find_match.h"
#include "common.h"
#include <ctype.h>
#include <string.h>

#define END_OF_EXP -5

char* handle_i(char* exp)
{
  char* lower_case_exp = NULL;
  int z = 0;

  if (NULL == (lower_case_exp = (char*)calloc(1, (1 + strlen(exp)) * sizeof(char)))) {
    exit(0);
  }
  for (z = 0; z < strlen(exp); z++) {
    lower_case_exp[z] = tolower(exp[z]);
  }
  lower_case_exp[z] = '\0';
  return lower_case_exp;
}

bool HasEmptyArgument(Expression* exam_segment)
{
  char* argument1 = NULL;
  char* argument2 = NULL;

  if (exam_segment->expression_type != TYPE_STRING_PAIR) {
    return false;
  }
  argument1 = strtok(exam_segment->segment, "|");
  argument2 = (exam_segment->segment + strlen(argument1) + 1);
  if (!(strncmp(argument1, EMPTY_LINE, strlen(argument1))) || !(strncmp(argument2, EMPTY_LINE, strlen(argument2)))) {
    return true;
  } else {
    return false;
  }
}

int IsSegmentMatch(Expression* part, const char* pose)
{
  char* str1 = NULL;
  char* str2 = NULL;
  char low_char;
  char high_char;

  switch (part->expression_type) {
    case TYPE_CHAR_SEQUENCE: {
      if (0 == strncmp(part->segment, pose, strlen(part->segment))) {
        return strlen(part->segment);
      } else {
        return EMPTY;
      }
    }
    case TYPE_DOT: {
      return 1;
    }
    case TYPE_STRING_PAIR: {
      str1 = strtok(part->segment, "|");
      str2 = (part->segment + strlen(str1) + 1);
      if ((0 == strncmp(str1, pose, strlen(str1)))) {
        return strlen(str1);
      } else if (0 == strncmp(str2, pose, strlen(str2))) {
        return strlen(str2);
      } else {
        return EMPTY;
      }
    }
    case TYPE_VALUE_RANGE: {
      low_char = *(part->segment);
      high_char = *(part->segment + 2);
      if ((low_char <= *pose) && (*pose <= high_char)) {
        return 1;
      } else {
        return EMPTY;
      }
    }
    case TYPE_END_OF_EXP: {
      return END_OF_EXP;
    }
    default:  // match error
      return EMPTY;
  }
}

Lines* GetMatch(const char* line, const int line_number, const int num_of_bytes)
{
  Lines* print = NULL;

  if (NULL == (print = (Lines*)malloc(sizeof(Lines)))) {
    exit(EXIT_FAILURE);
  }
  if (NULL == (print->line = (char*)malloc((EXTRA_SPACE + strlen(line)) * sizeof(char)))) {
    exit(EXIT_FAILURE);
  }
  print->num_of_bytes = num_of_bytes;
  print->num_of_line = line_number;
  print->next = NULL;
  strcpy(print->line, line);

  return print;
}

bool is_it_x(const char* line, Expression* exp_disassembled)
{

  int cnt_skip = 0;
  int position_cnt = 0;
  Expression* current_segment = NULL;
  bool is_it_x_flag = false;

  current_segment = exp_disassembled;

  while (true) {
    cnt_skip = IsSegmentMatch(current_segment, (line + position_cnt));
    if (cnt_skip == END_OF_EXP) {  // match was found
      is_it_x_flag = true;
      break;
    } else if (cnt_skip == EMPTY) {  // did not matched - it is not x
      cnt_skip = 0, position_cnt = 0;
      break;
    } else {
      position_cnt = position_cnt + cnt_skip;
      current_segment = current_segment->next;
    }
  }
  if (is_it_x_flag) {  // check if  it's only the expression
    if ((line[position_cnt] == '\0') || ((line[position_cnt] == '\n') && (line[position_cnt + 1] == '\0'))) {
      return true;
    }
  }
  return false;
}

bool CheckMatchInLine(Expression* current_exp, int pose_cnt, int* cnt_nextstep, Expression* expression_disassembled,
                         Input* input, char* exam_line, int i)
{
  bool match_in_line_flag = false;

  while (!match_in_line_flag) {
    *cnt_nextstep = IsSegmentMatch(current_exp, (exam_line + i + pose_cnt));
    if (*cnt_nextstep == END_OF_EXP) {  // match was found
      if (!(input->v)) {
        return true;
      } else {
        return false;
      }
    } else if (*cnt_nextstep == EMPTY) {
      if ((input->v) && (i == strlen(exam_line))) {
        return true;
      } else if (HasEmptyArgument(current_exp)) {
        pose_cnt = pose_cnt + *cnt_nextstep;
        current_exp = current_exp->next;
      } else {
        *cnt_nextstep = 0, pose_cnt = 0;
        current_exp = expression_disassembled->next;
        return false;
      }
    } else {  // keep checking for match
      pose_cnt = pose_cnt + *cnt_nextstep;
      current_exp = current_exp->next;
    }
  }
}

Lines* is_match_in_line(Input* input, Expression* expression_disassembled, char* line, int line_number, Lines* lines,
                        int num_of_bytes)
{
  int pose_cnt = 0;
  int cnt_nextstep = 0;
  char* exam_line = NULL;
  Lines* print_set = NULL;
  bool match_in_line_flag = false;
  Expression* current_exp = NULL;

  current_exp = expression_disassembled;
  if (strlen(line) < strlen(input->exp) && false == input->E) {
    if (true == input->v) {
      return GetMatch(line, line_number, num_of_bytes);
    } else {
      return NULL;
    }
  }
  if (input->i) {
    exam_line = handle_i(line);
  } else {
    exam_line = line;
  }
  if (input->x && input->v) {
    match_in_line_flag = !(is_it_x(exam_line, current_exp));
  } else if (input->x) {
    match_in_line_flag = is_it_x(exam_line, current_exp);
  } else {
    int i = 0;
    for (i = 0; i <= strlen(exam_line); i++) {
      current_exp = expression_disassembled;
      match_in_line_flag =
          CheckMatchInLine(current_exp, pose_cnt, &cnt_nextstep, expression_disassembled, input, exam_line, i);
      if (match_in_line_flag) {
        break;
      } else if ((input->v) && (cnt_nextstep == END_OF_EXP) && !(match_in_line_flag)) {
        break;
      }
    }
  }
  if ((input->i) && (exam_line != NULL)) {
    free(exam_line);
  }

  if (match_in_line_flag) {
    return GetMatch(line, line_number, num_of_bytes);
  } else {
    return NULL;
  }
}
