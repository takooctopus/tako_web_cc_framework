#include "LogFile.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "FileUtil.h"

LogFile::LogFile(const std::string& basename, int flush_interval)
    :   basename_(basename), 
        flush_interval_(flush_interval),
        count_(0),
        mutex_(new MutexLock()){
            //重置unique_ptr指针，销毁内部对象并接受新的对象的所有权
            file_.reset(new AppendFile(basename)); 
}

// 因为都用的智能指针，没啥要释放的
LogFile::~LogFile(){};

void LogFile::append(const char* logline, int len){
    MutexLockGuard lock(*mutex_); //返回所占用的对象，所以MutexLockGuard::lock()方法里使用的引用
    append_unlocked(logline, len);
}

void LogFile::flush(){
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::append_unlocked(const char* logline, int len){
    file_->append(logline, len);
    // 计数，如果满了，就清空缓存区
    ++count_;
    if(count_ >= flush_interval_){
        count_ = 0;
        file_->flush();
    }
}
