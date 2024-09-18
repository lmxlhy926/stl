//
// Created by 78472 on 2021/12/11.
//

#include <ctime>
#include <iostream>
#include "lhytime.h"

using namespace std;

void timetest(){
    time_t timeNow = time(nullptr);
    struct tm *gmtTime = gmtime(&timeNow);
    struct tm *localTime = localtime(&timeNow);

    std::cout << "ctime(&timeNow): " << ctime(&timeNow);
    std::cout << "asctime(gmtTime): "<< asctime(gmtTime);
    std::cout << "asctime(localTime): " << asctime(localTime);
    std::cout << std::endl;

    char buf[100]{};
    string formatString1 = "%c";                    //日期和时间表示法
    string formatString2 = "%x";                    //日期表示法
    string formatString3 = "%X";                    //时间表示法
    string formatString4 = "%Y-%m-%d %H:%M:%S";     //年-月-日 时-分-秒


    strftime(buf, 100, formatString1.c_str(), gmtTime);
    std::cout << "%c: " << buf << std::endl;

    strftime(buf, 100, formatString2.c_str(), gmtTime);
    std::cout << "%x: " << buf << std::endl;

    strftime(buf, 100, formatString3.c_str(), gmtTime);
    std::cout << "%X: " << buf << std::endl;

    strftime(buf, 100, formatString4.c_str(), gmtTime);
    std::cout << "%Y-%m-%d %H:%M:%S: " << buf << std::endl;

}


void tmTest(){
    time_t timeNow = time(nullptr);
    struct tm *localTime = localtime(&timeNow);
    std::cout << "localTime->tm_wday: " << localTime->tm_wday << std::endl;
    std::cout << "localTime->tm_yday: " << localTime->tm_yday << std::endl;
    std::cout << "localTime->tm_isdst: " << localTime->tm_isdst << std::endl;
    std::cout << std::endl;
    std::cout << "nowTime: " << asctime(localTime) << std::endl;

    struct tm tmTime = *localTime;
    tmTime.tm_wday = 0;
    tmTime.tm_yday = 0;
    tmTime.tm_isdst = 0;
    std::cout << "tmTime: " << asctime(&tmTime);

    time_t time = mktime(&tmTime);      //只看年月日时分秒，不看tm_wday和tm_yday
    std::cout << "time: " << ctime(&time);
}
