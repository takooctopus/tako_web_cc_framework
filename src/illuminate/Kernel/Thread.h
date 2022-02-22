#ifndef __THREAD__
#define __THREAD__
    #include <pthread.h>
    #include <sys/syscall.h>
    #include <unistd.h>
    #include <functional>
    #include <memory>
    #include <string>
    #include "CountDownLatch.h"
    #include "noncopyable.h"

class Thread : noncopyable{
public:
    using ThreadFunc = std::function<void()>; // 这个得写在public里，要不cc里面找不到
private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
public:
    explicit Thread(const ThreadFunc&, const std::string &name = std::string());
    ~Thread();
    void start();
    int join();
    bool started() const {return started_;}
    pid_t tid() const {return tid_;}
    const std::string& name() const {return name_;}
};


#endif