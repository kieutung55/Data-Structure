// Tung Kieu
// Professor Rizk
// hw3

#include <algorithm>
#include <fstream>
#include <vector>   // used only in split string by delimiter. never used for queue and stack
#include <string>
#include <sstream>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

struct Person
{
    string name;
    int black_req;
    int red_req;
    struct Person* next;
};

struct Card
{
    string color;
    int card_num;
    struct Card* next;
};

class MyQueue {
private:
    Person* head;
    size_t size;

public:
    MyQueue() {
        head = nullptr;
        size = 0;
    }
    // add new credential node at last position of the queue
    void push(string name, int black_req, int red_req);

    // pop top element of the queue
    Person* pop();
    size_t getSize();
};

class MyStack {
private:
    Card* head;

public:
    MyStack() {
        head = nullptr;
    }
    // add current card deal at top of stack
    void push(string color, int card_num);

    // pop top element of the stack
    Card* pop();
};

// ===================== Implementation of Member function of MyQueue ================
/*
    add new credential node at last position of the queue
*/
void MyQueue::push(string name, int black_req, int red_req) {
    Person* temp = new Person();
    temp->name = name;
    temp->black_req = black_req;
    temp->red_req = red_req;
    temp->next = nullptr;

    if (this->head == nullptr)
    {
        this->head = temp;
    }
    else
    {
        Person* cu = this->head;
        while (cu->next != nullptr)
        {
            cu = cu->next;
        }
        cu->next = temp;
    }
    this->size++;
}

Person* MyQueue::pop() {
    if (this->head == nullptr) {
        return nullptr;
    }
    else {
        Person* temp = this->head;
        this->head = this->head->next;
        this->size--;
        return temp;
    }
}

size_t MyQueue::getSize() {
    return this->size;
}

// ===================== Implementation of Member function of MyStack ================
/*
    add current card deal at top of stack
*/
void MyStack::push(string color, int card_num) {
    Card* temp = new Card();
    temp->color = color;
    temp->card_num = card_num;
    temp->next = nullptr;

    if (this->head == nullptr)
    {
        this->head = temp;
    }
    else
    {
        temp->next = this->head;
        this->head = temp;
    }
}

Card* MyStack::pop() {
    if (this->head == nullptr) {
        return nullptr;
    }
    else {
        Card* temp = this->head;
        this->head = this->head->next;
        return temp;
    }
}


//================ Auxiliary functions ==================

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
        token_stream << str[i];     // add every character into token string
    }
    tokens.push_back(token_stream.str());
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
    string command_filename = AM.get("command");        // file name for output
    string out_filename = AM.get("output");             // file name for output

    ifstream infile;                            // filestream for reading of input file
    ifstream commandfile;                       // filestream for reading of command file
    ofstream outfile;                           // filestream for writing to output file
    infile.open(input_filename);

    MyQueue person_queue = MyQueue();
    MyStack card_stack = MyStack();

    // ============= Load all informations for input ======================
    string line = "";
    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        string name = "";
        string request = "";
        line = trim(line);
        line = removeChar(line, ',');
        vector<string> cred_tokens = split(line, ' ');
        if (cred_tokens.size() == 2) {
            name = cred_tokens[0];
            request = cred_tokens[1];
            
            string black_req_str = request.substr(0, 1);
            int black_req = atoi(black_req_str.c_str());
            string red_req_str = request.substr(1, 1);
            int red_req = atoi(red_req_str.c_str());
            if (black_req + red_req == 10) {
                person_queue.push(name, black_req, red_req);
            }
            else {
                continue;
            }
        }
        else {
            continue;
        }
    }
    infile.close();

    // ============= Load all commands ======================
    commandfile.open(command_filename);
    while (getline(commandfile, line)) {        // get and store a line from input file. // if there is line to be parsed, pass  
        string color = "";
        int card_num = -10;
        line = trim(line);
        color = line.substr(0, 1);        
        string card_num_str = line.substr(1, line.size() - 1);
        card_num = atoi(card_num_str.c_str());
        if (color == "B" || color == "R") {
            card_stack.push(color, card_num);
        }
        else {
            continue;
        }
    }
    commandfile.close();                    // close filestream of input file
    size_t queue_len = person_queue.getSize();
    bool is_burn = false;
    int repeat_num = 0;         // used for checking of card burning
    string prev_color = "";
    int prev_card_num = -10;

    while (true) {
        Card* current = card_stack.pop();
        if (current == nullptr) {
            break;
        }
        string color = current->color;
        int card_num = current->card_num;
        if (color == prev_color && card_num == prev_card_num) {
            repeat_num++;
            if (repeat_num >= queue_len) {      // this means the current command card must be burned
                is_burn = true;
                repeat_num = 0;
            }
        }
        else {
            prev_color = color;
            prev_card_num = card_num;
        }

        Person* person = person_queue.pop();
        string name = person->name;
        int black_req = person->black_req;
        int red_req = person->red_req;
        if (color == "B") {
            int black_remain = black_req - card_num;
            if (black_remain < 0) {
                black_req = 0;
                card_num = abs(black_remain);
                if (is_burn) {                  // if is_burn is true, don't push current card command into stack which means bruning
                    is_burn = false;
                }
                else {
                    card_stack.push(color, card_num);
                }                            
            }
            else if (black_remain > 0) {
                black_req = black_remain;
            }
            else {
                black_req = 0;
            }
            person_queue.push(name, black_req, red_req);
        }
        else if (color == "R") {
            int red_remain = red_req - card_num;
            if (red_remain < 0) {
                red_req = 0;
                card_num = abs(red_remain);
                
                if (is_burn) {                    // if is_burn is true, don't push current card command into stack which means bruning
                    is_burn = false;
                }
                else {
                    card_stack.push(color, card_num);
                }
            }
            else if (red_remain > 0) {
                red_req = red_remain;
            }
            else {
                red_req = 0;
            }
            person_queue.push(name, black_req, red_req);
        }
    }

    // =========== write result =================
    outfile.open(out_filename);             // write the result into outfile stream
    while (true) {
        Person* person = person_queue.pop();
        if (person == nullptr) {
            break;
        }
        string name = person->name;
        int black_req = person->black_req;
        int red_req = person->red_req;
        if (black_req != 0 || red_req != 0) {
            outfile << name << endl;
        }
    }
    outfile.close();

    return 0;
}