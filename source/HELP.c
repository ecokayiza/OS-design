#include "functions.h"

void display_help(){
    print("提供命令的帮助信息。\n\nHELP [command]\ncommand - 显示该命令的帮助信息。\n\n");
}
void display_commands(){
    print("有关某个命令的详细信息，请键入 HELP 命令名\nCLS        清除屏幕\nDATE       显示或设定日期\nTIME       显示或设定时间\nDOSKEY     用于查看命令行历史或编辑宏\nFIND       用于匹配文件内容\nFINDSTR    用于匹配文件或文件内容\nCOMP       以字节方式比较两个文件\nFC         以行比较两个文件并显示差异\nEXIT       退出当前程序\nMORE       显示来自管道的文件信息\n\n");
}

int main(int argc, char* argv[]){
    if(argc < 2 ){
        display_commands();
        return 0;
    }
    if (argv[1][0] == '/' && argv[1][1] == '?')
    {
        display_help();
        return 0;
    }
    for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "CLS") == 0) {
            system("CLS /?");
            return 0;
        } else if (strcmp(argv[i], "DATE") == 0) {
            system("DATE /?");
            return 0;
        } else if (strcmp(argv[i], "TIME") == 0) {
            system("TIME /?");
            return 0;
        } else if (strcmp(argv[i], "DOSKEY") == 0) {
            system("DOSKEY /?");
            return 0;
        } else if (strcmp(argv[i], "FIND") == 0) {
            system("FIND /?");
            return 0;
        } else if (strcmp(argv[i], "FINDSTR") == 0) {
            system("FINDSTR /?");
            return 0;
        } else if (strcmp(argv[i], "COMP") == 0) {
            system("COMP /?");
            return 0;
        } else if (strcmp(argv[i], "FC") == 0) {
            system("FC /?");
            return 0;
        } else if (strcmp(argv[i], "EXIT") == 0) {
            system("EXIT /?");
            return 0;
        } else if (strcmp(argv[i], "MORE") == 0) {
            system("MORE /?");
            return 0;
        }
    }

    return 0;
}