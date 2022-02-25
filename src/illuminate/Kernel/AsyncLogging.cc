#include "AsyncLogging.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include "LogFile.h"

/*
    实现异步日志记录
*/

AsyncLogging::AsyncLogging(std::string log_filename, int flush_interval):   flush_interval_(flush_interval),
running_(false),
basename_(log_filename),
thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
mutex_(),
cond_(mutex_),
// TODO: ?Buffer可能的类型失效
current_buffer_(new Buffer),
next_buffer_(new Buffer),
buffers_(),
latch_(1){
    assert(log_filename.size() > 1);
    current_buffer_->bzero(); //初始化
    next_buffer_->bzero();
    buffers_.reserve(16); //预留16个空间
}

void AsyncLogging::append(const char* logline, int len){
    MutexLockGuard lock(mutex_);
    if(current_buffer_->avail() > len){
        //如果还有剩余空间，那么向里面输出
        current_buffer_->append(logline, len);
    }else{
        //要是空间不足了，就把当前的SPBuffer智能指针推入vector，此时相当于vctor.back()就获取了当前的buffer。
        buffers_.push_back(current_buffer_);
        //再重置current_buffer_后，vector中持有原来的，而current_buffer_重置了。
        current_buffer_.reset(); 
        if(next_buffer_){
            //要是还有下一个，就直接move过去
            current_buffer_ = std::move(next_buffer_);
        }else{
            //没有就新建一个
            current_buffer_.reset(new Buffer);
        }
        current_buffer_->append(logline, len);
        cond_.notify();
    }
}

void AsyncLogging::threadFunc(){
    assert(running_ == true);
    latch_.countDown();
    LogFile output(basename_);
    SPBuffer neo_buffer_1(new Buffer);
    SPBuffer neo_buffer_2(new Buffer);
    neo_buffer_1->bzero();
    neo_buffer_2->bzero();
    BufferVector buffers_to_write;
    buffers_to_write.reserve(16);
    while(running_){
        assert(neo_buffer_1 && neo_buffer_1->length() == 0);
        assert(neo_buffer_2 && neo_buffer_2->length() == 0);
        assert(buffers_to_write.empty());

        do{
            MutexLockGuard lock(mutex_);
            if(buffers_.empty()){
                cond_.waitForSeconds(flush_interval_);
            }
            //同样将其放入vector中，并重置shared_ptr
            buffers_.push_back(current_buffer_);
            current_buffer_.reset();
            current_buffer_ = std::move(neo_buffer_1);

            //将buffers_中的交换到buffers_to_write中去方便下面输出
            buffers_to_write.swap(buffers_);

            if(!next_buffer_){
                next_buffer_ = std::move(neo_buffer_2);
            }
        }while(0);

        // 上面交换肯定vector中有一个
        assert(!buffers_to_write.empty());

        // TODO: ?
        if(buffers_to_write.size() > 25){
            buffers_to_write.erase(buffers_to_write.begin()+2, buffers_to_write.end());
        }

        // 依次添加到FileLog类中，每添加一次类中output.count_就加1，到次数后就调用FileUtil类实例file_去一次性写入
        for (size_t i = 0; i < buffers_to_write.size(); ++i) {
            output.append(buffers_to_write[i]->data(), buffers_to_write[i]->length());
        }
        
        if (buffers_to_write.size() > 2) {
            // drop non-bzero-ed buffers, avoid trashing
            buffers_to_write.resize(2);
        }

        if(!neo_buffer_1){
            assert(!buffers_to_write.empty());
            neo_buffer_1 = buffers_to_write.back();
            buffers_to_write.pop_back();
            neo_buffer_1->reset();
        }

        if(!neo_buffer_2){
            assert(!buffers_to_write.empty());
            neo_buffer_2 = buffers_to_write.back();
            buffers_to_write.pop_back();
            neo_buffer_2->reset();
        }
        // 每次while循环都会将 buffers_to_write 重置
        buffers_to_write.clear();
    }
    // 结束while循环后
    output.flush();

}