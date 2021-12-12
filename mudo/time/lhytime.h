//
// Created by 78472 on 2021/12/11.
//

#ifndef STL_LHYTIME_H
#define STL_LHYTIME_H

/*
 * time_t和struct tm之间的相互转换
    time_t time(time_t *seconds);
    time_t mktime(struct tm *timeptr);
    struct tm *gmtime(const time_t *timer);
    struct tm *localtime(const time_t *timer);


 * 字符串化的格式表示
    char *ctime(const time_t *timer);
    char *asctime(const struct tm *timeptr);
    size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr);

注释
    ctime(),asctime()的字符串形式  "Www Mmm dd hh:mm:ss yyyy\n\0"。
    strftime()得到的字符串不自带换行
    strftime()可以以任意想要的格式来表示时间
*/


void timetest();

void tmTest();



#endif //STL_LHYTIME_H
