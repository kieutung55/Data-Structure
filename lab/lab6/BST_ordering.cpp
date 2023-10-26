// LinkedList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

struct Node {
    int data;
    struct Node* left, * right;
    Node(int data_in) {
        data = data_in;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
private:
    Node* root;

public:
    BST() {
        root = nullptr;
    }
    void insert(int data_in);
    // delete node having 'data' from BST
    void remove(int data);
    // delete node having 'data' from given node 'node'. it returns updated root after removing given node
    Node* deleteNode(Node* root, int data);
    // delete node having 'data' from entire tree
    // print BST tree by inorder
    string printInorder(Node* root);
    // print BST by pre-order
    string printPreorder(Node* root);
    // print BST by post-order
    string printPostorder(Node* root);
    Node* getRoot();
};

// ===================== Implementation of Member function ================

void BST::insert(int data_in) {
    Node* new_node = new Node(data_in);
    if (this->root == nullptr) {
        this->root = new_node;
    }
    else {
        Node* prev = nullptr;
        Node* temp = this->root;
        while(temp) {
            if (temp->data > data_in) {
                prev = temp;
                temp = temp->left;
            }
            else if (temp->data < data_in) {
                prev = temp;
                temp = temp->right;
            }
        }
        if (prev->data > data_in)
            prev->left = new_node;
        else
            prev->right = new_node;
    }
}
void BST::remove(int data) {
    this->root = deleteNode(this->root, data);
}
Node* BST::deleteNode(Node* root, int data) {
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
        // if the subroot is leaf
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        // if subroot have one child at max
        else if (root->left == nullptr) {
            Node* right_node = root->right;
            delete root;
            return right_node;
        }
        else if (root->right == nullptr) {
            Node* left_node = root->left;
            delete root;
            return left_node;
        }
        // if the subroot have both child
        else {
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
    }
    return root;
        
}

string BST::printInorder(Node* cur_root) {
    string result = "";
    if (cur_root == nullptr)
        return "";

    // first print on left tree of the node 
    result  = result + printInorder(cur_root->left);

    // then print current node 
    result = result + to_string(cur_root->data) + " ";

    // last print right child 
    result = result + printInorder(cur_root->right);
    return result;
}

string BST::printPreorder(Node* cur_root) {
    string result = "";
    if (cur_root == nullptr)
        return "";

    // first print cuurent node 
    result = result + to_string(cur_root->data) + " ";

    // then print on left tree of the node 
    result = result + printPreorder(cur_root->left);

    // last print on right tree of the node
    result = result + printPreorder(cur_root->right);
    return result;
}

string BST::printPostorder(Node* cur_root) {
    string result = "";
    if (cur_root == nullptr)
        return "";

    // first print on left subtree of the node
    result = result + printPostorder(cur_root->left);

    // then print on right tree of the node
    result = result + printPostorder(cur_root->right);

    // last print current node
    result = result + to_string(cur_root->data) + " ";
    return result;
}


Node* BST::getRoot() {
    return this->root;
}



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

/*
split string by delimiter 'del' and return vector of splited tokens
ex:
str = "hello world", del = ' '
then result in two tokens "hello" and "world"
*/
vector<string> split(string str, char del) {
    vector<string> tokens = vector<string>();
    // declaring temp string to store the curr "word" upto del
    string temp = "";
    stringstream token_stream;
    bool is_in = false;
    for (unsigned int i = 0; i <= str.length(); i++) { // iterate all characters in input string 'str'

        if (str[i] == del) {
            // Every meet delimiter, push token into vector and clear token string
            tokens.push_back(token_stream.str());
            token_stream.str("");
            continue;
        }
        token_stream << str[i];     // add every character into token string
    }
    tokens.push_back(token_stream.str());
    return tokens;
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

bool is_digits(const std::string& str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
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
    BST bst = BST();

    // ============= Load all informations for input ======================
    string line = "";
    int command_type = 0;
    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        line = trim(line);
        // get Command type
        if (line == "Insert") {
            command_type = 1; 
        }
        else if (line == "Remove") {
            command_type = 2;
        }
        else if (line == "Traverse") {
            command_type = 3;
        }
        // get command argument
        else {
            if (command_type == 1) {
                vector<string> tokens = split(line, ' ');                
                for (int i = 0; i < tokens.size(); i++) {
                    bst.insert(atoi(tokens[i].c_str()));
                }
            }
            else if (command_type == 2) {
                vector<string> tokens = split(line, ' ');
                for (int i = 0; i < tokens.size(); i++) {
                    bst.remove(atoi(tokens[i].c_str()));
                }
            }
            else if (command_type == 3) {
                Node* root = bst.getRoot();
                string inorder = bst.printInorder(root);
                string preorder = bst.printPreorder(root);
                string postorder = bst.printPostorder(root);
                inorder = trim(inorder);
                preorder = trim(preorder);
                postorder = trim(postorder);
                if (line == inorder) {
                    outfile << "Inorder" << endl;
                }
                else if (line == preorder) {
                    outfile << "Preorder" << endl;
                }
                else if (line == postorder) {
                    outfile << "Postorder" << endl;
                }
                else {
                    outfile << "False";
                }
            }
            command_type = 0;
        }
    }
    infile.close();    
    outfile.close();

    return 0;
}