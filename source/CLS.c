#include "functions.h"

void clear_screen() {
    // 使用ANSI转义序列清屏
    print("\033[2J\033[H");
}

int main(int argc, char *argv[]) {
    if(argc > 1) {
        if (argv[1][0] == '/' && argv[1][1] == '?')
        {
            print("Clear the screen\n\nCLS\n\n");
            return 0;
        }else{
            print("Unknown args\n");
        }

    }else{
        clear_screen();
    }
    return 0;
}