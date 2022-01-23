#include "Web.h"
#include <iostream>
#include "../app/http/controllers/IndexController.h"

// 本来应该有解析类的，但现在没有，那就先随便写个凑凑数
namespace TRoute{
    class Route{
        private:
            /* data */
        public:
            Route(/* args */);
            ~Route();
            void get(std::string s);
    };

    Route::Route(/* args */){
    }

    Route::~Route(){
    }

    void Route::get(std::string s){
        std::string s0 = "/";
        // std::cout<<s<<"\n"<<s0<<std::endl;
        if (s.compare(s0)==0)
        {
            std::cout<<"-----------------------------------------------"<<std::endl;
            std::cout<<"默认解析进入IndexController中"<<std::endl;
            App::Http::Controllers::IndexController res;
        }
        
    }
}

using namespace TRoute;

Web::Web(/* args */){
    this->resolve();
}

Web::~Web(){
}

void Web::resolve(){
    std::cout<<"++  WEB 解析函数，是我的话就直接在这里面写路由表了(现在只有最简单的一个/的解析)!!!"<<std::endl;
    std::cout<<"++  我们此时默认进入IndexController中."<<std::endl;
    Route route;
    route.get("/");
}