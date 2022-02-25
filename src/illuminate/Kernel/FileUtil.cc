#include "FileUtil.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

AppendFile::AppendFile(std::string filename) : fp_(fopen(filename.c_str(), "a+")){
    setbuffer(fp_, buffer_, sizeof(buffer_));
}

// 最后要关闭文件句柄
AppendFile::~AppendFile(){
    fclose(fp_);
}

void AppendFile::append(const char* logline, const size_t len){
    size_t n = write(logline, len);
    ssize_t remain = len - n;
    while(remain > 0){
        ssize_t x = write(logline + n, remain);
        if(x == 0){
            int err = ferror(fp_);
            if(err) fprintf(stderr, "AppendFile::apend() failed! \n");
            break;
        }
        n += x;
        remain = len - n;
    }
}

void AppendFile::flush(){ fflush(fp_); }

size_t AppendFile::write(const char* logline, size_t len) {
    // 使用无锁写是因为对于日志的写入，是分离了前后端的
    // 后端IO慢，内存交换快，我们将前端产生的log放到缓冲区里
    // 所以这里我们使用非安全版本的写入加快速度
  return fwrite_unlocked(logline, 1, len, fp_);
}