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


struct LINE_INFO;
struct LINE_INFO *search(FILE *file, const char *pattern_p, int i_option);
int contains(const char *text, const char *pattern, int i_option);
struct LINE_INFO *multi_search(FILE *file, const char *patterns_p[], int pattern_count, int i_option, int r_option);
char *get_abs_path(const char *filename);
struct LINE_INFO *search_dir(const char *dir_p, const char *pattern_p, int i_option, int full_path_option);
#endif // FUNCTIONS_H