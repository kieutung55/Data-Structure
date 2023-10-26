#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "DLL.h"
#include "ArgumentManager.h"

using namespace std;

//helper function for judgement
bool valueInList(node* tempHead, string idToCheck)
{
    if (tempHead == nullptr)
    {
        return false;
    }
    else
    {
        return valueInList(tempHead->next, idToCheck) || (tempHead->id == idToCheck);
    }
}
//helper function for judgement
void determineGuilt(DLL& mergedList, DLL& listOfGuilty, int index)
{
    if (index == mergedList.size(mergedList.getHead()) - 1)
    {
        return;
    }
    else
    {
        //compares mergedList[index] with mergedList[index + 1]
        if (mergedList.traverseToN(index, mergedList.getHead())->id == mergedList.traverseToN(index, mergedList.getHead())->next->id)
        {
            //ensures that mergedList[index + 1] has not already been added into listOfGuilty
            if (!valueInList(listOfGuilty.getHead(), mergedList.traverseToN(index, mergedList.getHead())->next->id))
            {
                listOfGuilty.addAtEnd(mergedList.traverseToN(index, mergedList.getHead())->id);
            }
        }
        determineGuilt(mergedList, listOfGuilty, ++index);
    }
}
//helper function for judgement
void determineInnocence(DLL& mergedList, DLL& listOfGuilty, DLL& listOfInnocents, int index)
{
    if (index == mergedList.size(mergedList.getHead()))
    {
        return;
    }
    else
    {
        //ensures that mergedList[index + 1] has not already been added into listOfGuilty
        if (!valueInList(listOfGuilty.getHead(), mergedList.traverseToN(index, mergedList.getHead())->id))
        {
            listOfInnocents.addAtEnd(mergedList.traverseToN(index, mergedList.getHead())->id);
        }
        determineInnocence(mergedList, listOfGuilty, listOfInnocents, ++index);
    }
}
//helper function for judgement
void judgement(DLL& mergedList, DLL& listOfGuilty, DLL& listOfInnocents, int index)
{
    mergedList.insertionSort();
    determineGuilt(mergedList, listOfGuilty, 0);
    determineInnocence(mergedList, listOfGuilty, listOfInnocents, 0);
}

//function to decode (reverse content inside parenthesis)
string reverseParentheses(string s) {
  vector<int> opened;
  string res;
  for (int i = 0; i < s.length(); ++i) {
    if (s[i] == '(')
      opened.push_back(res.length());
    else if (s[i] == ')') {
      int j = opened.back();
      opened.pop_back();
      reverse(res.begin() + j, res.end());
    } 
    else {
      res += s[i];
    }
  }
  return res;
}

int main(int argc, char* argv[])
{
    ArgumentManager am(argc,argv);
    string infileName = am.get("input"); 
    string outfileName = am.get("output");

    ifstream inFile(infileName);
    ofstream outFile(outfileName);
  
    DLL newList;
    DLL guilty;
    DLL innocent;
    //ofstream output("myoutput.txt");

    string input;

    // adds decoded id into linked list newList
    while(getline(inFile, input)) {
      if(input != "Bar1" && input != "Bar2" && input != "") {
         newList.addAtEnd(reverseParentheses(input));
      } 
    }
  
    // print linked list with decoded ids
    //newList.printToConsole(newList.getHead());

    judgement(newList, guilty, innocent, 0);

    //prints to file the list of guilty and innocent
    if (!guilty.empty())
    {
      outFile << "Guilty:" << endl;
      guilty.printToFile(outFile, guilty.getHead()); 
    }
    if (!innocent.empty())
    {
      outFile << endl << "Innocent:" << endl;
      innocent.printToFile(outFile, innocent.getHead()); 
    }
}