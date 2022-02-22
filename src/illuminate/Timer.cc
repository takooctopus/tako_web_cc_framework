#include "Timer.h"
#include <sys/time.h>
#include <unistd.h>
#include <queue>

TimerNode::TimerNode(std::shared_ptr<HttpData> request_data, int timeout)
    : deleted_(false), spHttpData_(request_data) {
    struct timeval now;
    gettimeofday(&now, nullptr);
    //以毫秒计数

}