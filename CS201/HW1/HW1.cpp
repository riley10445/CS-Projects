/*
 Name: Riley Fee
 Email: rfee@crimson.ua.edu
 Course Section: Fall 2024 CS 201
 Homework #: 1
 To Compile: Make sure you are in the same directory with csort.cpp and input(x).txt. Use the command 
 "g++ -std=c++11 -o csort.exe csort.cpp" to compile the program if on Windows. 
 To Run: Type ".\csort.exe input(x).txt" in the command line to run the program. The input(x) file will contain time taken information as well as the sorted list.
*/

#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>

using namespace std;

/*
A custom comparison function to use with the standard sort function, 
taking in the sorted list of words by count, then comparing them based on the count and then based on the word.
*/
bool compare(pair<int, string> one, pair<int, string> two) {
    if (one.first != two.first)
        if(one.first > two.first)
            return true;
        else
            return false;
    else
        if(one.second < two.second)
            return true;
        else
            return false;
}

int main(int argc, char *argv[]){
    vector<string> uniqueWords;
    vector<int> wordCount;
    vector<pair<int, string>> wordPairs;
    vector<string> wordList;
    
    int beginningWord = 0;
    int endWord;
    int tempNum;

    string word;
    string tempWord;

    // Open the input file
    string inputFilename = argv[1];
    ifstream fileRead;
    fileRead.open(inputFilename);

    // Read in the words from the input file
    while (fileRead >> word) {
        wordList.push_back(word);
    }

    fileRead.close();

    auto starttime = chrono::steady_clock::now(); // Start time
    std::sort(wordList.begin(), wordList.end()); // Sort the list of words using the standard sort function

    // Push unique words into a vector and then count the number of times they appear
    int totalWords = 1;
    uniqueWords.push_back(wordList[0]);
    wordCount.push_back(1);
    for(int i = 1; i < wordList.size(); i++){
        if(wordList[i] == uniqueWords.back()){
            wordCount.back()++;
            totalWords++;
        }
        else{
            uniqueWords.push_back(wordList[i]);
            wordCount.push_back(1);
            totalWords++;
        }
    }

    // Sort the unique words by the number of times they appear, first by the number of times they appear and then by the word itself
    for(int i = 0; i < uniqueWords.size(); i++){
        wordPairs.push_back(make_pair(wordCount[i], uniqueWords[i]));
    }
    std::sort(wordPairs.begin(), wordPairs.end(), compare);

    auto endtime = chrono::steady_clock::now(); // End time
    chrono::duration<double> timetaken = endtime - starttime; // Calculate time taken

    // Write the time taken and the sorted list to the input file
    ofstream fileWrite;
    fileWrite.open(inputFilename);
    fileWrite << "Time taken: " << timetaken.count() << "s" << endl;
    fileWrite << "Total words: " << uniqueWords.size() << endl;
    for(int i = 0; i < uniqueWords.size(); i++){
        fileWrite << wordPairs[i].second << ": " << wordPairs[i].first << endl;
    }
    fileWrite.close();

    return 0;
}