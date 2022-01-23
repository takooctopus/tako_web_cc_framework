#define _APP_HTTP_CONTROLLERS_CONTROLLER

namespace App{
    namespace Http{
        namespace Controllers{
            class Controller{
            private:
                /* data */
            public:
                Controller(/* args */);
                ~Controller();
                virtual void Index() = 0;
            };
            
        }
    }
}