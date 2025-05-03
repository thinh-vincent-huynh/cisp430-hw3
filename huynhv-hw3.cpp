#include <iostream>
#include <fstream>
using namespace std;

//work just like a singly linked list
struct aqueue{
    int key;
    aqueue* next;
    aqueue(int _key, aqueue* _next){
        key = _key;
        next = _next;
    }
};

//enqueue at the end of queue
void enqueue(aqueue *queue, int key){
    while (queue->next){    // traverse through queue
        queue = queue->next;
    }
    queue->next = new aqueue(key, nullptr);
}

//move front to the next one in queue, delete current front,
//and return key of current front
int dequeue(aqueue *&queue){
    int _key = queue->key;
    aqueue *curr = queue;
    queue = queue->next;
    delete curr;
    return _key;
}

//a node struct is sufficient to model a tree
struct node{
    int key;
    int height;
    node* left;
    node* right;
    node(int _key, int _height, node* _left, node* _right){
        key = _key;
        height = _height;
        left = _left;
        right = _right;
    }
};

//shorten allocating new node
node* newNode(int key){
    return new node(key, 0, nullptr, nullptr);
}

//get the higher value between the two
int max(int left, int right){
    return (left > right) ? left : right;
}

//update height
int height(node* node){
    if (!node){ //parent node was a leaf node
        return -1;
    }
    return max(height(node->left), height(node->right)) + 1;
}

//get balance to see whether to rotate
int getBalance(node* node){
    return height(node->left) - height(node->right);
}

//x, y(right), z(left) --> y, x(left), z(right)
node *leftRotate(node *x){
    node *y = x->right, *z = y->left;
    //add more code to rotate to the left, update heights for x and y
    y->left   = x;
    x->right  = z;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

//x, y(left), z(right) --> y, x(right), z(left)
node *rightRotate(node *x){
    node *y = x->left, *z = y->right;
    //add more code to rotate to the right, update heights for x and y
    y->right  = x;
    x->left   = z;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

//This is not a recursive algorithm
node *rebalance(node *node){
    node->height = max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node); //node->left - node->right
    
    if (balance > 1){   //left heavy
        if (getBalance(node->left) < 0){    //inside
            node->left = leftRotate(node->left);
        }
        return rightRotate(node);
    } else if (balance < 1){    //right heavy
        if (getBalance(node->right) > 0){   //inside
            node->right = rightRotate(node->right);
        }
        return leftRotate(node);
    }
    return node;    //no need to rotate
}

//non-tail recursive algorithm because of rebalance
node* insert(node* node, int key)
{
    //recursive Code for inserting a node
    //When insert happens set height to 0 for the node
    if (node == nullptr){
        return newNode(key);
    }
    
    if (key < node->key){
        node->left = insert(node->left, key);
    }else{
        node->right = insert(node->right, key);
    }

    node = rebalance(node); //update heights and rebalance
    return node;
}

//print
int main(){
    ifstream in("input.txt");
    aqueue *currLevel = nullptr, *nextLevel = nullptr;
    node* tree;
    int key;

    in >> key;
}