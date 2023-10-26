//Tung Kieu
//COSC 2436
//Professor Rizk

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;


struct Node {
    string data;
    struct Node* left, * right;
    Node(string data_in) {
        data = data_in;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
private:
    Node* root;
    bool is_ascend;     // mirror status, true is small->larger, false is larger->small
public:
    BST() {
        root = nullptr;
        is_ascend = true;
    }
    // insert new node
    void insert(string data_in);
    
    // print BST tree from most left to most right
    string printLR(Node* root);

    // print BST tree on given level
    string printLevel(Node* root, int given_level, int cur_level);    
    
    // compare two string. If the one is digits and other is string, then return -1
    // if str1 > str2 , return 1. if str1 < str2, return 2. if two are equal, return 0
    int compareString(string str1, string str2);
    
    Node* getRoot();

    // set Mirror and swap all nodes according to cuurent mirror status
    void setMirror();

    void swapTree(Node* root);
};

// Check if current string is digits
bool is_digits(string str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}
// ===================== Implementation of Member function ================

void BST::insert(string data_in) {
    Node* new_node = new Node(data_in);
    if (this->root == nullptr) {
        this->root = new_node;
    }
    else {
        Node* prev = nullptr;
        Node* temp = this->root;
        // ------- go to node (here, that is 'prev') to be attach new node -----
        while (temp) {
            int compare_result = this->compareString(temp->data, data_in);  // calculate the order between two strings
            if (compare_result < 0) {   // one is digit while another is not
                return;                 // discard this case
            }
            else {
                if (compare_result == 1) {
                    prev = temp;
                    if (this->is_ascend) {      // if left is smaller and right is bigger, add smaller string into left node
                        temp = temp->left;
                    }
                    else {                      // if left is bigger and right is smaller, add smaller string into right node
                        temp = temp->right;
                    }
                }
                else if (compare_result == 2) {
                    prev = temp;
                    if (this->is_ascend) {
                        temp = temp->right;
                    }
                    else {
                        temp = temp->left;
                    }

                }
            }
            
        }

        // attach new node to 'prev'
        int compare_result = this->compareString(prev->data, data_in);
        if (compare_result < 0) {
            return;
        }
        else {
            if (compare_result == 1) {
                if (this->is_ascend) {
                    prev->left = new_node;
                }
                else {
                    prev->right = new_node;
                }

            }
            else {
                if (this->is_ascend) {
                    prev->right = new_node;
                }
                else {
                    prev->left = new_node;
                }
            }
        }        
            
    }
}

string BST::printLR(Node* cur_root) {
    string result = "";
    if (cur_root == nullptr)
        return "";

    // first print on left tree of the node 
    result = result + printLR(cur_root->left);

    // then print current node 
    result = result + cur_root->data + " ";

    // last print right child 
    result = result + printLR(cur_root->right);
    return result;
}

string BST::printLevel(Node* cur_root, int given_level, int cur_level = 0) {
    string result = "";
    if (cur_root == nullptr) {
        return "";
    }        

    // first print on left tree of the node 
    result = result + printLevel(cur_root->left, given_level, cur_level + 1);

    // then print current node if current level equals to given level
    if (cur_level == given_level) {
        result = result + cur_root->data + " ";
    }   

    // last print right child 
    result = result + printLevel(cur_root->right, given_level, cur_level + 1);
    return result;
}

Node* BST::getRoot() {
    return this->root;
}

void BST::setMirror() {
    this->is_ascend = !this->is_ascend;
    swapTree(this->root);
}

// swap all nodes of currnet BST 
void BST::swapTree(Node* cur_root) {
    if (cur_root == nullptr) {
        return;
    }
    // swap two childs of current root.
    Node* temp = cur_root->left;
    cur_root->left = cur_root->right;
    cur_root->right = temp;

    // Continue swap for child nodes
    swapTree(cur_root->left);
    swapTree(cur_root->right);
}

int BST::compareString(string str1, string str2) {
    bool is_digits1 = is_digits(str1);
    bool is_digits2 = is_digits(str2);
    if (is_digits1 != is_digits2) {
        return -1;
    }
    if (is_digits1) {   // In case of two string are digits, compare them by numerical order
        int val1 = atoi(str1.c_str());
        int val2 = atoi(str2.c_str());
        if (val1 > val2) {
            return 1;
        }
        else if (val1 < val2) {
            return 2;
        }
        else {
            return 0;
        }
    }
    else {              // In case of two string are not digits, compare them by alphabetically
        if (str1 > str2) {
            return 1;
        }
        else if (str1 < str2) {
            return 2;
        }
        else {
            return 0;
        }
    }
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
    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        line = trim(line);
        // get Command type
        
        if (line.rfind("Insert", 0) == 0) {
            string value = getInner(line, '(', ')');
            bst.insert(value);
        }
        else if (line.rfind("PrintLR") == 0) {
            string result = bst.printLR(bst.getRoot());
            result = trim(result);
            outfile << result << endl;
        }
        else if (line.rfind("PrintLevel") == 0) {
            string value = getInner(line, '(', ')');
            int given_level = atoi(value.c_str());
            string result = bst.printLevel(bst.getRoot(), given_level, 0);
            result = trim(result);
            if (result == "") {
                outfile << "Does Not Exist!" << endl;
            }
            else {
                outfile << result << endl;
            }            
        }
        else if (line.rfind("Mirror") == 0) {
            bst.setMirror();
        }
    }
    infile.close();
    outfile.close();

    return 0;
}