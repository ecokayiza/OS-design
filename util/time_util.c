#include "time_util.h"
#include <linux/time.h>
// #include <time.h>
// #include <sys/time.h>
#include "functions.h"
#include <unistd.h>
#include <sys/syscall.h>


struct DATE
{
    int year;
    int month;
    int day;
    int day_of_week;
};

struct TIME
{
    int hour;
    int minute;
    int second;
};


//获取当前时间戳 from sys
long get_time_stamp() {
    struct timespec ts;  //存储时间
    syscall(SYS_clock_gettime, CLOCK_REALTIME, &ts); //获取系统时钟时间
    long seconds = ts.tv_sec; //获取时间戳
    return seconds;
}

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/////////////////////////
//获取指定日期的秒数
long get_seconds_from_date(int year,int month,int day){
    int days=0;

    int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(is_leap_year(year)){
        days_in_month[1] = 29;
    }
    month--;
    for (int i = month; i > 0; i--)
    {   
        // print("month: ");
        // print(int_to_char(i));
        // print("\n");
        days += days_in_month[i-1];
    }
    days += day;
    year--;
    while (year >= 1970)
    {
        days += is_leap_year(year) ? 366 : 365;
        year--;
    }
    days -= 1;
    long seconds = days * 86400;
    return seconds;
}

//获取指定时间的秒数
long get_seconds_from_time(int hour,int minute,int second){
    long seconds = 0;
    seconds += hour * 3600;
    seconds += minute * 60;
    seconds += second;
    return seconds;
}

///////////////////////////////////////////////////


//检查润年


//从时间戳获取日期
struct DATE get_date_from_stamp(long seconds) {
    

    int days = seconds / 86400;
    days++;
    int year = 1970;
    int month = 0;
    int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int year_days = is_leap_year(year) ? 366 : 365;

    //年份计算
    while(days > year_days){ //判断是否是闰年
        days -= year_days;
        year++;
        year_days = is_leap_year(year) ? 366 : 365;
    }

    if(is_leap_year(year)){
        days_in_month[1] = 29;
    }
    //月份计算
    for(month = 0; month < 12; month++){
        if(days <= days_in_month[month]){
            break;
        }
        days -= days_in_month[month];
    }
    month++;

    struct DATE date={0,0,0,0};

    int day_seconds = seconds % 86400;
    int hour = day_seconds / 3600;

    // 时区修正 (UTC+8)
    hour = (hour + 8);
    if(hour >=24){
        days++;
        if(days > days_in_month[month-1]){
            days = 1;
            month++;
            if(month > 12){
                month = 1;
                year++;
            }
        }
    }
    date.year = year;
    date.month = month;
    date.day = days;
    //星期公式
    // W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
    
    if (month == 1 || month == 2) {
        month += 12;
        year--;
    }
    int day_of_week = (days + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
    // print("get date: ");
    // print(int_to_char(day_of_week));
    date.day_of_week = day_of_week;

    return date;
}

//从时间戳获取当天时间
struct TIME get_time_from_stamp(long seconds){
    
    struct TIME time = {-1, -1, -1}; 
    seconds = seconds % 86400;
    int hour = seconds / 3600;
    int minute = (seconds % 3600) / 60;
    int second = seconds % 60;
    time.hour = hour;
    time.minute = minute;
    time.second = second;
    // (UTC+8)
    time.hour = (time.hour + 8) % 24;

    return time;
}
//////////////////////////////////////////////////////////////////////////////////

//设置系统时间
int set_time(long seconds) {
    struct timespec ts;
    ts.tv_sec = seconds;
    ts.tv_nsec = 0;
    if (syscall(SYS_clock_settime, CLOCK_REALTIME, &ts) == -1) {
        return 0;
    }
    return 1;
}