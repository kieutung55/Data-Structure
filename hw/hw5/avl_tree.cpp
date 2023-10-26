// Tung Kieu
// COSC 2436
// Professor Rizk

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

//================ Auxiliary functions ==================
/*
return string surrounding by 'from' and 'to'
for example, str = "hello (world)", from = '(', to = ')', then result = "world"
*/
string getInner(string str, char from, char to) {
    string result;
    stringstream result_stream;
    bool is_in = false;
    size_t pos_from = str.find(from);
    size_t pos_to = str.find(to);
    if (pos_from == string::npos || pos_to == string::npos) {
        return str;
    }
    for (unsigned int i = 0; i <= str.length(); i++) {
        if (str[i] == to) {
            is_in = false;
            result = result_stream.str();
            break;
        }
        if (is_in) {
            result_stream << str[i];
        }
        if (str[i] == from) {
            is_in = true;
        }
    }
    return result;
}

/* Remove latest trailing characters ' ', '\0' , '\r' and '\n' */
string trim(string str) {
    string str_temp = str;
    for (int i = str.size() - 1; i >= 0; i--) {
        if (str[i] == '\r' || str[i] == '\n' || str[i] == '\0' || str[i] == ' ') {
            str_temp = str.substr(0, i);
        }
        else {
            break;
        }
    }
    return str_temp;
}

// ------ AVL Tree ---------

struct Node {
    int data;
    struct Node* left, * right;
    Node(int data_in) {
        data = data_in;
        left = nullptr;
        right = nullptr;
    }
};

class AVL_BST {
private:
    Node* root;

    // insert node on tree begining from 'node',  and return new root
    Node* insertNode(Node* node, int data);

    // delete node having 'data' from given node 'node'. it returns updated root after removing given node
    Node* deleteNode(Node* node, int data);
    
    // get height of given node
    int getHeight(Node* node);
    
    // get balance factor of given node
    int getBalanceFactor(Node* node);
    
    // single right rotation of sub tree having 'node' as root and return newly updated subroot node 
    Node* singleRightRotation(Node* node);
    
    // single left rotation of sub tree having 'node' as root and return newly updated subroot node 
    Node* singleLeftRotation(Node* node);
    
    // right and left rotation of sub tree having 'node' as root and return newly updated subroot node 
    Node* rightLeftRotation(Node* node);
    
    // left and right rotation of sub tree having 'node' as root and return newly updated subroot node 
    Node* leftRightRotation(Node* node);

public:
    AVL_BST() {
        root = nullptr;
    }
    // BST insertion keeping balancing
    void insert(int data);

    // BST delete keeping balancing. delete node having 'data' from BST.
    void remove(int data);

    // print AVL BST tree on given level
    string printLevel(Node* root, int given_level, int cur_level);

    // write AVL BST tree level by level on output filestream
    void printAVL(ofstream* outfile);


    Node* getRoot();
};

// ===================== Implementation of Member function ================

Node* AVL_BST::insertNode(Node* node, int data) {
    if (node == nullptr) {
        node = new Node(data);
    }
    else {
        if (data < node->data) {    // left case
            // update left tree of 'node' by inserting new node
            node->left = this->insertNode(node->left, data);

            // after inserting new node, check the updated root is balanced or not.            
            int balance = this->getBalanceFactor(node);
            if (balance > 1) {  // if the updated root 'node' is imbalanced, do balancing
                if (data < node->left->data) {  // left left case
                    // do single right rotation on 'node'
                    node = this->singleRightRotation(node);
                }                    
                else {                          // left right case
                    // do left right rotation on 'node'
                    node = this->leftRightRotation(node);
                }
            }
        }
        else if (node->data < data) {      // right case
            // update right tree of 'node' by inserting new node 
            node->right = this->insertNode(node->right, data);

            // after inserting new node, check the updated root is balanced or not.     
            int balance = this->getBalanceFactor(node);
            if (balance < -1) {  // if the updated root 'node' is imbalanced, do balancing
                if (node->right->data < data) {   // right right case
                    // Do single left rotation on 'node'
                    node = this->singleLeftRotation(node);
                }
                else {                              // right left case
                    node = this->rightLeftRotation(node);
                }
                    
            }
        }
    }
    return node;
}

Node* AVL_BST::deleteNode(Node* root, int data) {
    /*------ Step 1:  Normal BST deteltion -------- */
    if (root == nullptr) {
        return root;
    }
    // if given data is smaller than subroot
    if (data < root->data) {
        // start delete operation from left node of subroot
        root->left = deleteNode(root->left, data);
    }
    // if given data is greater than subroot
    else if (data > root->data) {
        // start delete operation from right node of subroot
        root->right = deleteNode(root->right, data);
    }
    // if given data is same as subroot, this subroot should be removed
    // and it's childs must be re-arranged
    else {
        // if the subroot have both child
        if (root->left != nullptr and root->right != nullptr) {
            Node* succParent = root;

            // Find successor
            Node* succ = root->right;
            while (succ->left != nullptr) {
                succParent = succ;
                succ = succ->left;
            }

            // Delete successor.  Since successor is always left child of its parent
            // we can safely make successor's right right child as left of its parent.
            // If there is no succ, then assign succ->right to succParent->right
            if (succParent != root)
                succParent->left = succ->right;
            else
                succParent->right = succ->right;

            // Copy Successor Data to subroot
            root->data = succ->data;
            delete succ;
        }
        else {
            if (root->left == nullptr)
                root = root->right;
            else if (root->right == nullptr)
                root = root->left;
        }
    }
    /*---------- Step 2: Do balancing after deletion -------- */
    // after deleting node, check the updated root is balanced or not.            
    int balance = this->getBalanceFactor(root);

    
    if (balance > 1) {  // Left case
        int left_balance = this->getBalanceFactor(root->left);
        if (left_balance >= 0) { // left left case
            // Do single right rotation
            root = this->singleRightRotation(root);
        }
        else {                    // left right case
            // Do left right rotation
            root = this->leftRightRotation(root);
        }
    }
    else if (balance < -1) {    // Right Case
        int right_balance = this->getBalanceFactor(root->right);
        if (right_balance <= 0) {   // right right case
            // Do single left rotation
            root = this->singleLeftRotation(root);
        }
        else {                      // right left case
            // Do right left rotation
            root = this->rightLeftRotation(root);
        }
    }
    return root;
}

void AVL_BST::insert(int data) {
    this->root = insertNode(this->root, data);
}
void AVL_BST::remove(int data) {
    this->root = deleteNode(this->root, data);
}


// get height of given node
int AVL_BST::getHeight(Node* node) {
    if (node == nullptr) {
        return -1;
    }
    int left_height = getHeight(node->left);
    int right_height = getHeight(node->right);
    if (left_height > right_height) {
        return left_height + 1;
    }
    return right_height + 1;
}

// get balance factor of given node
int AVL_BST::getBalanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return this->getHeight(node->left) - this->getHeight(node->right);
}

/*
right rotate tree from 'A' and return new root 'B'

         A                                      B
        / \                                   /   \
       B   T4      Right Rotate (A)          C      A
      / \          - - - - - - - - ->      /  \    /  \
     C   T3                               T1  T2  T3  T4
    / \
  T1   T2
*/
Node* AVL_BST::singleRightRotation(Node* A) {
    Node* B = A->left;
    Node* T3 = B->right;

    // right rotation
    A->left = T3;
    B->right = A;

    // Return updated root 'B'
    return B;
}

/*
left rotate tree from 'A' and return new root 'B'

          A                                B
         /  \                            /   \
        T1   B     Left Rotate(A)       A      C
            /  \   - - - - - - - ->    / \    / \
           T2   C                     T1  T2 T3  T4
               / \
             T3  T4
*/
Node* AVL_BST::singleLeftRotation(Node* A) {
    Node* B = A->right;
    Node* T2 = B->left;

    // Left rotation
    B->left = A;
    A->right = T2;
    
    // Return updated root 'B'
    return B;
}
/*
   A                            A                            C
  / \                          / \                          /  \
T1   B   Right Rotate (B)    T1   C      Left Rotate(A)   A      B
    / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
   C   T4                      T2   B                  T1  T2  T3  T4
  / \                              /  \
T2   T3                           T3   T4
*/
Node* AVL_BST::rightLeftRotation(Node* A) {
    Node* B = A->right;
    // Do Single Right rotate on 'B'
    Node* updated_B = this->singleRightRotation(B);
    A->right = updated_B;

    // Do single Left rotate on 'A'
    Node* updated_root = this->singleLeftRotation(A);
    return updated_root;
}

/*
     A                               A                           C
    / \                            /   \                        /  \
   B   T4  Left Rotate (B)        C    T4  Right Rotate(A)    B      A
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   C                          B    T3                    T1  T2 T3  T4
    / \                        / \
  T2   T3                    T1   T2
*/
Node* AVL_BST::leftRightRotation(Node* A) {
    Node* B = A->left;
    // Do Single Left rotate on 'B'
    Node* updated_B = this->singleLeftRotation(B);
    A->left = updated_B;

    // Do single Right rotate on 'A'
    Node* updated_root = this->singleRightRotation(A);
    return updated_root;
}

string AVL_BST::printLevel(Node* cur_root, int given_level, int cur_level = 0) {
    string result = "";
    if (cur_root == nullptr) {
        return "";
    }

    // first print on left tree of the node 
    result = result + this->printLevel(cur_root->left, given_level, cur_level + 1);

    // then print current node if current level equals to given level
    if (cur_level == given_level) {
        result = result + to_string(cur_root->data) + " ";
    }

    // last print right child 
    result = result + printLevel(cur_root->right, given_level, cur_level + 1);
    return result;
}

void AVL_BST::printAVL(ofstream* outfile) {
    int height = this->getHeight(this->root);
    for (int i = 0; i <= height; i++) {
        string level_str = this->printLevel(this->root, i, 0);
        level_str = trim(level_str);
        *outfile << level_str << endl;
    }
}

Node* AVL_BST::getRoot() {
    return this->root;
}


int main(int argc, char* argv[])
{
    // Parse arguments using ArgumentManager.h
    ArgumentManager AM = ArgumentManager(argc, argv, ';');
    string input_filename = AM.get("input");            // file name for input
    string out_filename = AM.get("output");             // file name for output

    ifstream infile;                            // filestream for reading of input file
    ofstream outfile;                           // filestream for writing to output file
    infile.open(input_filename);
    outfile.open(out_filename);
    AVL_BST avl_tree = AVL_BST();

    // ============= Load all informations for input ======================
    string line = "";
    int command_type = 0;
    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        line = trim(line);
        if (line.rfind("Insert", 0) == 0) {
            string value_str = getInner(line, '(', ')');
            avl_tree.insert(atoi(value_str.c_str()));
        }
        else if (line.rfind("Remove") == 0) {
            string value_str = getInner(line, '(', ')');
            avl_tree.remove(atoi(value_str.c_str()));
        }
    }
    avl_tree.printAVL(&outfile);
    infile.close();
    outfile.close();

    return 0;
}