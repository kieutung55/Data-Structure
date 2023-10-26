//TUng Kieu
//COSC 2436
//Professor Rizk
//Lab 2

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

template <typename T>
struct T_Node {
    T data;
    struct T_Node* next;
    T_Node(T value) {
        data = value;
        next = nullptr;
    }
    T_Node() {
        next = nullptr;
    }
};

template <typename T>
class T_LinkedList {
private:
    T_Node<T>* head;
    size_t size;

public:
    T_LinkedList() {
        head = nullptr;
        size = 0;
    }

    // add new node at given position
    void addAtPos(int pos, T value);
    // add new node at head or tail depending on flag 'is_head'
    void add(T value, bool is_head = true);

    // remove node at given position
    void removeAtPos(int pos);
    // remove node at head or tail depending on flag 'is_head'
    void remove(bool is_head = true);

    // write all nodes into outfile stream
    void print(ofstream& outfile);
    // write all nodes on console
    void print();

};

template<typename T>
void T_LinkedList<T>::addAtPos(int pos, T value)
{
    T_Node<T>* new_node = new T_Node<T>(value);

    if (this->head == nullptr) {
        this->head = new_node;
        this->size++;
        return;
    }

    if (pos == 0)
    {
        new_node->next = this->head;
        this->head = new_node;
    }
    else
    {
        T_Node<T>* cur_node = new T_Node<T>();
        T_Node<T>* after_node = new T_Node<T>();
        cur_node = this->head;
        int id = 1;
        while (cur_node->next != nullptr)
        {
            if (id == pos) {
                break;
            }
            cur_node = cur_node->next;
            id++;
        }
        if (pos < this->size) {
            after_node = cur_node->next;
            cur_node->next = new_node;
            new_node->next = after_node;
        }
        else {  
            cur_node->next = new_node;
        }
    }
    this->size++;

}

template<typename T>
void T_LinkedList<T>::add(T value, bool is_head)
{
    if (is_head) {
        this->addAtPos(0, value);
    }
    else {
        this->addAtPos(this->size, value);
    }
    
}

template<typename T>
void T_LinkedList<T>::removeAtPos(int pos) {
    if (this->head == nullptr) {
        return;
    }

    if (pos == 0)
    {
        this->head = this->head->next;
    }
    else
    {
        T_Node<T>* cur_node = new T_Node<T>();
        T_Node<T>* before_node = new T_Node<T>();
        cur_node = this->head;
        before_node = nullptr;
        int id = 0;
        while (cur_node->next != nullptr)
        {
            if (id == pos) {                
                break;
            }
            before_node = cur_node;
            cur_node = cur_node->next;
            id++;
        }
        if (before_node == nullptr) {   // this case is when there are only one node in the linked list
            this->head = nullptr;
        }
        else {
            if (pos < this->size) {
                before_node->next = cur_node->next;
            }
            else {                      // in case of when the pos is greater than size of linked list
                before_node->next = nullptr;
            }
        }        
    }
    this->size--;
}

template<typename T>
void T_LinkedList<T>::remove(bool is_head)
{
    if (is_head) {
        this->removeAtPos(0);
    }
    else {
        this->removeAtPos(this->size);
    }
    
}

template <typename T>
void T_LinkedList<T>::print(ofstream& outfile) {
    if (this->head == nullptr)
    {
        return;
    }
    else
    {
        T_Node<T>* cu = this->head;
        outfile << cu->data;
        while (cu->next != nullptr)
        {
            cu = cu->next;
            outfile << " " << cu->data;
        }
    }
}

template <typename T>
void T_LinkedList<T>::print() {
    if (this->head == nullptr)
    {
        return;
    }
    else
    {
        T_Node<T>* cu = this->head;
        cout << cu->data;
        while (cu->next != nullptr)
        {
            cu = cu->next;
            cout << " " << cu->data;
        }
    }
    cout << endl;
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
        if (str[i] == '\0') {            
            continue;
        }
        token_stream << str[i];     // add every character into token string
    }
    string str_trimed = trim(token_stream.str());
    tokens.push_back(str_trimed);
    return tokens;
}



/*
remove all characters 'c' in the given string

if c = ']' and str = "hello ]"
then result will be "hello "
*/
string removeChar(string str, char c) {
    str.erase(std::remove(str.begin(), str.end(), c), str.end());
    return str;
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

    T_LinkedList<int> int_llist = T_LinkedList<int>();          // LinkedList for type 'int' 
    T_LinkedList<string> str_llist = T_LinkedList<string>();    // LinkedList for type 'string' 
    T_LinkedList<char> char_llist = T_LinkedList<char>();       // LinkedList for type 'char' 


    // ============= Load all informations for input ======================
    string line = "";  
    string data_type = "";
    bool is_head = true;
    if (getline(infile, line)) {
        line = trim(line);      // trim trailing newline '\n' or '\r'
        data_type = line;
    }

    if (getline(infile, line)) {
        line = trim(line);      // trim trailing newline '\n' or '\r'
        if (line == "head") {
            is_head = true;
        }
        else if (line == "tail") {
            is_head = false;
        }
    }

    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        line = trim(line);      // trim trailing newline '\n' or '\r'
        vector<string> tokens = split(line, ' ');       // split line by space ' '
        // ------------- command for remove node if there are 2 tokens on line -----------------
        if (tokens.size() > 1) {   
            string param = tokens[1];
            if (param == "head") {
                if (data_type == "string") {
                    str_llist.remove(true);
                }
                else if (data_type == "int") {
                    int_llist.remove(true);
                }
                else if (data_type == "char") {
                    char_llist.remove(true);
                }
            }
            else if (param == "tail") {
                if (data_type == "string") {
                    str_llist.remove(false);
                }
                else if (data_type == "int") {
                    int_llist.remove(false);
                }
                else if (data_type == "char") {
                    char_llist.remove(false);
                }
            }
            else {
                string pos_str = getInner(param, '[', ']');
                if (pos_str != "") {
                    int pos = atoi(pos_str.c_str());
                    if (data_type == "string") {
                        str_llist.removeAtPos(pos);
                    }
                    else if (data_type == "int") {
                        int_llist.removeAtPos(pos);
                    }
                    else if (data_type == "char") {
                        char_llist.removeAtPos(pos);
                    }
                }
            }
        }
        //--------------  command for add node if there are only one token in line ----------------------
        else {   
            string token = tokens[0];
            if (data_type == "string") {
                str_llist.add(token, is_head);
            }
            else if (data_type == "int") {
                int value = atoi(token.c_str());
                int_llist.add(value, is_head);
            }
            else if (data_type == "char") {
                char_llist.add(token[0], is_head);
            }
            
        }        
    }
    infile.close();  

    outfile.open(out_filename);
    if (data_type == "string") {
        str_llist.print(outfile);
    }
    else if (data_type == "int") {
        int_llist.print(outfile);
    }
    else if (data_type == "char") {
        char_llist.print(outfile);
    }

    return 0;
}


