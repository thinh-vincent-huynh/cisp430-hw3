#include <iostream>
#include <fstream>
using namespace std;

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

//work just like a singly linked list
struct aqueue{
    node* tree;
    aqueue* next;
    aqueue(node* _tree, aqueue* _next){
        tree = _tree;
        next = _next;
    }
};

//enqueue at the end of queue
void enqueue(aqueue *&queue, node *tree){
    if (!queue){            //empty queue
        queue = new aqueue(tree, nullptr);
    }else{
        aqueue *curr = queue;
        while (curr->next){ //traverse through queue
            curr = curr->next;
        }
        curr->next = new aqueue(tree, nullptr);
    }
}

//move front to the next one in queue and delete current front
void dequeue(aqueue *&queue){
    aqueue *curr = queue;
    queue = queue->next;
    delete curr->tree;
    delete curr;
}

//shorten allocating new node
node *newNode(int key){
    return new node(key, 0, nullptr, nullptr);
}

//get the higher value between the two
int max(int left, int right){
    return (left > right) ? left : right;
}

//update height
int height(node *node){
    if (!node){ //parent node was a leaf node
        return -1;
    }
    return max(height(node->left), height(node->right)) + 1;
}

//get balance to see whether to rotate
int getBalance(node *node){
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
    }else if (balance < -1){    //right heavy
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

//process and print
int main(){
    //initializations
    ifstream in("input.txt");   //file
    int key;                    //insert
    node *tree = nullptr;       //tree
    aqueue *currLevel = nullptr, *nextLevel = nullptr;  //print

    //population from file and insert to tree
    while (in >> key){
        tree = insert(tree, key);
    }

    //print
    enqueue(currLevel, tree);
    while (currLevel){  //process if there are still childrens left
        while (currLevel){  //process all nodes in currLevel
            cout << currLevel->tree->key << "(" << currLevel->tree->height
                 << "," << getBalance(currLevel->tree) << ")\t";
            if (currLevel->tree->left){     //push left child into nextLevel
                enqueue(nextLevel, currLevel->tree->left);
            }
            if (currLevel->tree->right){    //push right child into nextLevel
                enqueue(nextLevel, currLevel->tree->right);
            }
            dequeue(currLevel);
        }
        currLevel = nextLevel;  //move onto nextLevel
        nextLevel = nullptr;    //empty it for the next level
        cout << endl;
    }
    return 0;
}