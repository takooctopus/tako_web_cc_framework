
#ifndef CHANNEL__
#define CHANNEL__

#include <sys/epoll.h>
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "Timer.h"

class EventLoop;
class HttpData;

/*
    enum EPOLL_EVENTS
    {
        EPOLLIN = 0x001,    表示输入事件；
        EPOLLPRI = 0x002,   紧急数据事件；
        EPOLLOUT = 0x004,   输出事件；
        EPOLLRDNORM = 0x040,
        EPOLLRDBAND = 0x080,
        EPOLLWRNORM = 0x100,
        EPOLLWRBAND = 0x200,
        EPOLLMSG = 0x400,
        EPOLLERR = 0x008,   对应的文件描述符有error；
        EPOLLHUP = 0x010,
        EPOLLRDHUP = 0x2000,
        EPOLLEXCLUSIVE = 1u << 28,
        EPOLLWAKEUP = 1u << 29,
        EPOLLONESHOT = 1u << 30,
        EPOLLET = 1u << 31  事件为边沿触发方式（默认为水平触发方式）;
    };

    我们查看epoll的事件，最多用了32位，使用一个uint32的位来表示每一个事件。

    epoll事件有两种模型，边沿触发：edge-triggered (ET)， 水平触发：level-triggered (LT)

    水平触发(level-triggered):
    socket接收缓冲区不为空 有数据可读 读事件一直触发
    socket发送缓冲区不满 可以继续写入数据 写事件一直触发

    边沿触发(edge-triggered):
    socket的接收缓冲区状态变化时触发读事件，即空的接收缓冲区刚接收到数据时触发读事件
    socket的发送缓冲区状态变化时触发写事件，即满的缓冲区刚空出空间时触发读事件
    边沿触发仅触发一次，水平触发会一直触发。

        ---- 在这里我们使用边沿触发方式
*/



class Channel
{
private:
    // channel采用回调
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