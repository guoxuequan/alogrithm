#include <iostream>
#include <vector>
//#include <cstdlib>
#include <deque>
#include <math.h>
#include <algorithm>
#include "maxminkwindow_rb.h"

int main(){
    MaxMinKWindow<double> mmk(5);
    //mmk.set_nan(NAN);
    
    mmk.add_new(4480.2);
    mmk.add_new(0);
    for (int i = 10; i>0;i--){
        mmk.add_new(i);
        //std::cout<<mmk.size()<<"|"<<mmk.maxk_val(2)<<"|"<<mmk.mink_val(2)<<std::endl;
        std::cout<<mmk.size()<<"|"<<mmk.maxk_val(1)<<std::endl;
        /*
        for (int j=0;j<10;j++){
            std::cout<<j<<"max|"<<mmk.maxk_val(j)<<std::endl;
            std::cout<<j<<"min|"<<mmk.mink_val(j)<<std::endl;
        }
        */
        //mmk.add_new((-i));
        //std::cout<<mmk.size()<<"|"<<mmk.max_val()<<"|"<<mmk.min_val()<<std::endl;
    }
    return 0;
}
