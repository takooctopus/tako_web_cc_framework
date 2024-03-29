#include "index.h"
#include <iostream>
#include "routes/Web.h"
// #include "illuminate/EventLoop.h"
// #include "illuminate/Server.h"

// test相关引用
#include<ranges>
#include<vector>
#include<cstring>
#include "illuminate/Kernel/FileUtil.h"
#include <hiredis.h>

void footest(){
    std::cout<<"-----------------------------------------------"<<std::endl;
    std::cout<<"+    Hello World !!!"<<std::endl;
    std::cout<<"+    此处是整个程序入口"<<std::endl;    
    std::cout<<"-----------------------------------------------"<<std::endl;
    // 初始化web.cc就返回网页
    Web web;
    // 初始化api.cc就返回api json啥的

    std::cout<<"-----------------------------------------------"<<std::endl;
    std::cout<<"+    See ya !!!"<<std::endl;
    std::cout<<"+              "<<std::endl;    
    std::cout<<"-----------------------------------------------"<<std::endl;
    return ;
}

void test(){
    std::vector<int> ints{0,1,2,3,4,5};
    auto even = [](int i)
    {
        //std::cout<<"even("<<i<<")\n"; 
        return 0 == i % 2; 
    };
    auto square = [](int i) 
    {
        //std::cout<<"square("<<i<<")\n"; 
        return i * i; 
    };

    for (int i : ints | std::views::filter(even) | std::views::transform(square)) 
    {
        std::cout << i << "----------------\n";
    }
}

std::string getTest(){
    return "get test";
}



int main(){
    /*TODO: 读取配置文件并实例化 -> vector<string>
            转到后面的
    */
    //TODO: 创建子进程，关闭父进程，以在后台运行
    //TODO: 
    // EventLoop e;
    // e.class_name();
    // Server s;
    // s.class_name();
    int a = 0;
    int b = 8;
    std::vector<int> v{1,2,3,4,5,6,7,8};
    std::cout << "main" << std::endl;
    test();
    std::string inBuffer;
    char buff[128] = "123456";
    ssize_t nread = 5;
    inBuffer += std::string(buff, buff + nread);
    strcpy(buff, "abcdef");
    inBuffer += std::string(buff, buff + nread);
    a = 7;
    std::string filename = "/root/code/cpp/tako/test.log";
    AppendFile fd(filename);
    char buffer[100];
    char* p = buffer;
    std::string s = "abcdefg----------12345\n";
    strcpy(buffer, s.c_str());
    ssize_t n = strlen(buffer);
    fd.append(buffer, n);

    redisContext* c = redisConnect("127.0.0.1", 6379);
    if(c == nullptr || c->err){
        if(c) {
            printf("Error: %s\n", c->errstr);
            exit(-1);
        }
        else{
            printf("Can't allocate redis context\n");
            exit(-1);
        }
    }
    redisReply* reply = (redisReply *) redisCommand(c, "GET hello");
    printf("%s\n", reply->str);
    return 0;
}