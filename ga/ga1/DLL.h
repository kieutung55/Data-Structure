#ifndef DLL_H
#define DLL_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct node
{
    string id;
    node* next;
    node* prev;
};

class DLL
{
    private:
        node* head;
        node* tail;
    public:
        //For all functions that require tempHead,
        //use the getHead function
        DLL();
        //supplementary functions
        node* traverseToN(int n, node* tempHead);
        int size(node* tempHead);
        node* getHead();
        node* getTail();
        bool empty();
        void swap(node* nodeA, node* nodeB);
        //essential functions
        void addAtEnd(string idToAdd);
        void insertionSort();
        void printToConsole(node* tempHead);
        void printToFile(ofstream& output, node* tempHead);
        //entirely ignorable: these were created just to do insertion sort recursively;
        //in that sense, they are helper functions of the insertionSort function.
        void insertionSortOuter(int unsortedIndex);
        void insertionSortInner(int sortedIndex, node* unsortedValue);
};

#endif