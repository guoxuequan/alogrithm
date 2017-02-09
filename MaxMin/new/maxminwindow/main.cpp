#include <iostream>
#include <vector>
#include <cstdlib>
#include <deque>
#include <math.h>
#include "maxminwindow.h"

int main(){
    MaxMinWindow<double> mmk(5);
    mmk.add_new(1.10);
    mmk.add_new(2.10);
    mmk.add_new(3.10);
    std::cout<<mmk.size()<<"|"<<mmk.max_val()<<"|"<<mmk.min_val()<<std::endl;
    mmk.add_new(NAN);
    mmk.add_new(5.10);
    mmk.add_new(NAN);
    mmk.add_new(NAN);
    mmk.add_new(8.10);
    std::cout<<mmk.size()<<"|"<<mmk.max_val()<<"|"<<mmk.min_val()<<std::endl;
    std::cout<<"hello"<<std::endl;
    return 0;
}
