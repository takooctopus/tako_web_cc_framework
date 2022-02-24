#ifndef __EPOLL__
#define __EPOLL__
#include <sys/epoll.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Channel.h"
#include "HttpData.h"
#include "Timer.h"

/*

    typedef union epoll_data
    {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
    } epoll_data_t;

    struct epoll_event
    {
    uint32_t events;	
    epoll_data_t data;	
    } __EPOLL_PACKED;

    注册所感兴趣的事件和回传所发生待处理的事件
        
        EPOLLIN：表示对应的文件描述符可以读；
        EPOLLOUT：表示对应的文件描述符可以写；
        EPOLLPRI：表示对应的文件描述符有紧急的数可读；
        
        EPOLLERR：表示对应的文件描述符发生错误；
        EPOLLHUP：表示对应的文件描述符被挂断；
        EPOLLET：ET的epoll工作模式；

    extern int epoll_create (int __size) __THROW;
        该函数生成一个epoll专用的文件描述符，其中的参数是指定生成描述符的最大范围；


    extern int epoll_ctl (int __epfd, int __op, int __fd,
		      struct epoll_event *__event) __THROW;
        用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。
        
        ret -> 0/-1
        - epfd：由epoll_create生成的epoll专用的文件描述符；
        - op：要进行的操作，EPOLL_CTL_ADD注册、EPOLL_CTL_MOD修改、EPOLL_CTL_DEL删除；
        - fd：关联的文件描述符；
        - event：指向epoll_event的指针

    extern int epoll_wait (int __epfd, struct epoll_event *__events,
		       int __maxevents, int __timeout);
        该函数用于轮询I/O事件的发生。

        ret -> 0/-1
        - epfd：由epoll_create生成的epoll专用的文件描述符；
        - epoll_event：用于回传代处理事件的数组；
        - maxevents：每次能处理的事件数；
        - timeout：等待I/O事件发生的超时值；
*/

class Epoll
{
private:
    static const int MAXFDS = 100000;
    int epoll_fd_;
    std::vector<epoll_event> events_;
    std::shared_ptr<Channel> fd2chan_[MAXFDS];
    std::shared_ptr<HttpData> fd2http_[MAXFDS];
    TimerManager time_manager_;
public:
    using SPChannel = std::shared_ptr<Channel>;
    Epoll();
    ~Epoll();
    void epoll_add(SPChannel request, int timeout);
    void epoll_mod(SPChannel request, int timeout);
    void epoll_del(SPChannel request);
    std::vector<SPChannel> poll();
    std::vector<SPChannel> getEventsRequest(int events_num);
    void addTimer(SPChannel request, int timeout);
    int getEpollFd() {return epoll_fd_;}
    void handleExpired();
};


#endif