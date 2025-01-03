
#include "file_util.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <dirent.h>

#define MAX_FILE_LINE 1024
#define MAX_LINE_LEN 1024

struct LINE_INFO{
    char *line;
    int line_num;
    int if_in;
};

struct COMP_INFO
{
    int offset;
    int line;
    char file1;
    char file2;
};

struct FC_COMP_INFO
{
    int line;
    int is_diff;
    char* file1;
    char* file2;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//KMP字符匹配
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

struct LINE_INFO* multi_search(FILE *file, const char *patterns_p[],int pattern_count ,int i_option,int r_option)
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
    if(r_option){
        regex_t regex;
        int reti;
        if (i_option) {
            reti = regcomp(&regex, patterns_p[0], REG_ICASE | REG_EXTENDED);
        } else {
            reti = regcomp(&regex, patterns_p[0], REG_EXTENDED);
        }
        if (reti) {
            print("Could not compile regex\n");
            exit(1);
        }
        while(fgets(line, sizeof(line), file)){
            line_num++;
            result[i].line = strdup(line);
            result[i].line_num = line_num;
            result[i].if_in = 0;
            // print(line);
            reti = regexec(&regex, line, 0, NULL, 0);
            if (!reti) 
                result[i].if_in = 1;
            i++;
        }
        regfree(&regex);
    }else{
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
    }
    return result;
}

char * get_abs_path(const char* filename)
{
    if(contains(filename,"/",0)){
        return strdup(filename);
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        // print("Current working dir: ");
        // print(cwd);
        // print("\n");
        perror("getcwd() error");
        exit(1);
    } 
    char *full_path = malloc(1024);
    int l = 0;
    for (l; cwd[l];l++)
        full_path[l] = cwd[l];
    full_path[l] = '/';
    int m = 0;
    for (m; filename[m]; m++)
        full_path[l+m+1] = filename[m];
    full_path[l+m+1] = '\0';
    return full_path;
}

struct LINE_INFO *search_dir(const char *dir_p, const char *pattern_p, int i_option, int full_path_option)
{
    DIR *dir; //目录流
    struct dirent *entry; //目录项
    struct LINE_INFO *result = (struct LINE_INFO *)malloc(MAX_FILE_LINE * sizeof(struct LINE_INFO));
    char *pattern;
    pattern = strdup(pattern_p);

    if(i_option){
        for (int j = 0; pattern[j]; j++) 
            pattern[j] = tolower(pattern[j]);
    }
    if ((dir = opendir(dir_p)) == NULL) {
        perror("opendir() error");
        exit(1);
    } 
    
    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
            char *file_name = entry->d_name;
            if (contains(file_name, pattern, i_option)) {
                    if(full_path_option){
                    char *full_path = get_abs_path(file_name);
                    result[i].line = strdup(full_path);
                    free(full_path);
                    }else{
                        result[i].line = strdup(file_name);
                    }
                result[i].line_num = i + 1;
                result[i].if_in = 1;
                i++;
            }
        
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int comp_file_size(const char * file1, const char* file2){
    FILE *fp1 = fopen(file1, "rb");
    FILE *fp2 = fopen(file2, "rb");
    if (fp1 == NULL || fp2 == NULL) {
        perror("Error opening file");
        exit(1);
    }
    //移动指针到末尾
    fseek(fp1, 0, SEEK_END);
    fseek(fp2, 0, SEEK_END);
    //其位置即为大小
    long size1 = ftell(fp1);
    long size2 = ftell(fp2);
    fclose(fp1);
    fclose(fp2);
    if (size1 == size2) 
        return 1;
    return 0;
    }



struct COMP_INFO* compare(const char * file1, const char* file2, int n_lines, int c_mode){
    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");
    if (fp1 == NULL || fp2 == NULL) {
        perror("Error opening file");
        exit(1);
    }
    // c_mode 不区分ASCII大小写
    // n_lines 指定匹配的行范围
    if(n_lines == 0){
        n_lines = MAX_FILE_LINE;
    }
    struct COMP_INFO *result = (struct COMP_INFO *)malloc(MAX_FILE_LINE * sizeof(struct COMP_INFO));
    int offset = 0;
    int line = 1;
    int i = 0;
    result[0].line = 0;

    while (!feof(fp1) && !feof(fp2)) {
        char ch1 = fgetc(fp1);
        char ch2 = fgetc(fp2);
        if(c_mode){
            ch1 = tolower(ch1);
            ch2 = tolower(ch2);
        }
        if (ch1 != ch2) {
            result[i].offset = offset;
            result[i].line = line;
            result[i].file1 = ch1;
            result[i].file2 = ch2;
            i++;
            if(i > 9){ //找到10个不同项则停止匹配
                break;
            }
        }
        if (ch1 == '\n') {
            line++;
            if (line > n_lines) { //匹配行数限制
                break;
            }
        }
        offset++;
    }
    result[i].line = 0;
    fclose(fp1);
    fclose(fp2);

    // for(int j = 0; j < i; j++){

    //     print("line:");
    //     print(int_to_char(result[j].line));
    //     print(" ");
    //     char str[2] = {result[j].file1, '\0'};
    //     print(str);
    //     print(" ");
    //     char str2[2] = {result[j].file2, '\0'};
    //     print(str2);
    //     print("\n");
    // }
    return result;
}


struct FC_COMP_INFO* fc_compare(const char * file1, const char* file2, int c_mode)
{
    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");
    if (fp1 == NULL || fp2 == NULL) {
        perror("Error opening file");
        exit(1);
    }
    struct FC_COMP_INFO *result = (struct FC_COMP_INFO *)malloc(MAX_FILE_LINE * sizeof(struct LINE_INFO));
    char line1[MAX_LINE_LEN];
    char line2[MAX_LINE_LEN];
    int line_num = 0;
    int i = 0;

    while (fgets(line1, sizeof(line1), fp1) && fgets(line2, sizeof(line2), fp2)) {
        line_num++;
        if(line_num > MAX_FILE_LINE){
            break;
        }


        char *line1_dup = strdup(line1); //用于比较
        char *line2_dup = strdup(line2);
        char *line1_dupp = strdup(line1);
        char *line2_dupp = strdup(line2);
        result[i].file1 = line1_dupp;  //保存原始行
        result[i].file2 = line2_dupp;
        result[i].line = line_num;

        if (c_mode) {
            for (int j = 0; line1_dup[j]; j++) 
                line1_dup[j] = tolower(line1_dup[j]);
            for (int j = 0; line2_dup[j]; j++) 
                line2_dup[j] = tolower(line2_dup[j]);
        }

        result[i].line = line_num;
        if (strcmp(line1_dup, line2_dup) == 0) { 
            result[i].is_diff = 0;  //相同
        } else {
            // print(int_to_char(line_num));
            // print("\n");
            result[i].is_diff = 1;  //不同
        }
        
        i++;
        free(line1_dup);
        free(line2_dup);
    }
    result[i].line = 0;

    fclose(fp1);
    fclose(fp2);
    return result;
}
