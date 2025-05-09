#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
#include <sstream>

using namespace std;

class Info {
public:
    string name;
    int from;
    int to;
};

int getIndex(string& identifier, Info bandwidthInfo[], int &size) {
    int index = 0;
    while (index < size) {
        if (bandwidthInfo[index].name == identifier) {
            return index;
        }
        ++index;
    }
    if (index == size) {
        bandwidthInfo[size].name = identifier;
        ++size;
    }
    return index;
}

void makeUpper(string& identifier) {
    for (int i = 0; i < identifier.size(); ++i) {
        identifier[i] = toupper(identifier[i]);
    }
}

void outputData(ofstream &output, Info bandwidthInfo[], int size) {
    for (int i = 0; i < size; i++) {
        output << bandwidthInfo[i].name << "\n";
    }
    output << "\n";
    for (int i = 0; i < size; i++) {
        bool isNonLeaf = bandwidthInfo[i].from != 0 && bandwidthInfo[i].to != 0;
        if (isNonLeaf) {
            double divided = bandwidthInfo[i].to / (double)bandwidthInfo[i].from;
            output << fixed << setprecision(2) << bandwidthInfo[i].name << " ";
            output << bandwidthInfo[i].from << " ";
            output << bandwidthInfo[i].to << " ";
            output << divided << "\n";
        }
    }
}

int main(int argc, char *argv[]) {
    ifstream input(argv[1]);
    ofstream output(argv[2]);

    string line;
    string source;
    string destination;
    int bandwidth;
    int size = 0;
    Info bandwidthIdentifier[1000];

    while (getline(input, line)) {
        istringstream iss(line);
        iss >> source >> destination >> bandwidth;
        makeUpper(source);
        makeUpper(destination);
        int sourceIndex = getIndex(source, bandwidthIdentifier, size);
        int destIndex = getIndex(destination, bandwidthIdentifier, size);
        bandwidthIdentifier[sourceIndex].from += bandwidth;
        bandwidthIdentifier[destIndex].to += bandwidth;
    }

    outputData(output, bandwidthIdentifier, size);
    input.close();
    output.close();

    return 0;
}
