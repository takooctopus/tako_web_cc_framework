#include "Timer.h"
#include <sys/time.h>
#include <unistd.h>
#include <queue>

TimerNode::TimerNode(std::shared_ptr<HttpData> request_data, int timeout)
    : deleted_(false), sp_http_data_(request_data) {
    struct timeval now;
    gettimeofday(&now, nullptr);
    //以毫秒计数
    expired_time_ = 
        (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;

}

TimerNode::~TimerNode(){
    if(sp_http_data_)
        sp_http_data_->handleClose();
}

TimerNode::TimerNode(TimerNode &tn) : 
    sp_http_data_(tn.sp_http_data_), expired_time_(0) {}

void TimerNode::update(int timeout){
    struct timeval now;
    gettimeofday(&now, nullptr);
    expired_time_ = 
        (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
    
}

bool TimerNode::isValid(){
    struct timeval now;
    gettimeofday(&now, nullptr);
    size_t temp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
    if(temp < expired_time_)
        return true;
    else {
        this->setDeleted();
        return false;
    }
}

void TimerNode::clearReq(){
    sp_http_data_.reset();
    this->setDeleted();
}

TimerManager::TimerManager(){}
TimerManager::~TimerManager(){}

void TimerManager::addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout){
    SPTimerNode new_node(new TimerNode(SPHttpData, timeout));
    timer_node_queue_.push(new_node);
    SPHttpData->linkTimer(new_node);
}

//拿一个普通队列
//只要队头被删除了或者过期了，那么就出队
//满是不可能满的，总会超时就是了
void TimerManager::handleExpiredEvent(){
    while(!timer_node_queue_.empty()){
        SPTimerNode sptimer_now = timer_node_queue_.top();
        if(sptimer_now->isDeleted()){
            timer_node_queue_.pop();
        }else if (sptimer_now->isValid() == false){
            timer_node_queue_.pop();
        }else
            break;
    }
}