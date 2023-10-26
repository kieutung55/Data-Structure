// FindString.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "ArgumentManager.h"

using namespace std;

/* recusive function to search taget string in given character matrix.
board: given character matrix
r, c:  start element of matrix for search (r: row, c:column)
target: string to be searched by
index: this function finds substring('index'th character to latest character) of target 

for example:
find(board, 0, 0, "apple", 0) will return boolean value if there are exist "apple" starts with (0, 0) element
find(board, 2, 5, "apple", 2) will return boolean value if there are exist "ple" starts with (2, 5) element
*/
bool find(vector <vector<char>>board, int r, int c, string target, int index) {
    if (index == target.length())
    {
        return true;
    }
    
    if (r < 0 || c < 0 || r >= board.size() || c >= board[0].size() || target[index] != board[r][c])
    {
        return false;
    }
    //avoid going back back to the previous position
    char temp = board[r][c];
    board[r][c] = ' ';

    //call recursive function
    bool found = false;
    // There are 4 directions to be searched from current position(r, c)
    // (r-1, c):North, (r, c+1):East, (r+1, c):South, (r, c-1):West
    found = find(board, r - 1, c, target, index + 1);                   // North direction
    if(!found) {                // if not found in North direction
        found = find(board, r, c + 1, target, index + 1);               // East Direction
        if (!found) {           // if not found in East direction
            found = find(board, r + 1, c, target, index + 1);           // South Direction
            if (!found) {       // if not found in South direction
                found = find(board, r, c - 1, target, index + 1);       // West Direction
            }
        }
    }  

    board[r][c] = temp; // recover original board

    return found;
}

int main(int argc, char* argv[])
{
    // Parse arguments using ArgumentManager.h
    ArgumentManager AM = ArgumentManager(argc, argv, ';');
    string infilename = AM.get("input");        // file name for input
    string outfilename = AM.get("output");      // file name for output

    int col_cnt = 0;
    int row_cnt = 0;
    string target = "";
    vector<vector<char>> board = {};
    ifstream infile;                    // filestream for reading of input file
    ofstream outfile;                   // filestream for writing to output file
    infile.open(infilename);
    
    // ============= Load all informations for input ======================
    string line = "";                   // a line in input file
    // ------------ parse and load number of row and column respectively --------------------
    if (getline(infile, line)) {        // get and store a line from input file. // if there is line to be parsed, pass  
        stringstream ss(line);
        for (int i = 0; i < 2; i++) {
            int token;
            if (ss >> token) {          // if there are token to be parsed, 
                if (i == 0) {           // this token is for row count
                    row_cnt = token;
                }
                else if (i == 1) {      // this token is for row count
                    col_cnt = token;
                }
            }
            else {                      // if there is no exist desired token, raise error and exit program
                cout << "input file is invalid" << endl;
                return 0;
            }            
        }  
        for (int r = 0; r < row_cnt; r++) {
            if (getline(infile, line)) {    // if there is line in the current file stream, pass
                stringstream ss(line);
                vector<char> row_vector = {};       // decrare a vector of characters corresponding to a row of board
                for (int c = 0; c < col_cnt; c++) {
                    string token;           // a character in the current text line, line
                    if (ss >> token) {      // if there are token to be parsed for a character, 
                        row_vector.push_back(token[0]);
                    }
                    else {
                        cout << "input file is invalid" << endl;
                        return 0;
                    }
                }
                board.push_back(row_vector);
            }
            else {
                cout << "input file is invalid" << endl;
                return 0;
            }
        }
        infile >> target;
    }
    else {
        cout << "input file is invalid" << endl;
        return 0;
    }
    infile.close();                 // close filestream of input file

    bool found = false;             // result indicating target exists or not
    // iterate for each character in the board
    for (int r = 0; r < row_cnt; r++) {
        for (int c = 0; c < col_cnt; c++) {
            found = find(board, r, c, target, 0);   
            if (found) break;                       // if target string have been found, return the result. no need to iterate the board anymore
        }
        if (found) break;                           // if target string have been found, return the result. no need to iterate the board anymore
    }
    outfile.open(outfilename);      // write the result into outfile stream
    if (found) {
        outfile << "true";
    }
    else {
        outfile << "false";
    }    
    outfile.close();    
    return 0;
}