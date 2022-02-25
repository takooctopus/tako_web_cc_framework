#include "Epoll.h"
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include <deque>
#include <queue>

#include "Util.h"
#include "Kernel/Logging.h"
// TODO:
#include <arpa/inet.h> //提供IP地址转换函数
#include <iostream>


const int kEventSum = 4096;
const int kEpollWaitTime = 10000;

using SPChannel = std::shared_ptr<Channel>;

Epoll::Epoll() 
    : epoll_fd_(epoll_create1(EPOLL_CLOEXEC)), events_(kEventSum){
        assert(epoll_fd_ > 0);
    }

Epoll::~Epoll(){};

/*
    注册新的描述符
*/
void Epoll::epoll_add(SPChannel request, int timeout){
    int fd = request->getFd();
    if(timeout > 0){
        addTimer(request, timeout); //默认过期时间会加一个定时器
        fd2http_[fd] = request->getHoleder();
    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();

    request->EqualAndUpdateLastEvents();

    fd2chan_[fd] = request;
    if(epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0){
        perror("epoll_add error");
        fd2chan_[fd].reset();
    }
}

/*
    修改描述符状态
*/

void Epoll::epoll_mod(SPChannel request, int timeout){
    if(timeout > 0){
        addTimer(request, timeout);
    }
    int fd = request->getFd();
    if(!request->EqualAndUpdateLastEvents()){
        struct epoll_event event;
        event.data.fd = fd;
        event.events = request->getEvents();
        if(epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event) < 0){
            perror("epoll_mod error");
            fd2chan_[fd].reset();
        }
    }
}

void Epoll::epoll_del(SPChannel request){
    int fd = request->getFd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    if(epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &event) < 0){
        perror("epoll_del error");
    }
    fd2http_[fd].reset();
    fd2chan_[fd].reset();
}

std::vector<SPChannel> Epoll::poll(){
    while(true){
        int event_count = epoll_wait(epoll_fd_, &*events_.begin(), events_.size(), kEpollWaitTime);

        if(event_count < 0) perror("epoll wait error");
        std::vector<SPChannel> req_data = getEventsRequest(event_count);
        if(req_data.size() > 0) return req_data;
    }
}

void Epoll::handleExpired(){
    time_manager_.handleExpiredEvent();
}

std::vector<SPChannel> Epoll::getEventsRequest(int events_num){
    std::vector<SPChannel> req_data;
    for(int i=0; i<events_num; ++i){
        int fd = events_[i].data.fd;
        SPChannel cur_request = fd2chan_[fd];
        if(cur_request){
            cur_request->setRevents(events_[i].events);
            cur_request->setEvents(0);
            req_data.push_back(cur_request);
        }else{
            LOG << "SP cur_request is invaild";
        }
    }
    return req_data;
}

void Epoll::addTimer(SPChannel request_data, int timeout){
    using SPHttpData = shared_ptr<HttpData>;
    SPHttpData t = request_data->getHoleder();
    if(t){
        time_manager_.addTimer(t, timeout);
    }else{
        LOG << "timer add fail";
    }
}