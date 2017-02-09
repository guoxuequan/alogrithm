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
#include "Usim_archvier.h"

template <typename T>
class MaxMinKWindow{
    public:
        USIM_CHECKPOINT_SUPPORT(data * max_size * sorted_data * sorted_index)
        //constructor 
        MaxMinKWindow(size_t ws = 1)
        : data(0),
          max_size(ws),
          sorted_data(0),
          sorted_index(0)
        {}

        MaxMinKWindow(const MaxMinKWindow &mmw)
        : data(mmw.data),
          max_size(mmw.max_size),
          sorted_data(mmw.sorted_data),
          sorted_index(mmw.sorted_index)
        {}

        MaxMinKWindow& operator=(const MaxMinKWindow &mmw)
        {
            data = mmw.data;
            max_size = mmw.max_size;
            sorted_data = mmw.sorted_data;
            sorted_index = mmw.sorted_index;
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
                return T(NAN);
            }
            return sorted_data[k];
        }

        //get constant reference of max-k value in datawindow, k >= 1 
        inline const T& maxk_val(size_t k) const{
            int length = data.size();
            if(k >= length || k < 0){  
                std::cout<<"Error:k of "<<k<<" > size of "<<length<<std::endl;
                return T(NAN);
            }
            return sorted_data[length-k-1];
        }

        inline int mink_pos(int k) const {
            int length = data.size();
            if(k < 0 || k >= length){
                std::cout<<"Error:k of "<<k+1<<" > size of "<<length<<std::endl;
                return -1;
            }else{
                return sorted_index[k];
            }
        }

        inline int maxk_pos(int k) const {
            int length = data.size();
            if(k >= length || k < 0){
                std::cout<<"Error:k of "<<k<<" > size of "<<length<<std::endl;
                return -1;
            }
            return sorted_index[length-k-1];
        }

        //get constant reference of min value in datawindow
        inline const T& min_val() const {return sorted_data[0];}

        //get constant reference of max value in datawindow
        inline const T& max_val() const {return sorted_data[sorted_data.size()-1];}

        inline int min_pos() const {
            return sorted_index[0];
        }

        inline int max_pos() const { 
            int length = data.size();
            return sorted_index[length-1];
        }

        inline const int size() {return data.size();}

        std::deque<int> get_sorted_index(const std::deque<T> &data, bool ascending_order);

    private:
        std::deque<T> data;
        size_t max_size;
        std::deque <T> sorted_data; 
        std::deque <int> sorted_index;
};

template <typename T>
MaxMinKWindow<T>::~MaxMinKWindow<T>(){
}


template <typename T>
void MaxMinKWindow<T>::clear(){
    data.clear();
    sorted_data.clear();
    sorted_index.clear();
}

//this class is used for sorting only. T should be comparable using <
template <typename T>
class Node {
    public:
        Node() {}
        T data;
        int index;
        bool operator < (const Node<T> & a) const {return (data < a.data);}
};

template<typename T>
std::deque<int> MaxMinKWindow<T>::get_sorted_index(const std::deque<T> &data, bool ascending_order) {
	size_t n = data.size();
	std::vector<Node<T> > s(n);
	for (size_t i = 0 ; i < n ; i++) {
		s[i].data = data[i];
		s[i].index = i;
	}
	sort(s.begin(), s.end());
	std::deque<int> ret(n);
	if (ascending_order) {
		for (size_t i = 0 ; i < n ; i++) {
			ret[i] = s[i].index;
		}
	} else {
		for (size_t i = 0 ; i < n ; i++) {
			ret[n-1-i] = s[i].index;
		}
	}
	return ret;
}

template <typename T>
void MaxMinKWindow<T>::add_new(const T x){
    data.push_back(x);
    if(data.size() > max_size){
        data.pop_front();
    }
    sorted_data.clear();
	for (size_t i = 0 ; i < data.size() ; i++) {
        sorted_data.push_back(data[i]);
	}
    sort(sorted_data.begin(),sorted_data.end());
    sorted_index.clear();
    sorted_index = get_sorted_index(data,true);
}
template <typename T>
void MaxMinKWindow<T>::set_nan(T nan){
    //nil_key = nan;
}

#endif // __MAXMINKWINDOW_H__

