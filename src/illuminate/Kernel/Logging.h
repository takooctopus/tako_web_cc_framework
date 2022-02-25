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
}

#define LOG Logger(__FILE__, __LINE__).stream()

#endif