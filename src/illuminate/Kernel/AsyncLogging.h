#ifndef ASYNC_LOGGING__
#define ASYNC_LOGGING__

#include <functional>
#include <string>
#include <vector>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "MutexLock.h"
#include "Thread.h"
#include "noncopyable.h"

class AsyncLogging{
    private:
    void threadFunc();
    using Buffer = FixedBuffer<kLargeBUffer>;
    using SPBuffer = std::shared_ptr<Buffer>;
    using BufferVector = std::vector<SPBuffer>;
    const int flush_interval_;
    bool running_;
    std::string basename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    SPBuffer current_buffer_;
    SPBuffer next_buffer_;
    BufferVector buffers_;
    CountDownLatch latch_;

    public:
    AsyncLogging(const std::string base_name, int flush_internal = 2);
    ~AsyncLogging(){
        if (running_){
            stop();
        }
    }

    void append(const char* logline, int len);

    void start(){
        running_cond_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop(){
        running_cond_ = false;
        cond_.notify();
        thread_.join();
    }
}

#endif
