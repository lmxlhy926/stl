//
// Created by 78472 on 2022/5/31.
//

#ifndef EXHIBITION_LOGFILE_H
#define EXHIBITION_LOGFILE_H

#include <string>
#include <memory>
#include <mutex>
#include "common/noncopyable.h"
using namespace std;

namespace muduo{
    /*
     * 将日志字符串写入文件：
     *      1. 构造：打开文件，设置缓冲区
     *      2. 写入内容，修改writtenBytes_
     *      3. 刷新文件内容
     *      4. 析构：关闭文件(文件关闭时会自动刷新)
     */
    class AppendFile : noncopyable{
    private:
        FILE* fp_;                      //文件描述符
        char buffer_[64 * 1024]{};      //文件缓冲区
        size_t writtenBytes_;           //写入文件的字节总数
    private:
        size_t write(const char* logline, size_t len);
    public:
        /**
         * 如果文件打开失败，退出程序
         * @param fileName 要打开的日志文件的路径
         */
        explicit AppendFile(const string& fileName);

        //析构时，如果文件打开，则关闭文件
        ~AppendFile();

        //将内容写入文件，增加已写入的字节数
        void append(const char* logline, size_t len);

        //文件刷新
        void flush() {  ::fflush(fp_); }

        size_t writtenBytes() const { return writtenBytes_;}
    };


    /*
     * 将日志写入到日志文件：
     *      日志文件会自动更新：到达文件设定的可写入的最大字节数，或者日期天数发生变化
     */
    class LogFile : noncopyable{
    private:
        const string baseName_;     //文件全路径名
        const size_t rollSize_;     //文件的最大容纳字节数
        const int flushInterval_;   //最大刷新间隔
        const int checkEveryN_;     //写入checkEveryN_次后，检查是否需要刷新以及更新日志文件
        std::unique_ptr<std::recursive_mutex> mutex_;   //互斥锁

        int count_;                             //记录写入次数，计数到checkEveryN_后归零一次
        time_t startOfPeriod_;                  //创建日志文件的日期：精确到天数
        time_t lastRoll_;                       //上一次产生新的日志文件的时间戳
        time_t lastFlush_;                      //上一次刷新操作的时间戳
        std::unique_ptr<AppendFile> file_;      //实际接收日志写入的对象
        const static int kRollPerSecods = 60 * 60 * 24;     //一天代表的秒数

    public:
        LogFile(const string& baseName,
                size_t rollSize,
                bool threadSafe = true,
                int flushInterval = 3,
                int checkEveryN = 1024);

        ~LogFile() = default;

        //写入打印信息，线程安全
        void append(const char* logline, size_t len);

        //刷新内容到文件中，线程安全
        void flush();

    private:
        //关闭当前的AppendFile对象，生成新的AppendFile对象
        bool rollFile();

        void append_unlocked(const char* logline, int len);

        /*
         * 生成组装的日志文件名：
         *      1. 格式：basename.<时间>.log
         *      2. 时间精确到秒
         */
        static string getLogFileName(const string& baseName, time_t* now);
    };

}



#endif //EXHIBITION_LOGFILE_H
