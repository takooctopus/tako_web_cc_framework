#ifndef LOGFILE__
#define LOGFILE__

#include <memory>
#include <string>

#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"

class LogFile{
    private:
    void append_unlocked(const char* logline, int len);
    
    const std::string basename_;
    // 写入间隔次数，到次数了就往文件写入
    const int flush_interval_; 
    int count_;
    using UPMutexLock = std::unique_ptr<MutexLock>;
    using UPAppendFile = std::unique_ptr<AppendFile>;
    UPMutexLock mutex_;
    UPAppendFile file_;

    public:
    LogFile(const std::string& basename, int flush_interval = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    void rollFile();
};

#endif