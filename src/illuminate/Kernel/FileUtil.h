#ifndef FILE_UTIL__
#define FILE_UTIL__

#include <string>
#include "noncopyable.h"

class AppendFile : noncopyable{
    private:
    size_t write(const char* logline, size_t len);
    FILE *fp_;
    char buffer_[64*1024]; //即65536

    public:
    // 对于文件操作来说，不能隐式转换
    explicit AppendFile(std::string filename);
    ~AppendFile();

    void append(const char* logline, const size_t len);
    void flush();
}






#endif