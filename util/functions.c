#include "functions.h"
#include <unistd.h>
#include <sys/syscall.h>
///////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILE_LINE 1024
#define MAX_LINE_LEN 1024

////////////////////////////////////////////////////////////////////////////////////////////////////

int get_strlen(const char *str) {
    int len = 0;
    const char *tmp = str;
    while(*tmp++) {
        len++;
    }
    return len;
}

void print(const char *str) {
    size_t bytes=0;
    const char *start = str;
    while(*start){
        start++;
        bytes++;
    }
    // sys_call(SYS_write, STDOUT_FILENO, str, bytes);
    write(STDOUT_FILENO, str, bytes);
}

char* scan(){
    static char str[100];
    int i = 0;
    char c;
    while(1){
        read(STDIN_FILENO, &c, 1);
        if(c == '\n'){
            str[i] = '\0';
            break;
        }
        str[i] = c;
        i++;
    }
    return str;
}



char* int_to_char(int num){
    int is_negative = 0;
    int i = 0;
    if(num < 0){
        is_negative = 1;
        num = -num;
    }
    int tmp=num;
    do{
        i++;
        tmp /= 10;
    }while(tmp);
    //////////////////////////////////////
    static char str[30];
    str[i+is_negative] = '\0';
    do{
        i--;
        str[i+is_negative]= num % 10 + '0';
        num /= 10;
    }while(i);
    if(is_negative){
        str[0] = '-';
    }
    return str;
}

int char_to_int(char* str){
    int num = 0;
    int is_negative = 0;
    if(*str == '-'){
        is_negative = 1;
        str++;
    }
    while(*str){
        num = num * 10 + *str - '0';
        str++;
    }
    if(is_negative){
        num = -num;
    }
    return num;
}

//检查数字字符串
int is_digit(char* str) {
    while(*str) {
        if(*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }
    return 1;
}

//检查root权限
int check_permission(){
    int uid=getuid();
    print("uid: ");
    print(int_to_char(uid));
    print("\n");

    if(geteuid() != 0){
        return 0;
    }
    return 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//计算部分匹配表（前缀函数）
void compute_prefix_function(const char *pattern, int *prefix, int m) {
    int length = 0;
    prefix[0] = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[length]) {
            length++;
            prefix[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = prefix[length - 1];
            } else {
                prefix[i] = 0;
                i++;
            }
        }
    }
}

int contains(const char *text_p, const char *pattern, int i_option) {
    char *text;
    if(i_option){
        text = strdup(text_p);
        for (int i = 0; text[i]; i++) 
            text[i] = tolower(text[i]);
    }else{
        text = strdup(text_p);
    }

    int n = strlen(text);
    int m = strlen(pattern);
    int *prefix = (int *)malloc(m * sizeof(int));
    compute_prefix_function(pattern, prefix, m);

    int i = 0; // text的索引
    int j = 0; // pattern的索引
    while (i < n) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == m) {
            free(prefix);
            return 1; // 找到匹配
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = prefix[j - 1];
            } else {
                i++;
            }
        }
    }
    free(prefix);
    return 0;
}

struct LINE_INFO{
    char *line;
    int line_num;
    int if_in;
};

struct LINE_INFO* search(FILE *file, const char *pattern_p, int i_option)
{
    struct LINE_INFO *result = (struct LINE_INFO *)malloc(MAX_FILE_LINE * sizeof(struct LINE_INFO));
    char line[MAX_LINE_LEN];
    int line_num = 0;
    int i = 0;
    char *pattern;

    if(i_option){
        pattern = strdup(pattern_p);
        for (int j = 0; pattern[j]; j++) 
            pattern[j] = tolower(pattern[j]);
    }else{
        pattern = strdup(pattern_p);
    }

    while(fgets(line, sizeof(line), file)){
        line_num++;
        result[i].line = strdup(line);
        result[i].line_num = line_num;
        if(contains(line, pattern, i_option)){
            result[i].if_in = 1;
        } else {
            result[i].if_in = 0;
        }
        i++;
    }
    return result;
}

struct LINE_INFO* multi_search(FILE *file, const char *patterns_p[],int pattern_count ,int i_option)
{
    struct LINE_INFO *result = (struct LINE_INFO *)malloc(MAX_FILE_LINE * sizeof(struct LINE_INFO));
    char line[MAX_LINE_LEN];
    int line_num = 0;
    int i = 0;
    char *patterns[pattern_count];

    // for(int j = 0; j < pattern_count; j++){
    //     print(patterns_p[j]);
    //     print("\n");
    // }

    if(i_option){
        for (int j = 0; j < pattern_count; j++){
            patterns[j] = strdup(patterns_p[j]);
            for (int k = 0; patterns[j][k]; k++) 
                patterns[j][k] = tolower(patterns[j][k]);
        }                          
    }else{
        for (int j = 0; j < pattern_count; j++){
            patterns[j] = strdup(patterns_p[j]);
            }
    }


    while(fgets(line, sizeof(line), file)){
        line_num++;
        result[i].line = strdup(line);
        result[i].line_num = line_num;
        result[i].if_in = 0;
        for (int j = 0; j < pattern_count; j++)
        {
            if (contains(line, patterns[j], i_option))
            {
                // print("  find\n");
                // print(line);
                result[i].if_in = 1;
                break;
            }
        }
        i++;
    }
    return result;
}
////////////////////////////////////////////////////////////////