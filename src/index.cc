#include "index.h"
#include <iostream>
#include "routes/Web.h"

int main(){
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
    return 0;
}