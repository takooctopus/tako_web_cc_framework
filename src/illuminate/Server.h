#ifndef __SERVER__
#define __SERVER__
#include<string>
#include<iostream>
class Server
{
private:
    /* data */
    const std::string class_name_ = "Server";
    int threadNum = 4;
    int port = 8080;
    std::string logPath = "/tmp/log/WebServer.log";
public:
    Server(/* args */);
    ~Server();
    void class_name(){
        std::cout << class_name_ << std::endl;
    };
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}
#endif