//hw1 
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

struct Node
{
    string name;
    int age;
    int deposit;
    int numDrinks;
    struct Node* next;
};

class LinkedList {
private:
    Node* head;
    int size;   // count of nodes in linked list

public:
    LinkedList() {
        head = nullptr;
        size = 0;
    }
    // add new credential node at last position of linked list
    void add(string name, int age, int deposit, int numDrinks);

    // add new credential node at given position 'pos' of linked list
    // if there is exists duplicated node(same name and same age), update it's credential by new one 
    void addAtPos(int pos, string name, int age, int deposit, int numDrinks);  // pos is 0-based index. for exam, 0:first positin(head of linked list), 1:second node, ...

    // using template in case attribute is "name"
    template<typename T>
    void remove(string attribute, T value);

    // Remove a credential node that contains the attribute 'attribute' with the matching value 'value'  from the linked list
    void remove(string attribute, string value);

    // return a node at position 'pos'.  'pos' is 0-based index
    Node* getAtPos(int pos);

    // get previous node of given node 'node'
    Node* getBeforeNode(Node* node);

    // write all nodes of linked list into file stream for writing on output file
    void print(ofstream& output);

    // swap two nodes each other.  using swap inside sort function helps code look cleaner
    void swap(Node* cu1, Node* cu2);

    // Sort the linked list base on the given attribute
    // using BUBBLE SORT, case attribute is "name", can use ">" and "<", it knows hot to sort alphabetically
    void sort(string attribute);
};

// ===================== Implementation of Member function ================
/* 
    add new credential node at last position of linked list 
*/
void LinkedList::add(string name, int age, int deposit, int numDrinks) {
    Node* temp = new Node();
    temp->name = name;
    temp->age = age;
    temp->deposit = deposit;
    temp->numDrinks = numDrinks;
    temp->next = NULL;

    if (this->head == nullptr)
    {
        this->head = temp;
    }
    else
    {
        Node* cu = this->head;
        while (cu->next != nullptr)
        {
            cu = cu->next;
        }
        cu->next = temp;
    }
    this->size++;
}

/*
    add new credential node at given position 'pos' of linked list
    if there is exists duplicated node(same name and same age), update it's credential by new one
*/
void LinkedList::addAtPos(int pos, string name, int age, int deposit, int numDrinks) {
    Node* new_node = new Node();
    new_node->name = name;
    new_node->age = age;
    new_node->deposit = deposit;
    new_node->numDrinks = numDrinks;
    new_node->next = NULL;

    if (this->head == nullptr) {
        this->head = new_node;
        this->size++;
        return;
    }
    
    bool is_duplicated = false;
    if (name == this->head->name && age == this->head->age) {  // check if head is duplicated node or not
        // update duplicated node by new credential
        this->head->deposit = deposit;
        this->head->numDrinks = numDrinks;
        is_duplicated = true;
    }
    else {
        // finding duplicated node in linked list
        Node* cu = this->head;
        while (cu->next != nullptr)
        {
            cu = cu->next;
            if (name == cu->name && age == cu->age) {  // if found duplicated, update credential
                cu->deposit = deposit;
                cu->numDrinks = numDrinks;
                is_duplicated = true;
                break;
            }
        }
        // if there is no duplicated node, insert new node at give position
        if (!is_duplicated) {   
            if (pos == 0)
            {
                new_node->next = this->head;
                this->head = new_node;
            }
            else
            {
                Node* cur_node = new Node();
                Node* after_node = new Node();
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
                after_node = cur_node->next;
                cur_node->next = new_node;
                new_node->next = after_node;

            }
            this->size++;
        }        
    }    
}

/*
    Remove a credential node that contains the attribute 'attribute' with the matching value 'value'  from the linked list
*/
void LinkedList::remove(string attribute, string value) {
    // first, check the head. if header is target to delete, make head->next as head
    if (attribute == "name" && value == this->head->name ||
        attribute == "age" && atoi(value.c_str()) == this->head->age ||
        attribute == "deposit" && atoi(value.c_str()) == this->head->deposit ||
        attribute == "number of drinks" && atoi(value.c_str()) == this->head->numDrinks) {
        this->head = this->head->next;  // if head is target to delete, make head->next as head
        return;
    }

    Node* cur_node = new Node();
    Node* before_node = new Node();
    cur_node = this->head;
    while (cur_node->next != nullptr)
    {   // if current node is target to delete, omit the current node by assigning 'next' newly
        if (attribute == "name" && value == cur_node->name ||
            attribute == "age" && atoi(value.c_str()) == cur_node->age ||
            attribute == "deposit" && atoi(value.c_str()) == cur_node->deposit ||
            attribute == "number of drinks" && atoi(value.c_str()) == cur_node->numDrinks) {
            break;
        }
        before_node = cur_node;
        cur_node = cur_node->next;
    }
    before_node->next = cur_node->next;
}

/*
    get previous node of given node 'node'
*/
Node* LinkedList::getBeforeNode(Node* node) {
    Node* tmp;
    if (node == this->head)
        return NULL;
    else {
        tmp = this->head;
        while (tmp->next != node)
            tmp = tmp->next;
        return tmp;
    }
}

/*
    return a node at position 'pos'.  'pos' is 0-based index
*/
Node* LinkedList::getAtPos(int pos) {
    if (pos == 0)
    {
        return this->head;
    }
    else
    {
        Node* cur_node = new Node();
        cur_node = this->head;
        int id = 0;
        while (cur_node->next != nullptr)
        {
            if (id == pos) {
                return cur_node;
            }
            cur_node = cur_node->next;
            id++;
        }
        if (cur_node != nullptr) {
            return cur_node;
        }
        return nullptr;
    }
}

/*
    write all nodes of linked list into file stream for writing on output file
*/
void LinkedList::print(ofstream& outfile) {
    if (this->head == nullptr)
    {
        return;
    }
    else
    {
        Node* cu = this->head;
        outfile << "[name: " << cu->name << "; age: " << cu->age << "; deposit: " << cu->deposit << "; number of drinks: " << cu->numDrinks << "]" << endl;
        while (cu->next != nullptr)
        {
            cu = cu->next;
            outfile << "[name: " << cu->name << "; age: " << cu->age << "; deposit: " << cu->deposit << "; number of drinks: " << cu->numDrinks << "]" << endl;
        }
    }
}

/*
    swap two nodes each other.  using swap inside sort function helps code look cleaner
*/
void LinkedList::swap(Node* node1, Node* node2) {
    Node* pre_node1 = this->getBeforeNode(node1);
    Node* pre_node2 = this->getBeforeNode(node2);
    if (this->head == NULL || node1 == NULL || node2 == NULL) { // can not to swap since there is nothing to be swapped
        return;     
    }  

    // swap before nodes of two nodes
    if (pre_node1 != NULL) {
        pre_node1->next = node2;
    }
    else {
        this->head = node2;
    }

    if (pre_node2 != NULL) {
        pre_node2->next = node1;
    }
    else {
        this->head = node1;
    }

    // swap next nodes of two nodes
    Node* tmp = node1->next;
    node1->next = node2->next;
    node2->next = tmp;    
}

/*
Sort the linked list base on the given attribute
*/
void LinkedList::sort(string attribute) {
    for (int i = 0; i < this->size - 1; i++) {
        for (int j = this->size - 1; i < j; j--) {
            Node* node_i = this->getAtPos(i);
            Node* node_j1 = this->getAtPos(j - 1);
            if (attribute == "name" && node_i->name < node_j1->name ||
                attribute == "age" && node_i->age < node_j1->age ||
                attribute == "deposit" && node_i->deposit > node_j1->deposit ||
                attribute == "number of drinks" && node_i->numDrinks < node_j1->numDrinks) {
                this->swap(node_i, node_j1);
            }
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
parse a credential and return result in map format.
ex:
line = "name: Ali; age: 26; deposit: 7500; number of drinks: 3"
then 
result["name"] will be "Ali"
result["age"] will be "26"
...
*/
map<string, string> parseCredential(string line) {
    map<string, string> cred_map;
    line = getInner(line, '[', ']');
    string name = "";
    int age = -1;
    int deposit = -1;
    int drinks = -1;
    string item_name = "";
    int start_pos = 0;
    int end_pos = 0;
    for (int i = 0; i < line.length(); i++) {       // iterate all characters in input string 'line'
        int len_value = i - start_pos;

        if (line[i] == ':') {                       // take attribute name when iterator meet colon.
            start_pos = i + 2;
            item_name = line.substr(end_pos, i - end_pos);
        }
        if (line[i] == ';') {                       // store value of attribute when iterator meet semi-colon into map
            cred_map[item_name] = line.substr(start_pos, len_value);            
            end_pos = i + 2;
        }
        if (i == line.length() - 1) {               // at the end of string, store value of lates attribute
            cred_map[item_name] = line.substr(start_pos, line.length() - start_pos);
        }
    }
    return cred_map;
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

    LinkedList llist = LinkedList();

    // ============= Load all informations for input ======================
    string line = "";  
    while (getline(infile, line)) {             // get and store a line from input file. // if there is line to be parsed, pass  
        line = getInner(line, '[', ']');
        string name = "";
        int age = -1;
        int deposit = -1;
        int drinks = -1;
        
        map<string, string> cred_map = parseCredential(line);     
        name = cred_map["name"];
        age = atoi(cred_map["age"].c_str());    // cred_map["age"].c_str() is char address 
        deposit = atoi(cred_map["deposit"].c_str());
        drinks = atoi(cred_map["number of drinks"].c_str());
        llist.add(name, age, deposit, drinks);  // add this credential into linked list
    }
    infile.close();    

    // ============= Load all commands ======================
    commandfile.open(command_filename);
    while (getline(commandfile, line)) {        // get and store a line from input file. // if there is line to be parsed, pass  
        string command = "";        // command type
        string command_args = "";   // command arguments,  ex: "number of drinks: 7" in case of remove , "depoist" in case of sort, "3" in case of Add            
        
        vector<string> tokens = split(line, '[');               // split line into two parts: one is command part and another is for credential in case of "Add" command
        vector<string> com_tokens = split(tokens[0], '(');      // split command part into two parts: one is command type, another is argument of command
        command = com_tokens[0];        
        command = removeChar(command, ' ');                     // get command type
        command_args = getInner(tokens[0], '(', ')');           // get command argument, ex: "number of drinks: 7" in case of remove , "depoist" in case of sort, "3" in case of Add
        if (command == "Add") {
            if (tokens.size() == 2) {
                int add_pos = atoi(command_args.c_str());
                string cred_str = tokens[1];
                cred_str = removeChar(cred_str, ']');
                string name = "";
                int age = -1;
                int deposit = -1;
                int drinks = -1;

                map<string, string> cred_map = parseCredential(cred_str);   // parse credential
                name = cred_map["name"];
                age = atoi(cred_map["age"].c_str());
                deposit = atoi(cred_map["deposit"].c_str());
                drinks = atoi(cred_map["number of drinks"].c_str());

                llist.addAtPos(add_pos, name, age, deposit, drinks);        // add this credential at given position
            }            
        }
        else if (command == "Remove") {
            vector<string> arg_tokens = split(command_args, ':');
            if (arg_tokens.size() == 2) {
                string remove_attr = arg_tokens[0];
                string attr_val_str = arg_tokens[1];
                attr_val_str = attr_val_str.substr(1, attr_val_str.length() - 1);  // remove trailing space of the string, ex: " Rob" -> "Rob"
                int attr_val = atoi(attr_val_str.c_str());
                llist.remove(remove_attr, attr_val_str);
            }
        }
        else if (command == "Sort") {
            llist.sort(command_args);
        }
    }
    commandfile.close();                    // close filestream of input file

    // =========== write result =================
    outfile.open(out_filename);             // write the result into outfile stream
    llist.print(outfile);
    outfile.close();

    return 0;
}