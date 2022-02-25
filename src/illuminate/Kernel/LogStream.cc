#include "LogStream.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <limits>

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

// template<typename T>
// concept ArithmeticType = std::is_arithmetic<T>::value;

/* @return size_t 即转换成字符串后长度*/
template<IntegralType T>
size_t covert(char buff[], T value) {
    T i = value;
    char* p = buff;

    do {
        int lsd = static_cast<int> (i % 10);
        i /= 10;
        *p++ = zero[lsd]; // 取余数依次存到buff[]中，并将负数字换成正数字
    }while(i != 0);
    if(value < 0){
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buff, p); //两个iterator之间反转
    return p - buff;
}

template <IntegralType T>
void LogStream::formatInteger(T v){
    if(buffer_.avail() >= kMaxNumericSize){
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

// 整形的适配
template <IntegralType T>
LogStream& LogStream::operator<<(T v) {
  formatInteger(v);
  return *this;
}

// 浮点的适配
LogStream& LogStream::operator<<(double v) {
  if (buffer_.avail() >= kMaxNumericSize) {
    int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
    buffer_.add(len);
  }
  return *this;
}

LogStream& LogStream::operator<<(long double v) {
  if (buffer_.avail() >= kMaxNumericSize) {
    int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12Lg", v);
    buffer_.add(len);
  }
  return *this;
}


