#ifndef LOG_STREAM
#define LOG_STREAM
#include <assert.h>
#include <string.h>
#include <string>
#include "noncopyable.h"
#include <type_traits>


class AsyncLogging;

const int kSmallBuffer = 4000;
const int kLargeBUffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer : noncopyable {
    private:
    char data_[SIZE];
    const char* end() const {return data_ + sizeof(data_);}
    char* cur_;

    public:
    FixedBuffer(): cur_(data_){}
    ~FixedBuffer(){}

    int avail() const { return static_cast<int> (end_ - cur_); }
    const char* data() const { return data_; }
    int length() const { return static_cast<int> (end - data_); }
    char* current() {  return cur_; }
    void add(size_t len) { cur_ += len;}
    void reset() { cur_ = data_; }
    void bzero() { memset(data_, 0, sizeof(data_)); }
    void append(const char* buff, size_t len){
        if(avail() > static_cast<int>(len)){
            memcpy(cur_, buff, len);
            cur_ += len;
        }
    }
};

/* 限定为数学类型*/
template<typename T>
concept ArithmeticType = std::is_arithmetic<T>::value;

template<typename T>
concept IntegralType = std::is_integral<T>::value;


class LogStream : noncopyable {
    private:
    using Buffer = FixedBuffer<kSmallBuffer>;
    void staticCheck();

    template<IntegralType T>
    void formatInteger(T);

    Buffer buffer_;
    static const int kMaxNumericSize = 32;
    
    public:
    LogStream& operator<<(bool v){
        buffer_.append(v?"1":"0", 1);
        return *this;
    }

    LogStream& operator<<(const void*);

    LogStream& operator<<(float v){
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(long double);

    LogStream& operator<<(char v){
        buffer_.append(&v, 1);
        return *this;
    }

    LogStream& operator<<(const char* str){
        if(str)
            buffer_.append(str, strlen(str));
        else
            buffer_.append("(null)", 6);
        return *this;
    }

    LogStream& operator<<(const unsigned char* str){
        return operator<<(reinterpret_cast<const char*>(str));
    }

    template<IntegralType T>
    LogStream& operator<<(T);

    // LogStream& operator<<(short);
    // LogStream& operator<<(unsigned short);
    // LogStream& operator<<(int);
    // LogStream& operator<<(unsigned int);
    // LogStream& operator<<(long);
    // LogStream& operator<<(unsigned long);
    // LogStream& operator<<(long long);
    // LogStream& operator<<(unsigned long long);

    LogStream& operator<<(const std::string& v){
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char* data, int len) {buffer_.append(data, len);}
    
    const Buffer& buffer() { buffer_.reset(); }
}


#endif