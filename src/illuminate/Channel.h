
#ifndef __CHANNEL__
#define __CHANNEL__

#include <sys/epoll.h>
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "Timer.h"

class EventLoop;
class HttpData;

class Channel
{
private:
    using CallBack = std::function<void()>;
    
    EventLoop *loop_;
    int fd_;
    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t last_events_;

    std::weak_ptr<HttpData> holder_;

    int parse_URI();
    int parse_Headers();
    int analysisRequest();

    CallBack read_handler_;
    CallBack write_handler_;
    CallBack error_handler_;
    CallBack conn_handler_;


public:
    Channel(EventLoop *loop);
    Channel(EventLoop* loop, int fd);
    ~Channel();
    int getFd();
    void setFd(int fd);

    using SPHttpData = std::shared_ptr<HttpData>;
    void setHolder(SPHttpData holder){
        holder_ = holder;
    }
    SPHttpData getHoleder(){
        SPHttpData ret(holder_.lock());
        return ret;
    }

    void setReadHandler(CallBack && readHandler){
        read_handler_ = readHandler;
    }
    void setWriteHandler(CallBack && writeHandler){
        write_handler_ = writeHandler;
    }
    void setErrorHandler(CallBack && errorHandler){
        error_handler_ = errorHandler;
    }
    void setConnHandler(CallBack && connHandler){
        conn_handler_ = connHandler;
    }
    void handleRead();
    void handleWrite();
    void handleError(int fd, int err_num, std::string short_msg);
    void handleConn();

    void handleEvents(){
        events_ = 0;
        if( (revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)){
            events_ = 0;
            return;
        }
        if(revents_ & EPOLLERR) {
            if (error_handler_) error_handler_();
            events_ = 0;
            return;
        } 
        if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLHUP)){
            handleRead();
        }
        if(revents_ & EPOLLOUT){
            handleWrite();
        }
        handleConn();
    }

    void setRevents(__uint32_t ev){
        revents_ = ev;
    }

    void setEvents(__uint32_t ev){
        events_ = ev;
    }

    __uint32_t &getEvents(){
        return events_;
    }

    bool EqualAndUpdateLastEvents(){
        bool ret = (last_events_ == events_);
        last_events_ = events_;
        return ret;
    }

    __uint32_t getLastEvents(){
        return last_events_;
    }

    using SPChannel = std::shared_ptr<Channel>;
};






#endif