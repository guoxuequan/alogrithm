#include <iostream>
#include <vector>
//#include <cstdlib>
//#include <deque>
#include <math.h>
#include "maxminkwindow.h"

int main(){
    MaxMinKWindow<double> mmk(10);
    mmk.set_nan(NAN);
    
    for (int i = 1; i<100;i++){
        mmk.add_new(i*0.1);
        std::cout<<mmk.size()<<"|"<<mmk.max_val()<<"|"<<mmk.min_val()<<std::endl;
        for (int j=0;j<10;j++){
            std::cout<<j<<"max|"<<mmk.maxk_val(j)<<std::endl;
            std::cout<<j<<"min|"<<mmk.mink_val(j)<<std::endl;
        }
        //mmk.add_new((-i));
        //std::cout<<mmk.size()<<"|"<<mmk.max_val()<<"|"<<mmk.min_val()<<std::endl;
    }
    return 0;
}
