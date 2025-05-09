#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <vector>
#include <cstring>

using namespace std;

void MergeInts(vector<int>& inputs, int i, int j, int k, const vector<int>& indices, int numOfIndices) {
    int mergedSize = k - i + 1;
    vector<int> mergedNumbers(mergedSize);

    int mergePos = 0;
    int leftPos = i;
    int rightPos = j + 1;
   
    while (leftPos <= j && rightPos <= k) {
        bool comparisonMade = false;

        for(int idx = 0; idx < numOfIndices; idx++) {
            int indexToCheck = indices[idx];

            int leftDigit = (inputs[leftPos] / (int)pow(10, indexToCheck)) % 10;
            int rightDigit = (inputs[rightPos] / (int)pow(10, indexToCheck)) % 10;

            if (leftDigit < rightDigit) {
                mergedNumbers[mergePos] = inputs[leftPos];
                ++leftPos;
                comparisonMade = true;
                break;
            } else if (leftDigit > rightDigit) {
                mergedNumbers[mergePos] = inputs[rightPos];
                ++rightPos;
                comparisonMade = true;
                break;
            }
        }

        if (!comparisonMade) {
            mergedNumbers[mergePos] = inputs[leftPos];
            ++leftPos;
        }

        ++mergePos;
    }
   
    while (leftPos <= j) {
        mergedNumbers[mergePos] = inputs[leftPos];
        ++leftPos;
        ++mergePos;
    }
   
    while (rightPos <= k) {
        mergedNumbers[mergePos] = inputs[rightPos];
        ++rightPos;
        ++mergePos;
    }
   
    for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
        inputs[i + mergePos] = mergedNumbers[mergePos];
    }
}

void MergeStrings(vector<string>& inputs, int i, int j, int k, const vector<int>& indices, int numOfIndices) {
    int mergedSize = k - i + 1;
    vector<string> mergedStrings(mergedSize);

    int mergePos = 0;
    int leftPos = i;
    int rightPos = j + 1;
   
    while (leftPos <= j && rightPos <= k) {
        bool comparisonMade = false;

        for(int idx = 0; idx < numOfIndices; idx++) {
            int indexToCheck = indices[idx];

            char leftChar = '\0';
            char rightChar = '\0';
            if (indexToCheck < inputs[leftPos].size()) {
                leftChar = inputs[leftPos][indexToCheck];
            }
            if (indexToCheck < inputs[rightPos].size()) {
                rightChar = inputs[rightPos][indexToCheck];
            }

            if (leftChar < rightChar) {
                mergedStrings[mergePos] = inputs[leftPos];
                ++leftPos;
                comparisonMade = true;
                break;
            } 
            else if (leftChar > rightChar) {
                mergedStrings[mergePos] = inputs[rightPos];
                ++rightPos;
                comparisonMade = true;
                break;
            }
        }

        if (!comparisonMade) {
            mergedStrings[mergePos] = inputs[leftPos];
            ++leftPos;
        }

        ++mergePos;
    }
   
    while (leftPos <= j) {
        mergedStrings[mergePos] = inputs[leftPos];
        ++leftPos;
        ++mergePos;
    }
   
    while (rightPos <= k) {
        mergedStrings[mergePos] = inputs[rightPos];
        ++rightPos;
        ++mergePos;
    }
   
    for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
        inputs[i + mergePos] = mergedStrings[mergePos];
    }
}


void MergeSortInts(vector <int>& inputs, int i, int k, vector <int> indices, int numOfIndices) {
    int j;
    if (i < k) {
        j = (i + k) / 2; 

        MergeSortInts(inputs, i, j, indices, numOfIndices);
        MergeSortInts(inputs, j + 1, k, indices, numOfIndices);

        MergeInts(inputs, i, j, k, indices, numOfIndices);
    }
}

void MergeSortStrings(vector <string>& inputs, int i, int k, vector <int> indices, int numOfIndices) {
    int j;
    if (i < k) {
        j = (i + k) / 2;

        MergeSortStrings(inputs, i, j, indices, numOfIndices);
        MergeSortStrings(inputs, j + 1, k, indices, numOfIndices);

        MergeStrings(inputs, i, j, k, indices, numOfIndices);
    }
}

int main(int argc, char *argv[]){

    //ERROR AND ARGUMENT CHECKING
    if (argc != 5){
        cout << "Usage: ./a.out i|s sort_filename input_filename output_filename" << endl;
        return 0;
    }

    char* datatype = argv[1];
    if (strcmp(datatype, "i") != 0 && strcmp(datatype, "s") != 0){
        cout << "Invalid datatype \"" << datatype << "\" (must be \"i\" or \"s\")" << endl;
        return 0;
    }

    ifstream sortF(argv[2]);
    if(!sortF){
        cout << "Unable to open sort file" << endl;
        return 0;
    }

    ifstream inputF(argv[3]);
    if(!inputF){
        cout << "Unable to open input file" << endl;
        return 0;
    }

    ofstream outputF(argv[4]);
    if(!outputF){
        cout << "Unable to open output file" << endl;
        return 0;
    }

    //GET INDICES TO COMPARE
    vector <int> indices;
    int number;
    while(sortF >> number){
        indices.push_back(number);
    }
    int numOfIndices = indices.size();

    //GET INPUTS, MERGE AND OUTPUT
    if(strcmp(datatype, "s") == 0){
        vector <string> inputs;
        string input;
        while(inputF >> input){
            inputs.push_back(input);
        }
        int numOfInputs = inputs.size();
        MergeSortStrings(inputs, 0, numOfInputs - 1, indices, numOfIndices);
        for(int i = 0; i < numOfInputs; i++){
            outputF << inputs[i] << endl;
        }
    }
    else if(strcmp(datatype, "i") == 0){
        vector <int> inputs;
        int input;
        while(inputF >> input){
            inputs.push_back(input);
        }
        int numOfInputs = inputs.size();
        MergeSortInts(inputs, 0, numOfInputs - 1, indices, numOfIndices);
        for(int i = 0; i < numOfInputs; i++){
            outputF << right << setw(10) << inputs[i] << endl;
        }
    }

    return 0;
}