// Tung Kieu
// COSC 2436
// Prof. Rizk
// 3/6/2023

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

struct Node
{
    int command;
    int priority;
    struct Node* next;
    Node(int c, int p) {
        command = c;
        priority = p;
        next = nullptr;
    }
    Node() {
        command = -1000;
        priority = -1000;
        next = nullptr;
    }
};

class PriorityQueue {
private:
    Node* head;

public:
    PriorityQueue() {
        head = nullptr;
    }
    // return command of head node
    int peek();

    // pop head node
    void pop();

    // add new node according to it's priority    
    void add(int c, int p);

    bool isEmpty();
};

// ===================== Implementation of Member function ================

int PriorityQueue::peek() {
    if (this->head == nullptr) {
        return -10000;
    }
    else {
        return this->head->command;
    }
}

void PriorityQueue::pop() {
    if (this->head != nullptr) {
        this->head = this->head->next;
    }
}
/*
    add new node according to it's priority in the queue
*/
void PriorityQueue::add(int c, int p) {
    Node* temp = new Node(c, p);
    if (this->head == nullptr) {
        this->head = temp;
        return;
    }
    Node* start = this->head;
    if (this->head->priority > p) {
        temp->next = this->head;
        this->head = temp;
    }
    else {
        while (start->next != nullptr && start->next->priority < p) {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
}

bool PriorityQueue::isEmpty() {
    if (this->head == nullptr) {
        return true;
    }
    else {
        return false;
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

    PriorityQueue p_queue = PriorityQueue();

    // ============= Load all informations for input ======================
    string line = "";
    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        line = trim(line);
        vector<string> tokens = split(line, ' ');       // split line by space ' '
        if (tokens.size() == 4) {
            int command = -1000;
            int priority = -1000;
            string com_str = tokens[0];
            string pri_str = getInner(tokens[3], '(', ')');

            if (com_str == "close") {
                command = 0;
            }
            else if (com_str == "open") {
                command = 1;
            }
            else if (com_str == "lock") {
                command = 2;
            }
            else if (com_str == "unlock") {
                command = 3;
            }
            else {
                break;  // invalid input
            }

            if (is_digits(pri_str)) {
                priority = atoi(pri_str.c_str());
            }
            else {
                break; // invalid input
            }

            p_queue.add(command, priority);
        }
    }
    infile.close();
    int open_status = 0;    // 0: closed, 1:opened
    int lock_status = 1;    //0: locked, 1:unlocked
    while (!p_queue.isEmpty()) {
        int command = p_queue.peek();
        p_queue.pop();
        if (command == 2) {         // lock the door
            lock_status = 0;
        }
        else if (command == 3) {    // unlock the door
            lock_status = 1;
        }
        else if (command == 0) {    // close door
            if (lock_status == 1) {
                open_status = 0;
            }
        }
        else if (command == 1) {    // open the door
            if (lock_status == 1) {
                open_status = 1;
            }
        }
    }

    // =========== write result =================
    outfile.open(out_filename);             // write the result into outfile stream
    if (open_status == 0) {
        outfile << "the door is closed";
    }
    else {
        outfile << "the door is open";
    }
    // llist.print(outfile);
    outfile.close();

    return 0;
}