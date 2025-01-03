#include "functions.h"
#include <sys/types.h>
#include <signal.h>


void display_help(){
    print("用于退出当前程序\n");
    print("EXIT\n");
    print("退出当前程序\n");
}

int main(int argc, char* argv[]){
    if(argc < 2 ){
        pid_t parent_pid = getppid(); // 获取父进程ID
        kill(parent_pid, SIGHUP);     // 发送SIGHUP信号给父进程
        return 0;
    }
    if (argv[1][0] == '/' && argv[1][1] == '?')
    {
        display_help();
        return 0;
    }
    return 0;
}