#ifndef LOGGING__
#define LOGGING__

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "LogStream.h"

class AsyncLogging;

class Logger{
    private:
        class Impl{
            public:
            Impl(const char* file_name, int line);
            void formatTime();

            LogStream stream_;
            int line_;
            std::string base_name_;
        };

        Impl impl_;
        static std::string log_file_name_;
    public:
        Logger(const char* file_name, int line);
        ~Logger();
        LogStream &stream() {return impl_.stream_; }

        static void setLogFileName(std::string file_name){
            log_file_name_ = file_name;
        }

        static std::string getLogFileName(){
            return log_file_name_;
        }
};

#define LOG Logger(__FILE__, __LINE__).stream()

#endif