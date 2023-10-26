//Tung Kieu
//COSC 2436
//Professor Rizk
//hw3

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int value) {
        data = value;
        prev = nullptr;
        next = nullptr;
    }
};

class DoubleLinkedList {
    private:
        Node* head;
        size_t size;
    public:
        DoubleLinkedList() {
            head = nullptr;
            size = 0;
        }

        void add(int value);
        void insertionSort(ofstream& outfile);
        void selectionSort(ofstream& outfile);
        void print(ofstream& outfile);
};

// add node at the end
void DoubleLinkedList::add(int value)
{
    Node* new_node = new Node(value);
    Node* last_node = this->head;

    if (this->head == nullptr) {
        new_node->prev = nullptr;
        this->head = new_node;     
    }
    else {
        while (last_node->next != nullptr) {
            last_node = last_node->next;
        }
        last_node->next = new_node;
        new_node->prev = last_node;
    }
    
}

// sort a doubly linked list using insertion sort
void DoubleLinkedList::insertionSort(ofstream& outfile) {
   
    Node* current;
    Node* idx;          // temperary node for iterator
    Node* node = this->head;
    for (idx = node->next; idx != nullptr; idx = idx->next)
    {
        int temp = idx->data;
        current = idx;

        while (current != nullptr && current->prev != nullptr && current->prev->data >= temp)
        {
            current->data = current->prev->data;
            current = current->prev;
        }
        current->data = temp;
        
        //============== print sort result for this step ==============//

        node = this->head;
        outfile << "[" << node->data;
        while (node->next != nullptr)
        {
            node = node->next;
            outfile << "," << node->data;
            if (node->next == nullptr)
                outfile << "]" << endl;
        }
       
    }
}

// sort a doubly linked list using selection sort
void DoubleLinkedList::selectionSort(ofstream& outfile) {

    Node* current = nullptr;
    Node* idx = nullptr;            // temperary node for iterator
    Node* node = this->head;
    int temp;
    
    if (this->head == nullptr) {    // if head is nullptr, return
        return;
    }
    else {
        // Current will point to head  
        for (current = node; current->next != nullptr; current = current->next) {
            // idx will point to node next to current  
            for (idx = current->next; idx != nullptr; idx = idx->next) {
              
                if (current->data > idx->data) {
                    temp = current->data;
                    current->data = idx->data;
                    idx->data = temp;
                }
            }

            //============== print sort result for this step ==============//
            node = this->head;
            outfile << "[" << node->data;
            while (node->next != nullptr)
            {
                node = node->next;
                outfile << "," << node->data;
                if (node->next == nullptr)
                    outfile << "]" << endl;
            }
            
        }
    }
}

void DoubleLinkedList::print(ofstream &outfile) {
    Node* node = this->head;
    outfile << "[" << node->data;
    while (node->next != nullptr)
    {
        node = node->next;
        outfile << "," << node->data;
        if (node->next == nullptr)
            outfile << "]" << endl;
    }
}

//================ Auxiliary functions ==================

void printInvalid(ofstream &outfile) {

    outfile << "Input is Invalid.";
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

int main(int argc, char* argv[])
{
    // Parse arguments using ArgumentManager.h
    Node* head = nullptr;
    bool is_invalid = true;
    int line_cnt = 0;
    ArgumentManager AM = ArgumentManager(argc, argv, ';');
    string input_filename = AM.get("input");            // file name for input
    string out_filename = AM.get("output");             // file name for output
    ifstream infile;                            // filestream for reading of input file
    ofstream outfile;                           // filestream for writing to output file
    infile.open(input_filename);
    outfile.open(out_filename);
  
    // ============= Load all informations for input ======================
    string line = "";
    DoubleLinkedList dblLinkedList = DoubleLinkedList();
    
    while (getline(infile, line)) {                     // get and store a line from input file. // if there is line to be parsed, pass  
        is_invalid = false;
        line_cnt++;
        if (line_cnt > 2) {
            break;
        }
        line = trim(line);                              // trim trailing newline '\n' or '\r'
        vector<string> tokens = split(line, ' ');       // split line by space ' '

        if (tokens.size() < 1) {
            is_invalid = true;
        }
        if (tokens.size() > 1)                          // if selected line is the set of numbers, add it to node
        {
            for (int i = 0; i < tokens.size(); i++) {
                dblLinkedList.add(atoi(tokens[i].c_str()));
            }
        }
        else if (tokens.size() == 1) {
            if (tokens[0] == "Insertion") {             // if the sorting method is Insertion
                dblLinkedList.insertionSort(outfile);                   // run insertion sort
            }
            else if (tokens[0] == "Selection") {        // if the sorting method is Selection
                dblLinkedList.selectionSort(outfile);                   // run selection sort
            }
            else
                is_invalid = true;
        }
        else
            is_invalid = true;
    }
    

    if (is_invalid || line_cnt != 2) {
        printInvalid(outfile);
    }
   
    infile.close();
    outfile.close();
    return 0;
}


