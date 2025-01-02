#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#define MAX_FILE_LINE 1024
#define MAX_LINE_LEN 1024


void print(const char *str);
char *scan();

char *int_to_char(int num);
int char_to_int(char *str);


int get_strlen(const char *str);
int is_digit(char *str);
int check_permission();



#endif // FUNCTIONS_H