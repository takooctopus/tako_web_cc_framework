#ifndef LOGFILE__
#define LOGFILE__

#include <memory>
#include <string>

#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"

class LogFIle{
    private:
    void append_unlocked(const char* logline, int len);
    
    const std::string basename_;
    const int flush_interval_;
}

#endif