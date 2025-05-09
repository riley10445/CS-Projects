// Program to test red-black tree implementation
// To compile: g++ -std=c++20 RBTMain.cpp

#include <iostream>
#include <string>
#include <vector>

#include "RBT.hpp"
#include "RBTPrint.hpp"

using namespace std;

void SpaceSplit(const string& str, vector<string>& output) {
   size_t startIndex = 0;
   size_t endIndex = str.find(' ', startIndex);
   while (endIndex != string::npos) {
      // Only push non-empty strings
      if (endIndex > startIndex) {
         output.push_back(str.substr(startIndex, endIndex - startIndex));
      }
      
      // Get the next start and end indices
      startIndex = endIndex + 1;
      endIndex = str.find(' ', startIndex);
   }
   output.push_back(str.substr(startIndex));
}

int main() {
   cout << "Enter insert keys with spaces between: ";
   string userKeysStr;
   getline(cin, userKeysStr);
   cout << endl;
   
   cout << "Enter insert values with spaces between: ";
   string userValuesStr;
   getline(cin, userValuesStr);
   cout << endl;
   
   // Split the user input
   vector<string> userKeys;
   SpaceSplit(userKeysStr, userKeys);
   vector<string> userValues;
   SpaceSplit(userValuesStr, userValues);

   // Create an RedBlackTree object and add the values
   RedBlackTree<string, int> tree;
   for (int i = 0; i < userKeys.size(); i++) {
      tree.Insert(userKeys[i], stoi(userValues[i]));
   }
  
   // Display the height and tree after all inserts are complete
   cout << "Red-black tree with " << tree.GetLength();
   cout << " nodes has height " << tree.GetHeight() << endl;
   cout << RBTPrint<string, int>::TreeToString(tree.GetRoot()) << endl;

   // Read user input to get a list of values to remove
   cout << "Enter remove keys with spaces between: ";
   getline(cin, userKeysStr);
   cout << endl;
   
   // Split the user input
   userKeys.clear();
   SpaceSplit(userKeysStr, userKeys);

   for (string key : userKeys) {
      string toRemove = key;
      cout << "Remove node " << key << ": ";
      if (tree.Remove(toRemove)) {
         cout << "Red-black tree with " << tree.GetLength();
         cout << " nodes has height " << tree.GetHeight() << endl;
      }
      else {
         cout << "*** Key not found. Tree is not changed. ***" << endl;
      }
   }
      
   // Print the tree
   cout << RBTPrint<string, int>::TreeToString(tree.GetRoot()) << endl;

}
