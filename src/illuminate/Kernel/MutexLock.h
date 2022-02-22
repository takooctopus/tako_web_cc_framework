#ifndef __MUTEX_LOCK__
#define __MUTEX_LOCK__

#include <pthread.h>
#include <cstdio>
#include "noncopyable.h"

class MutexLock : noncopyable
{
private:
    pthread_mutex_t mutex_;
private:
    friend class Condition;
public:
    MutexLock() {
        pthread_mutex_init(&mutex_, NULL);
    };
    ~MutexLock(){
        pthread_mutex_lock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    };
    void lock(){
        pthread_mutex_lock(&mutex_);
    }
    void unlock(){
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t* get(){
        return &mutex_;
    }
};

class MutexLockGuard: noncopyable{
private:
    MutexLock &mutex_;
public:
    explicit MutexLockGuard(MutexLock &_mutex): mutex_(_mutex){
        mutex_.lock();
    };
    ~MutexLockGuard(){
        mutex_.unlock();
    };
};


#endif