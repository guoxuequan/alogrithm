/**
 * @file   maxmin_k.hh
 * @author  <ywei@ubiquant.com>
 * @date   Mon Jul 22 13:38:37 2013
 * 
 * @brief  A O(log(n))-time, O(n)-space algorithm for select k-th data from a window data maintain in a data flow.
 *         with red-black-tree.
 *
 * @merge  xguo,fix bug 1.T key 2.length k 3. DeleteNode (if z== nil)
 * @date   20140126
 */
#ifndef __MAXMINKWINDOW_H__
#define __MAXMINKWINDOW_H__
//Required library
// #include <list>
// #include <cstdlib>

enum Color{
    BLACK,
    RED
};

template <typename T>
class RBNode{

    public:
        T key;			/**< The data */
        int index;	    /**< The index of the data in the flow*/
        size_t size;	    /**< The size of the sub tree*/
        Color color;	/**< Color of this node */
        int left;	    /**< The pointer to the left son of this node */
        int right;      /**< The pointer to the right son of this node */
        int parent;	    /**< The pointer to the parent os this node */
};

template <typename T>
class MaxMinKWindow{
    public:
        //constructor length_k >= 1
        MaxMinKWindow(size_t length_k = 1){
            if (length_k <= 0){
                std::cout<<"Error:window must be>0"<<std::endl;
                return;
            }
            nil = new RBNode<T>;
            nil->index = -1;
            nil->color = BLACK;
            nil->size = 0;
            nil->key = (this->nil_key);
            root = -1;
            this->length_k = length_k;
            nodes.resize(length_k);
            index = 0;
        }

        //copy constructor
        MaxMinKWindow(const MaxMinKWindow& mmw):
            nil_key(mmw.nil_key),    
            index(mmw.index),
            length_k(mmw.length_k)
    {
        nil = new RBNode<T>;
        nil->color = BLACK;
        nil->size = 0;
        nil->key = (this->nil_key);
        root = -1;
        nodes.resize(length_k);
        for (int i = 0; i < length_k;i++){
            nodes[i] = mmw.nodes[i];
        }
    }

        MaxMinKWindow& operator=(const MaxMinKWindow &mmw)
        {
            nil_key = mmw.nil_key;
            index = mmw.index;
            length_k = mmw.length_k;
            nil = new RBNode<T>;
            nil->color = BLACK;
            nil->size = 0;
            nil->key = (this->nil_key);
            root = -1;
            nodes.resize(length_k);
            for (int i = 0; i < length_k;i++){
                nodes[i] = mmw.nodes[i];
            }
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
        void add_new(const T &x);

        //get constant reference of min-k value in datawindow, k >= 0 
        inline const T& mink_val(size_t k) const{
            k++;
            if( k < 1 || k > Node(root)->size){
                std::cout<<"Error:k > size"<<std::endl;
                return (nil_key);
            }
            return (Select(Node(root), k)->key);
        }

        //get constant reference of max-k value in datawindow, k >= 0 
        inline const T& maxk_val(size_t k) const{
            k++;
            if(k > Node(root)->size || k < 1){  
                std::cout<<"Error:k > size"<<std::endl;
                return (nil_key);
            }
            if(index < length_k) return (Select(Node(root), index - k+1)->key);
            else return (Select(Node(root), length_k - k + 1)->key);
        }

        inline int mink_pos(int k) const {
            k++;
            if(k < 1 || k > Node(root) -> size){
                std::cout<<"Error:k > size"<<std::endl;
                return -1;
            }else{
                if(index < length_k){
                    return (Select(Node(root), k)->index);
                }else{
                    return (Select(Node(root), k)->index - index + length_k);
                }
            }
        }

        inline int maxk_pos(int k) const {
            k++;
            if(k > Node(root)->size || k < 1){
                std::cout<<"Error:k > size"<<std::endl;
                return -1;
            }
            if(index < length_k){
                return (Select(Node(root), index - k+1)->index);
            }else{
                return (Select(Node(root), length_k - k + 1)->index -index + length_k);
            }
        }

        //get constant reference of min value in datawindow
        inline const T& min_val() const {return (Minimum(Node(root))->key);}

        //get constant reference of max value in datawindow
        inline const T& max_val() const {return (Maximum(Node(root))->key);}

        inline int min_pos() const {
            if(index < length_k){
                return (Minimum(Node(root))->index);
            }else{
                return (Minimum(Node(root))->index - (index -length_k));
            }
        }

        inline int max_pos() const { 
            if(index < length_k){
                return (Maximum(Node(root))->index);
            }else{
                return (Maximum(Node(root))->index) - index + length_k;
            }
        }

        inline const int size() {return Node(root)->size;}

    private:
        //@{ red-black tree methods.

        std::vector < RBNode<T> > nodes;
        //RBNode<T> nodes[1000];

        RBNode<T>* Successor(RBNode<T> *z);	

        void InsertNode(RBNode<T> *z); 

        void DeleteNode(RBNode<T> *z);

        inline RBNode<T>* Node(int index) const;
        inline RBNode<T>* Maximum(RBNode<T> *z) const; 
        inline RBNode<T>* Minimum(RBNode<T> *z) const;
        inline RBNode<T>* Select(RBNode<T>*x, size_t i) const; 

        int length_k;		
        int root;		/**< root of the tree */

        int index;		/**< The count of the flow after last clear() */

        RBNode<T> *nil;
        T nil_key;
        void LeftRotate(RBNode<T>* x);
        void RightRotate(RBNode<T>* x);
        void InsertFixup(RBNode<T>* z);
        void DeleteFixup(RBNode<T>* x);

        //@}
};


template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Node(int index) const{
    if (index == -1)
        return nil;
    else
        return (RBNode<T>*) &nodes[index % length_k];
}


template <typename T>
void MaxMinKWindow<T>::add_new(const T &x){
    RBNode<T> z;
    if(index >= length_k){
        DeleteNode(Node(index));
    }
    z.key = x;
    z.index = index;
    z.size = 1;
    nodes[index % length_k] = z;
    InsertNode(Node(index));
    index ++;
}

template <typename T>
MaxMinKWindow<T>::~MaxMinKWindow<T>(){
    delete nil;
}

template <typename T>
void MaxMinKWindow<T>::LeftRotate(RBNode<T>* x){
    RBNode<T> *y = Node(x->right);
    x -> right = y -> left;
    if(Node(y -> left) != nil) Node(y -> left) -> parent = x -> index;/// There was not if in the book, this will introduce an segment fault with 'input4.txt';
    y -> parent = x -> parent;
    if( Node(x -> parent) == nil ){
        root = y -> index;
    }else if( x == Node(Node(x -> parent) -> left)){
        Node(x -> parent) -> left = y -> index;
    }else{
        Node(x -> parent) -> right = y -> index;
    }
    y -> left = x -> index;
    x -> parent = y -> index;
    y -> size = x -> size;
    x -> size = Node(x -> left) -> size + Node(x -> right) ->size + 1;
}

template <typename T>
void MaxMinKWindow<T>::RightRotate(RBNode<T>* y){
    RBNode<T> *x = Node(y -> left);
    y -> left = x -> right;
    if(Node(x -> right) != nil) 
        Node(x -> right) -> parent = y -> index;
    x -> parent = y -> parent;
    if( Node(y -> parent) == nil ){
        root = x -> index;
    }else if( y == Node(Node(y -> parent) -> right)){
        Node(y -> parent) -> right = x -> index;
    }else{
        Node(y -> parent) -> left = x -> index;
    }
    x -> right = y -> index;
    y -> parent = x -> index;
    x -> size = y -> size;
    y -> size = Node(y -> right) -> size + Node(y -> left) -> size + 1;
}

template <typename T>
void MaxMinKWindow<T>::InsertNode(RBNode<T>* z){
    RBNode<T>* y = nil, *x = Node(root);
    while(x != nil){
        x -> size = x -> size + 1;
        y = x;
        if( z->key <  x->key){
            x = Node(x -> left);
        }else{
            x = Node(x -> right);
        }
    }
    z -> parent = y -> index;
    if(y == nil){
        root = z->index;
    }else if( z -> key < y->key){
        y -> left = z -> index;
    }else{
        y -> right = z -> index;
    }
    z->left = -1;
    z->right = -1;
    z->color = RED;
    z->size = 1;
    InsertFixup(z);
}

template <typename T>
void MaxMinKWindow<T>::InsertFixup(RBNode<T>* z){
    RBNode<T> *y;
    std::cout << z->parent << std::endl;
    while(Node(z->parent)->color == RED){
        if( z-> parent == Node(Node(z -> parent) -> parent) -> left){
            y = Node(Node(Node(z -> parent) -> parent) ->right);
            if(y -> color == RED){
                Node(z -> parent) -> color = BLACK;
                y -> color = BLACK;
                Node(Node(z -> parent) -> parent) -> color = RED;
                z = Node(Node(z -> parent) -> parent);
            }else{
                if( z == Node(Node(z -> parent)-> right)){
                    z = Node(z -> parent);
                    LeftRotate(z);
                }
                Node(z -> parent) -> color = BLACK;
                Node(Node(z -> parent) -> parent) -> color = RED;
                RightRotate(Node(Node(z -> parent) -> parent));
            }
        }else{
            y = Node(Node(Node(z -> parent) -> parent)->left);
            if(y -> color == RED){
                Node(z -> parent) -> color = BLACK;
                y -> color = BLACK;
                Node(Node(z -> parent) -> parent) -> color = RED;
                z = Node(Node(z -> parent) -> parent);
            }else{
                if( z == Node(Node(z -> parent) -> left)){
                    z = Node(z -> parent);
                    RightRotate(z);
                }
                Node(z -> parent) -> color = BLACK;
                Node(Node(z -> parent) -> parent) -> color = RED;
                LeftRotate(Node(Node(z -> parent) -> parent));
            }
        }
    }
    Node(root) -> color = BLACK;
}

template <typename T>
void MaxMinKWindow<T>::DeleteNode(RBNode<T> *z){
    RBNode<T> *y, *x;
    if( z->left == -1 || z->right == -1){
        y = z;
    }else{
        y = Successor(z);
    }
    if( Node(y->left) != nil){
        x = Node(y->left);
    }else{
        x = Node(y->right);
    }
    x->parent = y->parent;

    if( Node(y->parent) == nil){
        root = x->index;
    }else if( y  == Node(Node(y->parent)->left)){
        Node(y->parent)->left = x->index;
    }else{
        Node(y->parent)->right = x->index;
    }

    if( y != z){
        z->key = y->key;
        ///copy y's satellite data to z.
        z -> index = y -> index;
        nodes[ y->index % (length_k)] = *z;
//        y = z;
    }

    z = y; 
    while(z != Node(root)){
        z = Node(z -> parent);
        if (z == nil)
            break;
        z -> size = z -> size - 1;
    }
    if( y -> color == BLACK){
        DeleteFixup(x);
    }
}

template <typename T>
void MaxMinKWindow<T>::DeleteFixup(RBNode<T>* x){
    RBNode<T>* w;

    while( x != Node(root) && x->color == BLACK){
        if(x == Node(Node(x -> parent) -> left)){
            w = Node(Node(x -> parent) -> right);
            if( w -> color == RED){
                w -> color = BLACK;
                Node(x -> parent) -> color = RED;
                LeftRotate( Node(x -> parent));
                w = Node(Node(x -> parent) -> right);
            }
            if( Node(w -> left) -> color == BLACK && Node(w -> right) -> color == BLACK){
                w -> color = RED;
                x = Node(x -> parent);
            }else{
                if( Node(w -> right) -> color == BLACK){
                    Node(w -> left) -> color = BLACK;
                    w -> color = RED;
                    RightRotate(w);
                    w = Node(Node(x -> parent) -> right);
                }
                w -> color = Node(x -> parent) -> color;
                Node(x -> parent) -> color = BLACK;
                Node(w -> right) -> color = BLACK;
                LeftRotate(Node(x -> parent));
                x = Node(root);
            }
        }else{
            w = Node(Node(x -> parent) -> left);
            if( w -> color == RED){
                w -> color = BLACK;
                Node(x -> parent) -> color = RED;
                RightRotate( Node(x -> parent));
                w = Node(Node(x -> parent) -> left);
            }
            if( Node(w -> right) -> color == BLACK && Node(w -> left) -> color == BLACK){
                w -> color = RED;
                x = Node(x -> parent);
            }else{
                if( Node(w -> left) -> color == BLACK){
                    Node(w -> right) -> color = BLACK;
                    w -> color = RED;
                    LeftRotate(w);
                    w = Node(Node(x -> parent) -> left);
                }
                w -> color = Node(x -> parent) -> color;
                Node(x -> parent) -> color = BLACK;
                Node(w -> left) -> color = BLACK;
                RightRotate(Node(x -> parent));
                x = Node(root);
            }

        }
    }
    x -> color = BLACK;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Select(RBNode<T>* x, size_t i) const{
    int r = Node(x ->left) -> size + 1;
    if(i > Node(root) -> size){
        return nil;
    }
    if( i == r){
        return x;
    }else if(i < r){
        return Select( Node(x -> left), i);
    }else{
        return Select( Node(x -> right), i - r);
    }
}

template <typename T>
RBNode<T>* MaxMinKWindow<T>::Successor(RBNode<T> *x){
    if(x->right != -1){
        return Minimum(Node(x->right));
    }
    RBNode<T>* y = Node(x->parent);
    while( y != nil && x == Node(y->right)){
        x = y;
        y = Node(y->parent);
    }
    return y;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Minimum(RBNode<T> *x) const{
    while(x->left != -1){
        x = Node(x->left);
    }
    return x;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Maximum(RBNode<T> *x) const{
    while(x->right != -1){
        x = Node(x->right);
    }
    return x;
}

template <typename T>
void MaxMinKWindow<T>::clear(){
    nil -> color = BLACK;
    nil -> size = 0;
    root = -1;
    index = 0;
}

template <typename T>
void MaxMinKWindow<T>::set_nan(T nan){
    nil_key = nan;
}

#endif // __MAXMINKWINDOW_H__

