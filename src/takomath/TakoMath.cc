#include "TakoMath.h"
#include <iostream>

TakoMath::TakoMath(/* args */)
{
    this->value = 0;
    std::cout<<"This is [[TakoMath.cc]] !!!"<<std::endl;    
}

TakoMath::TakoMath(int n)
{
    this->value = n;
    std::cout<<"This is [[TakoMath.cc]] !!!"<<std::endl;    
}

TakoMath::~TakoMath()
{

}

void TakoMath::test(){
    std::cout<<"This is [[TakoMath.cc]] [[function: test]] !!!"<<std::endl; 
}

