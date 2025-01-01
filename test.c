#include <linux/time.h>
#include <unistd.h>
#include <sys/syscall.h>



int get_strlen(const char *str) {
    int len = 0;
    while(*str++) {
        len++;
    }
    return len;
}

void print(const char *str) {
    size_t bytes=0;
    const char *start = str;
    while(*str){
        str++;
        bytes++;
    }
    write(STDOUT_FILENO, start, bytes);
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
    static char str[20];
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


long get_time() {
    struct timespec ts;  //存储时间
    syscall(SYS_clock_gettime, CLOCK_REALTIME, &ts); //获取系统时钟时间
    long seconds = ts.tv_sec; //获取时间戳
    return seconds;
}

int is_digit(char* str) {
    while(*str) {
        if(*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }
    return 1;
}

int main(){
    char* str = "123";
    print(int_to_char(is_digit(str)));

}