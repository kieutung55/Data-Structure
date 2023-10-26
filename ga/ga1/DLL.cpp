#include "DLL.h"

using namespace std;

DLL::DLL()
{
    //allocate memory for nodes
    head = nullptr;
    tail = nullptr;
}

node* DLL::traverseToN(int n, node* tempHead)
{
    if (n == 0)
        return tempHead;
    else
    {
        return traverseToN(n - 1, tempHead->next);
    }
}
int DLL::size(node* tempHead)
{
    if (tempHead == nullptr)
        return 0;
    else
        return 1 + size(tempHead->next);
}
node* DLL::getHead()
{
    return head;
}
node* DLL::getTail()
{
    return tail;
}
bool DLL::empty()
{
    return head == nullptr && tail == nullptr;
}
void DLL::swap(node* nodeA, node* nodeB)
{
    string tempID = nodeA->id;

    nodeA->id = nodeB->id;
    nodeB->id = tempID;
}

void DLL::addAtEnd(string idToAdd)
{
    //create new node
    node* temp = new node;
    temp->id = idToAdd;
    temp->next = nullptr;
    temp->prev = nullptr;

    if (empty())
    {
        head = tail = temp;
    }
    else
    {
        //connecting the current tail
        tail->next = temp;
        temp->prev = tail;
        tail = temp;
    }

}
void DLL::insertionSort()
{
    insertionSortOuter(1);
}
void DLL::insertionSortOuter(int unsortedIndex)
{
    if (unsortedIndex < size(getHead()))
    {
        node* unsortedValue = traverseToN(unsortedIndex, getHead());
        insertionSortInner(unsortedIndex - 1, unsortedValue);
        insertionSortOuter(unsortedIndex + 1);
    }
    else
    {
        return;
    }
}
void DLL::insertionSortInner(int sortedIndex, node* unsortedValue)
{
    if (sortedIndex >= 0 && stoi(traverseToN(sortedIndex, getHead())->id) > stoi(unsortedValue->id))
    {
        swap(traverseToN(sortedIndex + 1, getHead()), traverseToN(sortedIndex, getHead()));
        insertionSortInner(sortedIndex - 1, unsortedValue->prev);
    }
    else
    {
        return;
    }
}

void DLL::printToConsole(node* tempHead)
{
    if (tempHead == nullptr)
        cout << endl;
    else
    {
        cout << tempHead->id << " ";
        printToConsole(tempHead->next);
    }
}
void DLL::printToFile(ofstream& output, node* tempHead)
{
    if (tempHead->next == nullptr)
        output << tempHead->id;
    else
    {
        output << tempHead->id << endl;
        printToFile(output, tempHead->next);
    }
}