#include "functions.h"
#include <stdio.h>
#include <sys/ioctl.h>
// #include <unistd.h>

void display_help(){
    print("用于显示文件内容\n如cat file | MORE\n或者 more < file\n");
    print("回车显示下一行,空格显示下一页,q退出\n\n");
}


int get_terminal_rows() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    //TIOCGWINSZ请求码用于获取终端窗口的大小。
    return w.ws_row;
}



int main(int argc, char *argv[]) {
    if(argc > 1 ){
        if (argv[1][0] == '/' && argv[1][1] == '?')
        {
            display_help();
            return 0;
        }

        int file_mode = 0;
        char *file_name = NULL;

        for(int i = 1; i < argc; i++){
            if(argv[i] =="<"){
                file_mode = 1;
                if(argc > i+1){
                    file_name = argv[i+1];
                 }
            }

        }

        if(file_mode){  //将文件作为标准输入
            if (freopen(file_name, "r", stdin) == NULL) {
                perror("freopen");
                return 1;
            }
        }
    }
    char buffer[1024];
    int lines = 0;
    int max_lines = (get_terminal_rows() - 7 > 0 )? get_terminal_rows() - 7 : 1; 
    FILE *tty = fopen("/dev/tty", "r");  // 打开终端设备文件

    if (tty == NULL) {
        perror("fopen");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        print(buffer);
        lines++;
        if (lines > max_lines) {
            // print("MORE >");
            char c;
            read(fileno(tty), &c, 1);  //从终端获取用户输入
            if (c == 'q') {
                break;
            }else if(c==' '){
                lines=0;
            }else{
                lines--;
            }
        }
    }
    return 0;
}