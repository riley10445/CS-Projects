#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

using namespace std;
 
class Node {
public:
    char character;
    int frequency;
    Node *left, *right;

    Node(char character, int frequency) {
        this->character = character;
        this->frequency = frequency;
        left = nullptr;
        right = nullptr;
    }
};

class QueueCompare {
public:
    bool operator()(Node* x, Node* y) {
        return x->frequency > y->frequency;
    }
};

Node* huffmanTree(priority_queue<Node*, vector<Node*>, QueueCompare> minQueue){
    while (minQueue.size() != 1) {
        Node* left = minQueue.top();
        minQueue.pop();

        Node* right = minQueue.top();
        minQueue.pop();

        Node* parent = new Node('0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        minQueue.push(parent);
    }

    return minQueue.top();
}

void huffmanCodes(Node* root, string code, map<char, string>& huffmanCode) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        huffmanCode[root->character] = code;
    }

    huffmanCodes(root->left, code + "0", huffmanCode);
    huffmanCodes(root->right, code + "1", huffmanCode);
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

int main(int argc, char *argv[]) {
    ifstream file(argv[1]);

    map<char, int> frequencies;

    priority_queue<Node*, vector<Node*>, QueueCompare> minQueue;

    char ch;
    while (file.get(ch)) {
        frequencies[ch]++;
    }

    file.close();

    for (auto pair : frequencies) {
        Node* newNode = new Node(pair.first, pair.second);

        minQueue.push(newNode);
    }

    Node* tree = huffmanTree(minQueue);

    map<char, string> huffmanCodeList;
    huffmanCodes(tree, "", huffmanCodeList);

    long int compressedLength = 0;
    for (auto pair : frequencies) {
        char character = pair.first;
        int frequency = pair.second;
        int codeLength = huffmanCodeList[character].length();
        
        compressedLength += frequency * codeLength;
    }
   
    long int uncompressedLength = 0;
    for (auto pair : frequencies) {
        uncompressedLength += pair.second;
    }

    uncompressedLength *= 8;

    cout << "Uncompressed length: " << uncompressedLength << " bits" << endl;
    cout << "Compressed length: " << compressedLength << " bits" << endl;
    cout << "CHAR|FREQ|CODE_LEN" << endl;
    for (auto pair : frequencies) {
        char character = pair.first;
        int frequency = pair.second;
        int codeLength = huffmanCodeList[character].length();

        cout << "'" << character << "'|" << frequency << "|" << codeLength << endl;
    }

    deleteTree(tree);

    return 0;
}
