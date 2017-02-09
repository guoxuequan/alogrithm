/**
 * @file   maxmin_k.hh
 * @author  <xguo@ubiquant.com>
 * 
 * @brief  easy for check point 
 *
 * @date   20140620
 */
#ifndef __MAXMINKWINDOW_H__
#define __MAXMINKWINDOW_H__
//Required library
// #include <vector>
// #include <cstdlib>
//#include <deque>
//#include <algorithm>
//#include "Usim_archvier.h"

//this class is used for sorting only. T should be comparable using <
template <typename T>
class Node {
    public:
        Node() :data(0),index(0){}
        T data;
        int index;
        Node& operator=(const Node &node)
        {
            data = node.data;
            index = node.index;
            return (*this);
        }
};

template <typename T>
class MaxMinKWindow{
    public:
        //USIM_CHECKPOINT_SUPPORT(data * max_size * sorted_data)
        //constructor 
        MaxMinKWindow(size_t ws)
        : data(0),
          max_size(ws),
          sorted_data(0)
        {}

        MaxMinKWindow(const MaxMinKWindow &mmw)
        : data(mmw.data),
          max_size(mmw.max_size),
          sorted_data(mmw.sorted_data)
        {}

        MaxMinKWindow& operator=(const MaxMinKWindow &mmw)
        {
            data = mmw.data;
            max_size = mmw.max_size;
            sorted_data = mmw.sorted_data;
            return (*this);
        }

        ~MaxMinKWindow();

        /** 
         * clear all data. 
         * 
         */
        void clear();

        /** 
         * define nan(not a number).
         * 
         * @param nan, define the value of nan as "not a number"
         * 
         * @return 
         */
        void set_nan(T nan);

        /**
         *  main operation of MaxMinKWindow
         *  1.insert a data into datawindow.
         *  2.remove oldest data if datawindow is full.
         *  3.maintain the tree depth of the red-black-tree.
         *
         * @param x 
         */
        void add_new(const T x);

        //get constant reference of min-k value in datawindow, k >=0 
        inline const T& mink_val(size_t k) const{
            int length = data.size();
            if( k < 0 || k >= length){
                std::cout<<"Error:k of "<<k<<" > size of "<<length<<std::endl;
                return nil_key;
            }
            return sorted_data[k].data;
        }

        //get constant reference of max-k value in datawindow, k >= 1 
        inline const T& maxk_val(size_t k) const{
            int length = data.size();
            if(k >= length || k < 0){  
                std::cout<<"Error:k of "<<k<<" > size of "<<length<<std::endl;
                return nil_key;
            }
            return sorted_data[length-k-1].data;
        }

        inline int mink_pos(int k) const {
            int length = data.size();
            if(k < 0 || k >= length){
                std::cout<<"Error:k of "<<k+1<<" > size of "<<length<<std::endl;
                return -1;
            }else{
                return sorted_data[k].index;
            }
        }

        inline int maxk_pos(int k) const {
            int length = data.size();
            if(k >= length || k < 0){
                std::cout<<"Error:k of "<<k<<" > size of "<<length<<std::endl;
                return -1;
            }
            return sorted_data[length-k-1].index;
        }

        //get constant reference of min value in datawindow
        inline const T& min_val() const {return sorted_data[0];}

        //get constant reference of max value in datawindow
        inline const T& max_val() const {return sorted_data[sorted_data.size()-1];}

        inline int min_pos() const {
            return sorted_data[0].index;
        }

        inline int max_pos() const { 
            int length = data.size();
            return sorted_data[length-1].index;
        }

        inline const int size() {return data.size();}

    private:
        std::deque<T> data;
        size_t max_size;
        std::deque < Node<T> > sorted_data; 
        T nil_key;
};

template <typename T>
MaxMinKWindow<T>::~MaxMinKWindow<T>(){
}


template <typename T>
void MaxMinKWindow<T>::clear(){
    data.clear();
    sorted_data.clear();
}


template <typename T>
void MaxMinKWindow<T>::add_new(const T x){
    data.push_back(x);
    if(data.size() > max_size){
        data.pop_front();
    }
    Node<T> node; //= new Node();
    Node<T> tmp_node;// = new Node();
    node.data = x;
    node.index = data.size()-1;
	size_t n = sorted_data.size();
    if (n == max_size){
        int zero_index = -1;
	    for (size_t i = 0 ; i < n ; i++) {
            sorted_data[i].index--;
            if (sorted_data[i].index < 0)
                zero_index = i; 
	    }
        if (x > sorted_data[zero_index].data){
            sorted_data[zero_index] = node;
            for (size_t i = zero_index ;i < n-1; i++){
                if (sorted_data[i].data>sorted_data[i+1].data){
                    tmp_node = sorted_data[i];
                    sorted_data[i] = sorted_data[i+1];
                    sorted_data[i+1] = tmp_node;
                }else
                    break;
            }
        }else if (x < sorted_data[zero_index].data){
            sorted_data[zero_index] = node;
            for (size_t i = zero_index ;i > 0; i--){
                if (sorted_data[i].data<sorted_data[i-1].data){
                    tmp_node = sorted_data[i];
                    sorted_data[i] = sorted_data[i-1]; sorted_data[i-1] = tmp_node; }else
                    break;
            }
        }else{
            sorted_data[zero_index] = node;
        }
    }else{
        sorted_data.push_back(node); 
        n++;
        for (size_t i=1; i < n;i++){
            if (sorted_data[n-i].data<sorted_data[n-i-1].data){
                tmp_node = sorted_data[n-i];
                sorted_data[n-i] = sorted_data[n-i-1];
                sorted_data[n-i-1] = tmp_node;
            }else
                break;
        }
    }
}

template <typename T>
void MaxMinKWindow<T>::set_nan(T nan){
    nil_key = nan;
}

#endif // __MAXMINKWINDOW_H__

