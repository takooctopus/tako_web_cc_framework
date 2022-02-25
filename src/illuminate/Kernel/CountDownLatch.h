#ifndef COUNT_DOWN_LATCH__
#define COUNT_DOWN_LATCH__
#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

//确保Thread中传进去的func真实启动后，再返回外层
class CountDownLatch : noncopyable
{
private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
public:
    explicit CountDownLatch(int count); //初始化了锁，状态和时间，后面的函数只能等待或者减少时间
    ~CountDownLatch();
    void wait();
    void countDown();
};

#endif