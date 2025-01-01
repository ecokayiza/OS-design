
#ifndef TIME_UTIL_H
#define TIME_UTIL_H


struct DATE;
struct TIME;

int is_leap_year(int year);
long get_time_stamp();
int set_time(long seconds);

long get_seconds_from_date(int year, int month, int day);
long get_seconds_from_time(int hour, int minute, int second);

struct DATE get_date_from_stamp(long seconds);
struct TIME get_time_from_stamp(long seconds);



#endif // TIME_UTIL_H