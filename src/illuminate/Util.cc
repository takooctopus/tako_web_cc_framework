#include "Util.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_BUFF = 4096;
ssize_t readn(int fd, void *buff, size_t n){
    ssize_t nleft = n;
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    char *ptr = (char*) buff;
    while(nleft > 0){
        if((nread = read(fd, ptr, nleft)) < 0){
            if(errno == EINTR)
                nread = 0;
            else if (errno == EAGAIN){
                return read_sum;
            }else{
                return -1;
            }
        }else if (nread == 0) break;
        read_sum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return read_sum;
}

ssize_t readn(int fd, std::string &inBuffer){
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    while(true){
        char buff[MAX_BUFF]
    }
}

