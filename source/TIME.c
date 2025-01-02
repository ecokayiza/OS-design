#include "functions.h"
#include "time_util.h"

struct TIME
{
    int hour;
    int minute;
    int second;
};

void display_help(){
    print("\nTIME [/T | time]\n\n显示当前时间设置和输入新时间的提示，请键入\n不带参数的 TIME。要保留现有时间，请按 Enter。\n\n如果命令扩展被启用，TIME 命令会支持 /T 命令行开关；该命令行开关告诉\n命令只输出当前时间，但不提示输入新时间。\n");
}

//显示TIME
void show_time(struct TIME time){
    if(time.hour < 10) print("0");
    print(int_to_char(time.hour));
    print(":");
    if(time.minute < 10) print("0");
    print(int_to_char(time.minute));
    print(":");
    if(time.minute < 10) print("0");
    print(int_to_char(time.second));
    print("\n\n");
}


//显示当前时间
void display_current_time()
{
    long seconds = get_time_stamp();
    // print(int_to_char(seconds));
    // print("\n");
    struct TIME time = get_time_from_stamp(seconds);
    show_time(time);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int check_legal_time(int hour,int minute,int second,int is_12){
    
    if(is_12){
        if(hour < 0 || hour > 11){
            return 0;
        }
    }else{
        if (hour < 0 || hour > 23) {
            return 0;
        }
    }

    if (minute < 0 || minute > 59) {
        return 0;
    }
    if (second < 0 || second > 59) {
        return 0;
    }
    return 1;
}


// time 17:30:00
// time 5:30 pm
// 检查时间格式 并获取时间
struct TIME check_time_format(const char *time) {
    int times[3];
    char temp[40]; //保存当前数字字符串
    int i = 0;
    int j = 0;
    int len = get_strlen(time)+1;
    struct TIME time_struct = {-1,-1,-1};
    
    
    for (int k = 0;k < len;k++) {
        char c = *time;   //必须先赋值给一个变量，否则会出现段错误
        // print(&c);
        // print("\n");
        if(c !=':'  && c != '\0') {
            temp[i] = c;
            i++;
        }
        else {
            //加上结束符
            char z = '\0';
            temp[i+1] = z;
            // print("\n jump out :");
            // print(temp);
            // print("\n");
            //检查合法性
            if (!is_digit(temp))
            {
                return time_struct;
            }
            //获取数字
            times[j] = char_to_int(temp);

            //重置temp(全部赋为结束符)
            for (i; i >= 0; i--){
                char z = '\0';
                temp[i] = z;
            }
            i++;
            // print("\n i : ");
            // print(int_to_char(i));
            // print("\n");
            j++;
        }
        //非法输入数量
        if(j > 3) {
            return time_struct;
        }
        time++;
    } ;

    if(j!=3) {
        return time_struct;
    }
    int hour = times[0];
    int minute = times[1];
    int second = times[2];

    time_struct.hour = hour;
    time_struct.minute = minute;
    time_struct.second = second;

    return time_struct;
    

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//设置时间
void set_sys_time(struct TIME time) {

    struct TIME copy = time;

    // 将当天的cn时间转换为国际标准时间
    time.hour -= 8;
    if (time.hour < 0) {
        time.hour += 24;
    }
    long day_seconds = get_seconds_from_time(time.hour, time.minute, time.second);
    long secs = get_time_stamp();
    secs = secs - secs % 86400 + day_seconds;

    //检查用户权限
    if(!check_permission()){
        print("user permission denied\n\n");
    }else{
        if(set_time(secs)){
            print("已将日期更改为");
            show_time(copy);
        }else{
            print("日期更改失败\n");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if(argv[1][0] == '/' && argv[1][1] == '?') {
            display_help();
            return 0;
        }
        if(argv[1][0] == '/' && (argv[1][1] == 'T' || argv[1][1] == 't')) {
            display_current_time();
            return 0;
        }
    }
    // print(int_to_char(argc));
    char *user_time;
    if (argc == 1){
        display_current_time();
        print("请输入新的时间:\n");
        user_time = scan();
    }
    else{
        user_time = argv[1];
    }
    // [am | pm]
    int time_option_flag = 0;
    if(argc > 2){
        char *time_option = argv[2];
        if (time_option[0] == 'p' && time_option[1] == 'm'){
            time_option_flag = 2;
        }else if (time_option[0] =='a' && time_option[1] == 'm'){
            time_option_flag = 1;
        }else{
            print("Unknown args\n");
            return 0;
        }
    }
    struct TIME time = check_time_format(user_time);
    // show_time(time);
    if(!check_legal_time(time.hour,time.minute,time.second,time_option_flag)){
        print("不是一个有效的时间\n");
        return 0;
    }
    if (time_option_flag == 2){
        time.hour += 12;
    }
    print("输入的时间为: ");
    show_time(time);

    set_sys_time(time);
}