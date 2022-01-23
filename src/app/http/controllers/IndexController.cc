#include "IndexController.h"
#include <iostream>
namespace App{
    namespace Http{
        namespace Controllers{
            void IndexController::Index(){
                std::cout<<"+++ 这里是IndexController的index()方法. +++"<<std::endl;
            }
        }
    }
}
