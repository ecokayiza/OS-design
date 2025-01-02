#include "functions.h"
#include <unistd.h>
#include <sys/syscall.h>


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




