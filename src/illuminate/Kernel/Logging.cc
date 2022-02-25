#include "Logging.h"
#include "CurrentThread.h"
#include "Thread.h"
#include "AsyncLogging.h"
#include <assert.h>
#include <iostream>
#include <time.h>  
#include <sys/time.h> 

/*
    int pthread_once(pthread_once_t* once_control, void (*init_routine)(void));
        本函数使用初值为PTHREAD_ONCE_INIT的once_control变量保证init_routine()函数在本进程执行序列中仅执行一次。在多线程编程环境下，尽管pthread_once()调用会出现在多个线程中，init_routine()函数仅执行一次，究竟在哪个线程中执行是不定的，是由内核调度来决定。


*/

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging* async_logger_;

std::string Logger::log_file_name_ = "./log/WebServer.log";

void once_init(){
    async_logger_ = new AsyncLogging(Logger::getLogFileName());
    async_logger_->start();
}

void output(const char* msg, int len){
    pthread_once(&once_control_, once_init);
    async_logger_->append(msg, len);
}

Logger::Impl::Impl(const char* file_name, int line)
    :   stream_(),
        line_(),
        base_name_(file_name){
            formatTime();
        }

void Logger::Impl::formatTime(){
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    stream_ << str_t;
}

Logger::Logger(const char* file_name, int line): impl_(file_name, line){}

Logger::~Logger(){
    impl_.stream_ << " -- " << impl_.base_name_ << ':' << impl_.line_ << " \n";
    const LogStream::Buffer& buff(stream().buffer());
    output(buff.data(), buff.length());
}