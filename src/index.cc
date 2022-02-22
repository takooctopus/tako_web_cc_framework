#include "index.h"
#include <iostream>
#include "routes/Web.h"
#include "illuminate/EventLoop.h"
#include "illuminate/Server.h"

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

int main(){
    /*TODO: 读取配置文件并实例化 -> vector<string>
            转到后面的
    */
    //TODO: 创建子进程，关闭父进程，以在后台运行
    //TODO: 
    EventLoop e;
    e.class_name();
    Server s;
    s.class_name();
    std::cout << "main" << std::endl;
    return 0;
}