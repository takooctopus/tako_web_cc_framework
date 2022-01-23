#ifndef _APP_HTTP_CONTROLLERS_CONTROLLER
#include "Controller.h"
#endif

namespace App{
    namespace Http{
        namespace Controllers{
            class NowcoderController:public Controller {
            private:
                /* data */
            public:
                NowcoderController(/* args */);
                ~NowcoderController();
                void Index();
                void GetQuestion();
            };
            
            NowcoderController::NowcoderController(/* args */){
                this->Index();
            }
            
            NowcoderController::~NowcoderController(){
            }
            

        }
    }
}
