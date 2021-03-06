#include <cstdlib>
#include <iostream>

template <typename T>
MaxMinKWindow<T>::MaxMinKWindow(size_t length_k){
  nil = new RBNode<T>;
  nil -> color = BLACK;
  nil -> size = 0;
  nil -> key = &(this->nil_key);
  root = nil;
  this -> length_k = length_k;
  nodes.resize(length_k);
  pool = new RBNode<T>[length_k + 1];
  poolroot = pool;
  for(int i = 0; i < length_k; i ++){
    pool[i].right = &pool[i +1];
    pool[ i + 1].parent = &pool[i];
  }
  index = 0;
}
template <typename T>
void MaxMinKWindow<T>::copy_tree(RBNode<T> *node, const RBNode<T>*from, const MaxMinKWindow<T> &mmw){
  node -> key = from -> key;
  node -> index = from -> index;
  node -> size = from -> size;
  node -> color = from -> color;
  nodes[ node -> index % length_k] = node;

  if(from -> left != mmw.nil){
    RBNode<T>*left_node = pool_pop();
    left_node -> parent = node;
    node -> left = left_node;
    copy_tree(left_node, from->left, mmw);
  }else{
    node -> left = nil;
  }
  if(from -> right != mmw.nil){
    RBNode<T>*right_node = pool_pop();
    right_node -> parent = node;
    node -> right = right_node;
    copy_tree(right_node, from->right, mmw);
  }else{
    node -> right = nil;
  }

}

template <typename T>
MaxMinKWindow<T>::~MaxMinKWindow<T>(){
  delete nil;
  delete pool;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::pool_pop(){
  poolroot = poolroot -> right;
  return poolroot -> parent;
}

template <typename T>
inline void MaxMinKWindow<T>::pool_push(RBNode<T>* z){
  poolroot -> parent = z;
  z -> right = poolroot;
  poolroot = z;
}

template <typename T>
void MaxMinKWindow<T>::LeftRotate(RBNode<T>* x){
  RBNode<T> *y = x->right;
  x -> right = y -> left;
  if(y -> left != nil) y -> left -> parent = x;/// There was not if in the book, this will introduce an segment fault with 'input4.txt';
  y -> parent = x -> parent;
  if( x -> parent == nil ){
    root = y;
  }else if( x == x -> parent -> left){
    x -> parent -> left = y;
  }else{
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
  y -> size = x -> size;
  x -> size = x -> left -> size + x -> right ->size + 1;
  //cout<<"left rotate"<< x -> left ->size << x-> right ->size << y -> left ->size<<y -> right ->size<<endl;
}

template <typename T>
void MaxMinKWindow<T>::RightRotate(RBNode<T>* y){
  RBNode<T> *x = y->left;
  y -> left = x -> right;
  if(x -> right != nil) x -> right -> parent = y;
  x -> parent = y -> parent;
  if( y -> parent == nil ){
    root = x;
  }else if( y == y -> parent -> right){
    y -> parent -> right = x;
  }else{
    y -> parent -> left = x;
  }
  x -> right = y;
  y -> parent = x;
  x -> size = y -> size;
  y -> size = y -> right -> size + y -> left ->size + 1;
  //cout<<"right rotate"<< y -> left ->size << y-> right ->size << x -> left ->size<< x -> right ->size<<endl;
}

template <typename T>
void MaxMinKWindow<T>::InsertNode(RBNode<T>* z){
  RBNode<T> * y = nil, * x = root;
  while(x != nil){
    x -> size = x -> size + 1;
    y = x;
    if( *z -> key <  *x -> key){
      x = x -> left;
    }else{
      x = x -> right;
    }
  }
  z -> parent = y;
  if( y == nil ){
    root = z;
  }else if( *z -> key < *y -> key){
    y -> left = z;
  }else{
    y -> right = z;
  }
  z -> left = nil;
  z -> right = nil;
  z -> color = RED;
  z -> size = 1;
  InsertFixup(z);
}

template <typename T>
void MaxMinKWindow<T>::InsertFixup(RBNode<T>* z){
  RBNode<T> *y;
  while(z -> parent -> color == RED){
    if( z -> parent == z -> parent -> parent -> left){
      y = z -> parent -> parent ->right;
      if(y -> color == RED){
	z -> parent -> color = BLACK;
	y -> color = BLACK;
	z -> parent -> parent -> color = RED;
	z = z -> parent -> parent;
      }else{
	if( z == z -> parent -> right){
	  z = z -> parent;
	  LeftRotate(z);
	}
	z -> parent -> color = BLACK;
	z -> parent -> parent -> color = RED;
	RightRotate(z -> parent -> parent);
      }
    }else{
      y = z -> parent -> parent ->left;
      if(y -> color == RED){
	z -> parent -> color = BLACK;
	y -> color = BLACK;
	z -> parent -> parent -> color = RED;
	z = z -> parent -> parent;
      }else{
	if( z == z -> parent -> left){
	  z = z -> parent;
	  RightRotate(z);
	}
	z -> parent -> color = BLACK;
	z -> parent -> parent -> color = RED;
	LeftRotate(z -> parent -> parent);
      }
    }
  }
  root->color = BLACK;
}

template <typename T>
RBNode<T>* MaxMinKWindow<T>::DeleteNode(RBNode<T> *z){
  RBNode<T> *y, *x;
  if( z -> left == nil || z -> right == nil){
    y = z;
  }else{
    y = Successor(z);
  }
  if( y -> left != nil){
    x = y -> left;
  }else{
    x = y -> right;
  }
  x -> parent = y -> parent;

  if( y -> parent == nil){
    root = x;
  }else if( y  == y -> parent -> left){
    y -> parent -> left = x;
  }else{
    y -> parent -> right = x;
  }

  if( y != z){
    z -> key = y -> key;
    ///copy y's satellite data to z.
    z -> index = y -> index;
    nodes[ y -> index % (length_k)] = z;
  }

  z = y; 
  while(z != root){
    z = z -> parent;
    z -> size = z -> size - 1;
  }
  if( y -> color == BLACK){
    DeleteFixup(x);
  }
  return y;
}

template <typename T>
void MaxMinKWindow<T>::DeleteFixup(RBNode<T>* x){
  RBNode<T>* w;

  while( x != root && x -> color == BLACK){
    if(x == x -> parent -> left){
      w = x -> parent -> right;
      if( w -> color == RED){
	w -> color = BLACK;
	x -> parent -> color = RED;
	LeftRotate( x -> parent);
	w = x -> parent -> right;
      }
      if( w -> left -> color == BLACK && w -> right -> color == BLACK){
	w -> color = RED;
	x = x -> parent;
      }else{
	if( w -> right -> color == BLACK){
	  w -> left -> color = BLACK;
	  w -> color = RED;
	  RightRotate(w);
	  w = x -> parent -> right;
	}
	w -> color = x -> parent -> color;
	x -> parent -> color = BLACK;
	w -> right -> color = BLACK;
	LeftRotate(x -> parent);
	x = root;
      }
    }else{
      w = x -> parent -> left;
      if( w -> color == RED){
	w -> color = BLACK;
	x -> parent -> color = RED;
	RightRotate( x -> parent);
	w = x -> parent -> left;
      }
      if( w -> right -> color == BLACK && w -> left -> color == BLACK){
	w -> color = RED;
	x = x -> parent;
      }else{
	if( w -> left -> color == BLACK){
	  w -> right -> color = BLACK;
	  w -> color = RED;
	  LeftRotate(w);
	  w = x -> parent -> left;
	}
	
	w -> color = x -> parent -> color;
	x -> parent -> color = BLACK;
	w -> left -> color = BLACK;
	RightRotate(x -> parent);
	x = root;
      }

    }
  }
  x -> color = BLACK;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Select(RBNode<T>* x, size_t i) const{
  int r = x ->left -> size + 1;
  //std::cout <<i<<" " <<r << " "<<x->left->size <<" "<<x->right->size<<endl;
  if(i > root-> size){
    return nil;
  }
  if( i == r){
    return x;
  }else if(i < r){
    return Select( x -> left, i);
  }else{
    return Select( x -> right, i - r);
  }
}

template <typename T>
RBNode<T>* MaxMinKWindow<T>::Successor(RBNode<T> *x){
  if(x -> right != nil){
    return Minimum(x -> right);
  }
  RBNode<T> *y = x -> parent;
  while( y != nil && x == y -> right){
    x = y;
    y = y -> parent;
  }
  return y;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Minimum(RBNode<T> *x) const{
  while(x -> left != nil){
    x = x -> left;
  }
  return x;
}

template <typename T>
inline RBNode<T>* MaxMinKWindow<T>::Maximum(RBNode<T> *x) const{
  while(x -> right != nil){
    x = x -> right;
  }
  return x;
}

template <typename T>
void MaxMinKWindow<T>::clear(){
  index = 0;
  nil -> color = BLACK;
  nil -> size = 0;
  root = nil;
  poolroot = pool;
  for(int i = 0; i < length_k; i ++){
    pool[i].right = &pool[i +1];
    pool[ i + 1].parent = &pool[i];
  }
  poolsize = length_k +1;
  index = 0;
}

template <typename T>
void MaxMinKWindow<T>::add_new(const T &x){
  RBNode<T> * z, *y;
  if(index >= length_k){
    y = DeleteNode(nodes[ index%length_k]);
    pool_push(y);
  }
  z = pool_pop();
  z -> key = &x;
  z -> index = index;
  z -> size =1;
  nodes[index % length_k] = z;
  InsertNode(z);
  index ++;
}

template <typename T>
inline const T& MaxMinKWindow<T>::mink_val(size_t k) const{
  if( k < 1 || k > root->size){
    return *(nil->key);
  }
  return *(Select(root, k) -> key);
}

template <typename T>
inline const T& MaxMinKWindow<T>::maxk_val(size_t k) const{
  if(k > root->size || k < 1){
    return *(nil->key);
  }
  if(index < length_k){
    return *(Select(root, index - k+1) -> key);
  }else{
    return *(Select(root, length_k - k + 1) -> key);
  }
}

template <typename T>
inline const T& MaxMinKWindow<T>::min_val() const{
  return *(Minimum(root) -> key);
}

template <typename T>
inline const T& MaxMinKWindow<T>::max_val() const{
  return *(Maximum(root) -> key);
}

template <typename T>
void MaxMinKWindow<T>::set_nan(T nan){
  nil_key = nan;
}

