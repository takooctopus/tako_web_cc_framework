#include "Util.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_BUFF = 4096;
/*
    采用unistd中c函数read()
    ssize_t read(int fd, void * buf, size_t count);
    参数fd所指定的文件传送count个到buf所指定的内存中
    - count == 0 : 没作用并返回0
    - 返回0 ： 到达文件末尾或者没有可以读取的数据
    -- 所以需要将nread与read()返回的值做比较
    -- 有错误时返回-1， 错误存入errno中
    --- EINTR 此调用被信号所中断.
    --- EAGAIN 当使用不可阻断I/O 时(O_NONBLOCK), 若无数据可读取则返回此值.
    --- EBADF 参数fd 非有效的文件描述词, 或该文件已关闭.
*/
ssize_t readn(int fd, void *buff, size_t n){
    ssize_t nleft = n;
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    char *ptr = (char*) buff;
    while(nleft > 0){
        //有错误时的错误处理逻辑
        if((nread = read(fd, ptr, nleft)) < 0){ 
            if(errno == EINTR)
                //被中断，这次没读上
                nread = 0;
            else if (errno == EAGAIN){
                // 读取完毕，返回读取字节总数
                return read_sum;
            }else{
                perror("read error");
                return -1;
            }
        }else if (nread == 0) break;
        //已经读取总数
        read_sum += nread;
        // 剩余需要读取数目减少
        nleft -= nread;
        ptr += nread;
    }
    return read_sum;
}

ssize_t readn(int fd, std::string &inBuffer, bool &zero){
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    while(true){
        char buff[MAX_BUFF];
        if((nread = read(fd, buff, MAX_BUFF)) < 0){
            if(errno == EINTR)
                continue;
            else{
                if(errno == EAGAIN){
                    return read_sum;
                } else {
                    perror("read error");
                    return -1;
                }
            }
        }else if (nread == 0){
            zero = true;
            break;
        }
        read_sum += nread;
        // 这个右值是构造的字符串，即从buff中头到缓存结束位置
        inBuffer += std::string(buff, buff + nread);
    }
    return read_sum;
}

ssize_t readn(int fd, std::string &inBuffer) {
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    while (true){
        char buff[MAX_BUFF];
        if((nread = read(fd, buff, MAX_BUFF)) < 0){
            if(errno == EINTR)
                continue;
            else if(errno == EAGAIN){
                return read_sum;
            }else {
                perror("read error");
            }
        }else if(nread == 0){
            break;
        }
        read_sum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return read_sum;
}

ssize_t writen(int fd, void *buff, size_t n){
    ssize_t nleft = n;
    ssize_t nwrittern = 0;
    ssize_t write_sum = 0;
    char* ptr = (char*) buff;
    while(nleft > 0){
        if(nwrittern = write(fd, ptr, nleft) < 0){
            if(errno == EINTR){
                nwrittern = 0;
                continue;
            }else if(errno == EAGAIN){
                return write_sum;
            }else return -1;
        }else{
            //等于0就相当于弄完了，大于0就继续循环，其实也没区别
        }
        write_sum += nwrittern;
        nleft -= nwrittern;
        ptr += nwrittern;
    }
    return write_sum;
}

ssize_t written(int fd, std::string &outBuffer){
    ssize_t nleft = outBuffer.size();
    ssize_t nwritten = 0;
    ssize_t write_sum = 0;
    const char *ptr = outBuffer.c_str();
    while(nleft > 0){
        if(nwritten = write(fd, ptr, nleft) < 0){
            if (errno == EINTR){
                nwritten = 0;
                continue;
            }else if (errno == EAGAIN){
                return write_sum;
            }else return -1;
        }else{
            //nwrittern等于0就相当于弄完了，大于0就继续循环，其实也没区别
        }
        write_sum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    //TO_QUESTION: 怎么这里还能没输出完就退出的啊
    if(write_sum == static_cast<int>(outBuffer.size()))
        outBuffer.clear();
    else 
        outBuffer = outBuffer.substr(write_sum);
    return write_sum;
}

/*
    考虑信号处理函数原型必须为 void func (int) 或者以下两种
    - SIG_IGN : 忽略信号
    - SIG_DFL : 恢复信号的默认行为

    int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);

    对于结构体 sigaction:
        - void (*) (int) sa_handler：处理函数指针，相当于signal函数的func参数。
        - sigset_t sa_mask： 指定一个。信号集，在调用sa_handler所指向的信号处理函数之前，该信号集将被加入到进程的信号屏蔽字中。信号屏蔽字是指当前被阻塞的一组信号，它们不能被当前进程接收到。
        - int sa_flags：信号处理修改器;
    
    int kill(pid_t pid, int sig);
        - 失败返回-1
        - 给定的信号无效（errno = EINVAL)
        - 发送权限不够( errno = EPERM ）
        - 目标进程不存在( errno = ESRCH )
    
    unsigned int alarm(unsigned int seconds);

    信号：
        - SIGPIPE:
            如果一个 socket 在接收到了 RST packet 之后，程序仍然向这个 socket 写入数据，那么就会产生SIGPIPE信号。
*/
void handle_for_sigpipe(){
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    //这边新的处理函数默认是一个不动如山的状况
    if(sigaction(SIGPIPE, &act, NULL)) return;
}

/*
    fcntl() 文件描述词来操作文件的特性
        如果出错，所有命令都返回－1，如果成功则返回某个其他值。
        - 复制一个现有的描述符（cmd=F_DUPFD）.
        - 获得／设置文件描述符标记(cmd=F_GETFD或F_SETFD).
        - 获得／设置文件状态标记(cmd=F_GETFL或F_SETFL).
        - 获得／设置异步I/O所有权(cmd=F_GETOWN或F_SETOWN).
        - 获得／设置记录锁(cmd=F_GETLK,F_SETLK或F_SETLKW).
        
        F_DUPFD   返回新的文件描述符
        F_GETFD   返回相应标志
        F_GETFL , F_GETOWN   返回一个正的进程ID或负的进程组ID
    
    对于 F_GETFL/F_SETFL 可更改的几个标志如下面的描述：
        - O_NONBLOCK: 非阻塞I/O，如果read(2)调用没有可读取的数据，或者如果write(2)操作将阻塞，则read或write调用将返回-1和EAGAIN错误
        - O_APPEND: 强制每次写(write)操作都添加在文件大的末尾，相当于open(2)的O_APPEND标志
        - O_DIRECT: 最小化或去掉reading和writing的缓存影响。系统将企图避免缓存你的读或写的数据。如果不能够避免缓存，那么它将最小化已经被缓存了的数据造成的影响。如果这个标志用的不够好，将大大的降低性能
        - O_ASYNC: 当I/O可用的时候，允许SIGIO信号发送到进程组，例如：当有数据可以读的时候
*/

int setSocketNoBlocking(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1) return -1;
    flag |= O_NONBLOCK; // 设置成非阻塞IO
    if(fcntl(fd, F_SETFL, flag) == -1) return -1;
    return 0;
}

/*
    extern int setsockopt (int __fd, int __level, int __optname,
		       const void *__optval, socklen_t __optlen) __THROW;

    __level 想要在套接字级别上设置选项，就必须设置
        - SOL_SOCKET
    
    __optname   __optval选项:
        SO_BROADCAST 	BOOL	允许套接口传送广播信息。
        SO_DEBUG 	BOOL	记录调试信息。
        SO_DONTLINER 	BOOL	不要因为数据未发送就阻塞关闭操作。设置本选项相当于将SO_LINGER的l_onoff元素置为零。
        SO_DONTROUTE 	BOOL 	禁止选径；直接传送。
        SO_KEEPALIVE BOOL	 	发送“保持活动”包。
        SO_LINGER  	struct linger FAR*	如关闭时有未发送数据，则逗留。
        SO_OOBINLINE 	BOOL	在常规数据流中接收带外数据。
        SO_RCVBUF 	int 	为接收确定缓冲区大小。
        SO_REUSEADDR 	BOOL	允许套接口和一个已在使用中的地址捆绑（参见bind()）。
        SO_SNDBUF 	int	指定发送缓冲区大小。
        TCP_NODELAY 	BOOL 	禁止发送合并的Nagle算法。
        
    若无错误发生，setsockopt()返回0。否则的话，返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()获取相应错误代码。
        WSANOTINITIALISED：在使用此API之前应首先成功地调用WSAStartup()。
        WSAENETDOWN：WINDOWS套接口实现检测到网络子系统失效。
        WSAEFAULT：optval不是进程地址空间中的一个有效部分。
        WSAEINPROGRESS：一个阻塞的WINDOWS套接口调用正在运行中。
        WSAEINVAL：level值非法，或optval中的信息非法。
        WSAENETRESET：当SO_KEEPALIVE设置后连接超时。
        WSAENOPROTOOPT：未知或不支持选项。其中，SOCK_STREAM类型的套接口不支持SO_BROADCAST选项，SOCK_DGRAM 类型的套接口不支持SO_DONTLINGER 、SO_KEEPALIVE、SO_LINGER和SO_OOBINLINE选项。
        WSAENOTCONN：当设置SO_KEEPALIVE后连接被复位。
        WSAENOTSOCK：描述字不是一个套接口。 
*/
void setSocketNoDelay(int fd){
    bool enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (bool*)enable , sizeof(enable));
}

/*
    Linux下tcp连接断开的时候调用close()函数，有优雅断开和强制断开两种方式。
    设置socket描述符一个linger结构体属性。
    三种断开方式：

    l_onoff = 0; l_linger忽略
        - close()立刻返回，底层会将未发送完的数据发送完成后再释放资源，即优雅退出。
    l_onoff != 0; l_linger = 0;
        - close()立刻返回，但不会发送未发送完成的数据，而是通过一个REST包强制的关闭socket描述符，即强制退出。
    l_onoff != 0; l_linger > 0;
        - close()不会立刻返回，内核会延迟一段时间，这个时间就由l_linger的值来决定。如果超时时间到达之前，发送完未发送的数据(包括FIN包)并得到另一端的确认，close()会返回正确，socket描述符优雅性退出。否则，close()会直接返回错误值，未发送数据丢失，socket描述符被强制性退出。需要注意的时，如果socket描述符被设置为非堵塞型，则close()会直接返回值。
*/
void setSocketNoLinger(int fd){
    struct linger f_linger;
    f_linger.l_onoff = 1;
    f_linger.l_linger = 30;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&f_linger,
             sizeof(f_linger));
}

/*
    为了优雅地断开连接，我们使用
    extern int shutdown (int __fd, int __how) __THROW;

    SHUT_RD：断开输入流。套接字无法接收数据（即使输入缓冲区收到数据也被抹去），无法调用输入相关函数。
    SHUT_WR：断开输出流。套接字无法发送数据，但如果输出缓冲区中还有未传输的数据，则将传递到目标主机。
    SHUT_RDWR：同时断开 I/O 流。相当于分两次调用 shutdown()，其中一次以 SHUT_RD 为参数，另一次以 SHUT_WR 为参数。
*/

void shutDownWR(int fd){
    shutdown(fd, SHUT_WR);
}

/*
    extern int socket (int __domain, int __type, int __protocol) __THROW;
    用于创建套接字

    __domain 地址族（Address Family），也就是 IP 地址类型。
        - AF_INET   AF_INET6
        - PF_INET   PF_INET6

    __type  
        - SOCK_STREAM（流格式套接字/面向连接的套接字）
        - SOCK_DGRAM（数据报套接字/无连接的套接字）
    
    __protocol 传输协议
        - IPPROTO_TCP 
        - IPPROTO_UDP 
        - ...
        - IPPROTO_IP = 0 自动推演出应该使用什么协议


    htonl()--"Host to Network Long"
    ntohl()--"Network to Host Long"
    htons()--"Host to Network Short"
    ntohs()--"Network to Host Short"

        - Little-Endian就是低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。
        - Big-Endian就是高位字节排放在内存的低地址端，低位字节排放在内存的高地址端。
        - 网络字节序：TCP/IP各层协议将字节序定义为Big-Endian，因此TCP/IP协议中使用的字节序通常称之为网络字节序。


    extern int listen (int __fd, int __n) __THROW;

        在调用bind之后-调用accept之前调用
        __n backlog
            - 进程处理一个一个连接请求的时候，可能还存在其它的连接请求。因为TCP连接是一个过程，所以可能存在一种半连接的状态，有时由于同时尝试连接的用户过多，使得服务器进程无法快速地完成连接请求。
            - 内核会在自己的进程空间里维护一个队列以跟踪这些完成的连接但服务器进程还没有接手处理或正在进行的连接。


    extern int accept (int __fd, __SOCKADDR_ARG __addr,
		   socklen_t *__restrict __addr_len);
        
        - 服务程序调用accept函数从处于监听状态的流套接字s的客户连接请求队列中取出排在最前的一个客户请求，并且创建一个新的套接字来与客户套接字创建连接通道，如果连接成功，就返回新创建的套接字的描述符，以后与客户套接字交换数据的是新创建的套接字
        - 如果失败就返回 INVALID_SOCKET
        - 操作系统利用第二个参数来返回新创建的套接字的地址结构
        - 操作系统利用第三个参数来返回新创建的套接字的地址结构的长度
*/

int socket_bind_listen(int port){
    if(port < 0 || port > 65535) return -1;

    // 创建socket(IPv4 + TCP)，返回监听描述符
    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1) return -1;

    // 消除bind时"Address already in use"错误
    bool enable = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
        close(listen_fd);
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 设置成0.0.0.0，服务器端口对上什么都行
    server_addr.sin_port = htons((unsigned short) port);
    if(bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        close(listen_fd);
        return -1;
    }

    //开始监听
    if(listen(listen_fd, 2048) == -1){
        close(listen_fd);
        return -1;
    }

    //无效监听
    if(listen_fd == -1){
        close(listen_fd);
        return -1;
    }
    
    return listen_fd;
}