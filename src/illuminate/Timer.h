#ifndef TIMER__
#define TIMER__

#include <unistd.h>
#include <deque>
#include <memory>
#include <queue>

#include "Kernel/MutexLock.h"
#include "Kernel/noncopyable.h"
#include "HttpData.h"

class HttpData;

class TimerNode
{
private:
    bool deleted_;
    size_t expired_time_;
    std::shared_ptr<HttpData> sp_http_data_;
public:
    TimerNode(std::shared_ptr<HttpData> request_data, int timeout);
    TimerNode(TimerNode &tn);
    ~TimerNode();
    void update(int timeout);
    bool isValid();
    void clearReq();
    void setDeleted(){deleted_ = true;}
    bool isDeleted() const { return deleted_;}
    size_t getExpTime() const { return expired_time_;}
};

struct TimerCmp{
    bool operator()(std::shared_ptr<TimerNode> &a,
                    std::shared_ptr<TimerNode> &b) const {
                        return a->getExpTime() > b->getExpTime();
                    }
};

class TimerManager{
private:
    using SPTimerNode = std::shared_ptr<TimerNode>;
    std::priority_queue<SPTimerNode, std::deque<SPTimerNode>, TimerCmp> timer_node_queue_;
public:
    TimerManager();
    ~TimerManager();
    void addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout);
    void handleExpiredEvent();

};

#endif