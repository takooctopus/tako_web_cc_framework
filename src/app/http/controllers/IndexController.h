#ifndef _APP_HTTP_CONTROLLERS_CONTROLLER
#include "Controller.h"
#endif

namespace App{
    namespace Http{
        namespace Controllers{
            class IndexController:public Controller {
            private:
                /* data */
            public:
                IndexController(/* args */);
                ~IndexController();
                void Index();
            };
            
            IndexController::IndexController(/* args */){
                this->Index();
            }
            
            IndexController::~IndexController(){
            }
            

        }
    }
}
