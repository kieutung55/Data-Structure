#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <algorithm>
#include "ArgumentManager.h"

using namespace std;

//The inputProcessor function reads the input file line by line and removes any newline or carriage return characters. It then stores each non-empty line in a queue, which is returned at the end of the function
queue<string> inputProcessor(ifstream& inputStream)
{
  string input;
  queue<string> queueToReturn;
  while(!inputStream.eof())
  {
    getline(inputStream, input);
    input.erase(remove(input.begin(), input.end(), '\n'), input.end());
    input.erase(remove(input.begin(), input.end(), '\r'), input.end());

    if (!input.empty())
    {
      queueToReturn.push(input);
    }
  }

  return queueToReturn;
}

//Following three functions are helper functions for the main validity checking functions

//The matchingParentheses function takes two characters representing an open and closed parenthesis and returns true if they match
bool matchingParentheses(char open, char closed)
{
  if (open == '{' && closed == '}')
    return true;
  else if (open == '[' && closed == ']')
    return true;
  else if (open == '(' && closed == ')')
    return true;
  else
    return false;
}

//The areParentheses function takes two characters and returns true if both characters are open parentheses
bool areParentheses(char one, char two)
{

  bool charOneParentheses = (one == '(') || (one == '[') || (one == '{');
  bool charTwoParentheses = (two == '(') || (two == '[') || (two == '{');

  //Returns true if both are a form of parentheses
  return charOneParentheses && charTwoParentheses;
}


//The precedence function takes an open parenthesis character and returns its precedence level (3 for '{', 2 for '[', and 1 for '(', since '{' has the highest precedence)
//This function should only receive '{', '[', '('—hence, the default case is '('
int precedence (char openParen)
{
  if (openParen == '{')
    return 3;
  else if (openParen == '[')
    return 2;
  else
    return 1;
}

//Following three functions are main validity-checking functions

//The validParentheses function checks whether its parentheses are balanced and properly nested. It uses a stack to keep track of the open parentheses encountered and matches them with the corresponding closing parentheses
//If a closing parenthesis is encountered without a matching open parenthesis, the function returns false
bool validParentheses(string toCheck)
{
  int size = toCheck.size();
  stack<char> stackOfChars;

  for (int i = 0; i < size; i++)
  {
    //will push open parentheses
    if (toCheck[i] == '(' || toCheck[i] == '[' || toCheck[i] == '{')
      stackOfChars.push(toCheck[i]);
    //branches here if closed parenthesis encountered
    else if (toCheck[i] == ')' || toCheck[i] == ']' || toCheck[i] == '}')
    {
      //first arg should be open parentheses
      //second arg should be matching closed parentheses
      //else exits function and returns false
      if (stackOfChars.empty() || !matchingParentheses(stackOfChars.top(), toCheck[i]))
        return false;
      else
      {
        stackOfChars.pop();
      }
    }
  }
  //if no errors encountered, will return true
  return true;
}

//The noDoubleParentheses function takes a string and checks whether it contains any double parentheses. It uses a stack to keep track of the previous character encountered and compares it with the current character. If both are parentheses, the function returns false
bool noDoubleParentheses(string toCheck)
{
  int size = toCheck.size();
  stack<char> stackOfChars;

  for (int i = 0; i < size; i++)
  {
    //Compares two previous character of string
    //with current character of string
    //if both are parentheses, double parentheses found
    if (!stackOfChars.empty() && areParentheses(stackOfChars.top(), toCheck[i]))
    {
      return false;
    }
    stackOfChars.push(toCheck[i]);
  }
  //if no errors encountered, will return true—no double parentheses
  return true;
}

//The correctOrder function takes a string and checks whether its parentheses are in the correct order of precedence. It uses a stack to keep track of the open parentheses encountered and compares the precedence level of the current open parenthesis with the top of the stack
bool correctOrder(string toCheck)
{
  int size = toCheck.size();
  stack<char> stackOfChars;

  for (int i = 0; i < size; i++)
  {
    //if closed parentheses are encountered, it pops the top—under the assumption that the top is matching parentheses
    //This assumption is only made because of the order in which the checks happen in main():
    //correct() function is called last, so it wouldn't even be reached if string didn't have valid parentheses
    //because of this, to pop first or to push first wouldn't matter
    if (!stackOfChars.empty() && toCheck[i] == ')' || toCheck[i] == ']' || toCheck[i] == '}')
      stackOfChars.pop();

    //ensures that only open parentheses characters are checked
    if (toCheck[i] == '(' || toCheck[i] == '[' || toCheck[i] == '{')
    {
      if (!stackOfChars.empty() && precedence(toCheck[i]) > precedence(stackOfChars.top()))
      {
        return false;
      }
      stackOfChars.push(toCheck[i]);
    }
  }

  //functions returns true if precedence is valid throughout string
  return true;
}

//for file output
int main(int argc, char* argv[])
{
  ArgumentManager am(argc,argv);
  string infileName = am.get("input"); 
  string outfileName = am.get("output");

  ifstream inFile(infileName);
  ofstream outFile(outfileName);

  queue<string> queueOfInput = inputProcessor(inFile);
  queue<string> queueOfValid;
  queue<string> queueOfInvalid;

  //separation into a list of valid and invalid strings
  while (!queueOfInput.empty())
  {
    if (validParentheses(queueOfInput.front()) 
       && noDoubleParentheses(queueOfInput.front()) 
       && correctOrder(queueOfInput.front()))
    {
      queueOfValid.push(queueOfInput.front());
      queueOfInput.pop();
    }
    else
    {
      queueOfInvalid.push(queueOfInput.front());
      queueOfInput.pop();
    }
  }

  //print list of valid strings
  if (!queueOfValid.empty())
  {
    outFile << "Valid" << endl;
    while (!queueOfValid.empty())
    {
      outFile << queueOfValid.front() << endl;
      queueOfValid.pop();
    }
  }

  //print list of invalid strings
  if (!queueOfInvalid.empty())
  {
    outFile << "Invalid" << endl;
    while (!queueOfInvalid.empty())
    {
      outFile << queueOfInvalid.front() << endl;
      queueOfInvalid.pop();
    }
  }
}