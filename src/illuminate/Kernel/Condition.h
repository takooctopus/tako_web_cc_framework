#ifndef __CONDITION__
#define __CONDITION__
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include "MutexLock.h"
#include "noncopyable.h"

class Condition : noncopyable{
private:
    MutexLock &mutex_; //拿了个引用
    pthread_cond_t cond_;
public:
    explicit Condition(MutexLock &_mutex) : mutex_(_mutex) {
        pthread_cond_init(&cond_, NULL); //初始化
    };
    ~Condition(){
        pthread_cond_destroy(&cond_);
    };
    void wait() {
        pthread_cond_wait(&cond_,  mutex_.get());
    }
    void notify(){
        pthread_cond_signal(&cond_);
    }
    void notifyAll(){
        pthread_cond_broadcast(&cond_);
    }
    bool waitForSeconds(int seconds){
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get(), &abstime);
    }
};


#endif