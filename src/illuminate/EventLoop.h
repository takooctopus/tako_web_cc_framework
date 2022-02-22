#ifndef __EVENT_LOOP__
#define __EVENT_LOOP__
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
/* TODO:
    #include "Epoll.h"
    #include "Util.h"
    #include "base/CurrentThread.h"
    #include "base/Logging.h"
    #include "base/Thread.h"
*/

class EventLoop
{
private:
    /* data */
    const std::string class_name_ = "EventLoop";
public:
    EventLoop(/* args */);
    ~EventLoop();
    void class_name(){
        std::cout << class_name_ << std::endl;
    }
};

EventLoop::EventLoop(/* args */)
{
}

EventLoop::~EventLoop()
{
}

#endif
