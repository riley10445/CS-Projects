/*
 Name: Riley Fee
 Email: rfee@crimson.ua.edu
 Course Section: Fall 2024 CS 201
 Homework #: 2
 To Compile: Make sure you are in the same directory with RBTSort.cpp and input(x).txt. Use the command 
 "g++ -std=c++11 -o RBTSort.exe RBTSort.cpp" to compile the program if on Windows. 
 To Run: Type ".\RBTSort.exe input(x).txt" in the command line to run the program. The input(x) file will contain time taken information as well as the sorted list.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include "ST.hpp"

using namespace std;

bool compare(pair<string, int>& one, pair<string, int>& two) {
    if (one.second != two.second)
        return one.second > two.second;
    else
        return one.first < two.first;
}

int main(int argc, char *argv[]) {
    ST<string, int> wordCounts;

    string word;
    string inputFilename = argv[1];

    ifstream fileRead(inputFilename);

    auto starttime = chrono::steady_clock::now();

    while (fileRead >> word) {
        if (wordCounts.contains(word)) {
            wordCounts[word]++;
        } else {
            wordCounts.insert(word, 1);
        }
    }

    fileRead.close();

    vector<pair<string, int>> wordPairs;
    vector<pair<string, int>> temporary = wordCounts.toVector();
    for (int i = 0; i < temporary.size(); i++) {
        wordPairs.push_back(make_pair(temporary[i].first, temporary[i].second));
    }

    sort(wordPairs.begin(), wordPairs.end(), compare);

    auto endtime = chrono::steady_clock::now();
    chrono::duration<double> timetaken = endtime - starttime;

    ofstream fileWrite(inputFilename);

    fileWrite << "Time taken: " << timetaken.count() << "s" << endl;
    fileWrite << "Total unique words: " << wordCounts.size() << endl;
    for (int i = 0; i < wordPairs.size(); i++) {
        fileWrite << wordPairs[i].first << ": " << wordPairs[i].second << endl;
    }

    fileWrite.close();

    return 0;
}