#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

void display_help(){
    print("目前实现了两个功能，显示历史记录和编辑命令行宏\n");
    print("DOSKEY [/history] [-mirco -exename] [/reinstall] [/micros] \n\n");
    print("/history[:n] 显示历史记录,可指定显示的最大历史数,默认10\n");
    print("micro=exename 编辑命令行宏\n");
    print("/reinstall 清除所有宏\n");
    print("/micros 显示所有宏\n");
}


char* add_home_path(char* filename){
    char *homedir = getenv("HOME");
    if (homedir == NULL) {
        perror("getenv error!");
        exit(1);
    }
    char *full_path = malloc(1024);
    int l = 0;
    for (l; homedir[l];l++)
        full_path[l] = homedir[l];
    full_path[l] = '/';
    int m = 0;
    for (m; filename[m]; m++)
        full_path[l+m+1] = filename[m];
    full_path[l+m+1] = '\0';
    return full_path;
}

void print_history(int lines_p) {
    char *filename = ".bash_history";
    char *full_path = add_home_path(filename);
    FILE *fp = fopen(full_path, "r");
    if(fp == NULL){
        perror("open file error!");
        exit(1);
    }
    char line[MAX_COMMAND_LENGTH];
    fseek(fp, 0, SEEK_END);  //到文件末尾
    long file_size = ftell(fp);
    int lines = 0;
    for (long pos = file_size - 1; pos > 0; pos--) {  //反向累加行数
        fseek(fp, pos, SEEK_SET);
        if (fgetc(fp) == '\n') {
            lines++;
            if (lines > lines_p) {
                break;
            }
        }
    }
    while (fgets(line, sizeof(line), fp) != NULL) {
        print(line);
    }
    fclose(fp);
}

void assure_file(char* path){
    FILE *fp = fopen(path, "r");
    if(fp == NULL){
        fp = fopen(path, "w");
        if (fp == NULL) {
            perror("create file error!");
            exit(1);
        }
    }
    fclose(fp);
}


void add_micro(char* micro, char* exename){
    char *filename = ".doskey_micros";
    char *full_path = add_home_path(filename);
    assure_file(full_path);

    //check if the micro already exists
    FILE *fp1 = fopen(full_path, "r");
    char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), fp1) != NULL) {
        if (strncmp(line+6, micro, strlen(micro)) == 0) {
            print("macro already exists\n");
            return;
        }
    }
    fclose(fp1);

    FILE *fp = fopen(full_path, "a");
    fwrite("alias ", 1, 6, fp);
    fwrite(micro, 1, strlen(micro), fp);
    fwrite("=", 1, 1, fp);
    fwrite(exename, 1, strlen(exename), fp);
    fwrite("\n", 1, 1, fp);
    fclose(fp);
}

void clear_micros(){
    char *filename = ".doskey_micros";
    char *full_path = add_home_path(filename);
    FILE *fp = fopen(full_path,"r");
    if (fp != NULL){
        fclose(fp);
        fp = fopen(full_path, "w");
        if (fp == NULL) {
            perror("clear file error!");
            exit(1);
        }
        fclose(fp);
    }
}

void print_micros(){
    char *filename = ".doskey_micros";
    char *full_path = add_home_path(filename);
    assure_file(full_path);
    // print(full_path);
    // print("\n");
    FILE *fp = fopen(full_path, "r");
    char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        print(line);
    }
    fclose(fp);
}

// void source_file(){
    // char* source_command = "bash -c 'source ";
    // char *source_path = add_home_path(".doskey_micros");
    // assure_file(source_path);
    // // print(source_path);
    // char* source_command_path = malloc(1024);
    // int i = 0;
    // for(i = 0; source_command[i]; i++){
    //     source_command_path[i] = source_command[i];
    // }
    // // print(source_command_path);
    // // print("\n");
    // int j = 0;
    // for (j; source_path[j]; j++)
    // {
    //     source_command_path[i+j] = source_path[j];
    // }
    // source_command_path[i+j] = '\'';
    // source_command_path[i+j+1] = '\0';
    // print(source_command_path);
    // print("\n");
    // system(source_command_path);
    // system("bash -c 'source ~/.bashrc'");
// }

int main(int argc, char* argv[]) {


    if (argc < 2)
    {
        return 0;
    }
    if (argv[1][0] == '/' && argv[1][1] == '?')
    {
        display_help();
        return 0;
    }
    int history_mode = 0;
    int edit_mode = 0;
    int reinstall_mode = 0;
    int micros_mode = 0;
    char *micro = NULL;
    char *exename = NULL;
    int max_lines = 10;

    for(int i = 1; i < argc; i++){
        if(strncmp(argv[i], "/history", 8) == 0){  // /history[:n]
            history_mode = 1;
            if(argv[i][8] == ':' && argv[i][9] != '\0'){
                char* num = argv[i] + 9;
                if(is_digit(num)){
                    max_lines = char_to_int(num);
                }else{
                    print("history:n格式错误\n");
                    return 0;
                }
            }
        } else if(strcmp(argv[i], "/reinstall") == 0){
            reinstall_mode = 1;
        } else if(strcmp(argv[i], "/micros") == 0){
            micros_mode = 1;
        } else{
            if(argv[i][0] == '=')
                return 0;
            for (int j = 0; argv[i][j]; j++)
            {
                if(argv[i][j]=='=' && argv[i][j+1] != '\0'){
                    micro = argv[i];
                    micro[j] = '\0';
                    exename = argv[i] + j + 1;
                    edit_mode = 1;
                }
                
            }
        }
    }
    ///////////////////////////////////////////////////////////////
    if(history_mode){
        print_history(max_lines);
        return 0;
    }
    if(edit_mode){
        add_micro(micro, exename);
    }
    if(micros_mode){
        print_micros();
        return 0;
    }
    if(reinstall_mode){
        clear_micros();
        return 0;
    }
    // source_file();
    return 0;
}