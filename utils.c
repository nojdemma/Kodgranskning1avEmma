#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"

extern char *strdup(const char *);

typedef bool (*check_function)(char *);
typedef answer_t (*convert_function)(char *);

answer_t make_float(char *str)
{
  answer_t a;                // skapa ett oinitierat answer_t-värde
  a.float_value = atof(str); // gör det till en float, via atof
  return a;                  // returnera värdet
}

int read_string(char *buf, int buf_siz)
{
  int counter = 0;

  if (buf_siz <= 0)
  {
    return 0;
  }
  else
  {
    for (int i = 0; i < buf_siz - 1; i++)
    {
      char c = getchar();
      if (c == '\n' || c == EOF)
      {
        buf[i] = '\0';
        break;
      }
      buf[i] = c;
      counter++;
    }
    return counter;
  }
}

bool is_number(char *str)
{
  if (!isdigit(str[0]) && str[0] != 45)
  {
    return false;
  }

  for (int i = 1; i < strlen(str); i++)
  {
    if (!isdigit(str[i]))
    {
      return false;
    }
  }
  return true;
}

bool is_float(char *str)
{
  int i = 0;
  bool has_dot = false;
  while (str[i] != '\0')
  {
    if (str[i] == '.')
    {
      if (has_dot)
      {
        return false;
      }
      has_dot = true;
    }
    else if (!isdigit(str[i]))
    {
      return false;
    }
    i++;
  }
  return true;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

bool is_shelf(char *str)
{
  int length =strlen(str);
  bool ver1 = false;
  bool ver2 = true;
  bool ver = true;
  
  //Kolla om bokstav först
  char *alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  for (int i = 0; i < strlen(alf); i += 1)
  {
    if(str[0] == alf[i])
    {
      ver1 = true;
    }
  }
  for(int i=1; i<length; i = i+1)
  {
    bool is_number = isdigit(str[i]);
      if(is_number==0)
      { 
        ver2 = false;
      }
  }
  if(ver1 == 0 || ver2 == 0)
  {
    ver = false;
  }
  return ver;
}

bool is_valid_meny(char *str)
{
  char *valid = "AaLlDdEeSsPpCcRr+-=OoQq";
  int length = strlen(valid);
  if (strlen(str) == 1)
  {
    for (int x = 0; x < length; x++)
    {
      if (str[0] == valid[x])
      {
        return true;
      }
    }
  }
  return false;
}

answer_t ask_question(char *question, check_function check, convert_function convert)
{
  int buf_size = 20;
  char buf[buf_size];

  while (true)
  {

    printf("%s", question);
    read_string(buf, buf_size);
    if (check(buf))
    {
      break;
    }
  }
  return convert(buf);
}

char ask_question_menu(char *question)
{
  char *meny = ask_question(question, is_valid_meny, (convert_function)strdup).string_value;
  char answer = toupper(meny[0]);
  free(meny);
  return answer;
}
double ask_question_float(char *question)
{
  return ask_question(question, is_float, (convert_function)make_float).float_value;
}
char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_function)strdup).string_value;
}
int ask_question_int(char *question)
{
  return ask_question(question, is_number, (convert_function)atoi).int_value;
}
char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_function)strdup).string_value;
}