#include "functions.h"
#include "time_util.h"
#include <linux/time.h>

struct DATE {
    int year;
    int month;
    int day;
    int day_of_week;
};

void display_help() {
    print("\n显示或设置日期。\n\nDATE [/T | date]\n\n显示当前日期设置和输入新日期的提示，请键入不带参数的 DATE。\n要保留现有日期，请按 Enter。\n\n如果命令扩展被启用，DATE 命令会支持 /T 开关；\n该开关指示命令只输出当前日期，但不提示输入新日期。\n\n");
}

//显示DATE
void show_date(struct DATE date){
    print(int_to_char(date.year));
    print("/");
    print(int_to_char(date.month));
    print("/");
    print(int_to_char(date.day));
    print(" ");
    char* day_of_weeks[] = {"一", "二", "三", "四", "五", "六", "天"}; 
    print("星期");
    print(day_of_weeks[date.day_of_week]);
    print("\n\n");
}

//显示当前日期
void display_current_date()
{
    long seconds = get_time_stamp();
    struct DATE date = get_date_from_stamp(seconds);
    show_date(date);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//检查日期合法性
int check_legal_date(int year, int month, int day) {
    if(year < 1970 || year > 9999) {
        return 0;
    }
    if(month < 1 || month > 12) {
        return 0;
    }
    int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(is_leap_year(year)) {
        days_in_month[1] = 29;
    }
    if(day < 1 || day > days_in_month[month-1]) {
        return 0;
    }
    return 1;
}


// date 08.03.2007
// date 08-03-07
// date 8/3/07
// 检查日期格式 并获取日期
struct DATE check_time_format(const char *date) {
    int dates[3];
    char temp[40]; //保存当前数字字符串
    int i = 0;
    int j = 0;
    int len = get_strlen(date)+1;
    struct DATE date_struct = {0,0,0,0};
    
    
    for (int k = 0;k < len;k++) {
        char c = *date;   //必须先赋值给一个变量，否则会出现段错误
        // print(&c);
        // print("\n");
        if(c !='.' && c != '-' && c != '/' && c != '\0') {
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
                return date_struct;
            }
            //获取数字
            dates[j] = char_to_int(temp);

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
            return date_struct;
        }
        date++;
    } ;

    if(j!=3) {
        return date_struct;
    }
    int month = dates[0];
    int day = dates[1];
    int year = dates[2];

    year += year < 2000 ? 2000 : 0;
    if (check_legal_date(year, month, day)){
        date_struct.year = year;
        date_struct.month = month;
        date_struct.day = day;

        print("输入的日期为: ");
        print(int_to_char(year));
        print("/");
        print(int_to_char(month));
        print("/");
        print(int_to_char(day));
        print("\n");

        return date_struct;
    }

    return date_struct;
}

//设置时间
void set_sys_time(int year,int month,int day) {

    // print("target date: ");
    // print(int_to_char(year));
    // print("/");
    // print(int_to_char(month));
    // print("/");
    // print(int_to_char(day));
    // print("\n");

    long secs = get_seconds_from_date(year, month, day);
    
    //还原当天时间
    struct timespec ts;
    syscall(SYS_clock_gettime, CLOCK_REALTIME, &ts);
    int day_seconds = ts.tv_sec % 86400;
    secs += day_seconds;

    // print("get time stamp : ");
    // print(int_to_char(secs));
    // print("\n");

    struct DATE date_from_stamp = {0,0,0,0};
   
    date_from_stamp = get_date_from_stamp(secs);

    // print("get date: ");
    // print(int_to_char(date_from_stamp.year));
    // print("/");
    // print(int_to_char(date_from_stamp.month));
    // print("/");
    // print(int_to_char(date_from_stamp.day));
    // print("\n");

    //检查用户权限
    if(!check_permission()){
        print("user permission denied\n\n");
    }else{
        if(set_time(secs)){
            print("已将日期更改为");
            print(int_to_char(year));
            print("/");
            print(int_to_char(month));
            print("/");
            print(int_to_char(day));
            print("\n");
        }else{
            print("日期更改失败\n");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if(argv[1][0] == '/' && argv[1][1] == '?') {
            display_help();
            return 0;
        }
        else if(argv[1][0] == '/' && (argv[1][1] == 'T' || argv[1][1] == 't')) {
            display_current_date();
            return 0;
        }
        else {
            char *user_date = argv[1];
            struct DATE date = check_time_format(user_date);


            if(date.year == 0 || date.month == 0 || date.day == 0) {
                print("not a valid date\n");
                return 0;
            }else{
                set_sys_time(date.year, date.month, date.day);
            }
        }
    }
    else {
        display_current_date();
        //wiat for user input
        print("请输入新日期:\n");
        char *user_date = scan();
        struct DATE date = check_time_format(user_date);
        if (date.year == 0 || date.month == 0 || date.day == 0){
            print("not a valid date\n");
            return 0;
        }
        else{
            set_sys_time(date.year, date.month, date.day);
        }
    }
}