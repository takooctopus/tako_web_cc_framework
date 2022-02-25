#ifndef EVENT_LOOP__
#define EVENT_LOOP__
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>

#include "Epoll.h"
#include "Util.h"
#include "Kernel/CurrentThread.h"
#include "Kernel/Logging.h"
#include "Kernel/Thread.h"


class EventLoop
{
private:
    using SPEpoll = std::shared_ptr<Epoll>;
    using Functor = std::function<void()>;
    using SPChannel = std::shared_ptr<Channel>;
    bool looping_;
    SPEpoll poller_;
    int wakeup_fd_;
    bool quit_;
    bool event_hadling_;
    mutable MutexLock mutex_;
    std::vector<Functor> pending_functors_;
    bool calling_pending_functors_;
    const pid_t thread_id_;
    SPChannel p_wakedup_channel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();

public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
    bool isInLoopThread() const { return thread_id_ == CurrentThread::tid(); }
    void assertInLoopThread() { assert(isInLoopThread()); }
    void shutdown(SPChannel channel){ shutDownWR(channel->getFd()); }
    void removeFromPoller(SPChannel channel){ poller_->epoll_del(channel); }
    void updatePoller(SPChannel channel, int timeout = 0){
        poller_->epoll_mod(channel, timeout);
    }
    void addToPoller(SPChannel channel, int timeout = 0){
        poller_->epoll_add(channel, timeout);
    }
};


#endif
