#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

extern char *strdup(const char *);

typedef union
{
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool (*check_function)(char *);
typedef answer_t (*convert_function)(char *);

char ask_question_menu(char *question);
char *ask_question_string(char *question);
double ask_question_float(char *question);
int ask_question_int(char *question);
char *ask_question_shelf(char *question);
answer_t ask_question(char *question, check_function check, convert_function convert);
bool not_empty(char *str);
answer_t make_float(char *str);
int read_string(char *buf, int buf_siz);
bool is_float(char *str);
bool is_number(char *str);
void print(char *s);
void println(char *s);

#endif