/*
 Name: Riley Fee
 Email: rfee@crimson.ua.edu
 Course Section: Fall 2024 CS 201
 Homework #: 2
 To Compile: Make sure you are in the same directory with RBTSort.cpp and input(x).txt. Use the command 
 "g++ -std=c++11 -o RBTSort.exe RBTSort.cpp" to compile the program if on Windows. 
 To Run: Type ".\RBTSort.exe input(x).txt" in the command line to run the program. The input(x) file will contain time taken information as well as the sorted list.
*/

#ifndef _ST_HPP_
#define _ST_HPP_

#include <utility>

#include "RBT.hpp"
#include "RBTPrint.hpp"

template <class Key, class Type>
class ST : public RedBlackTree<Key, Type> {
public:
	typedef RBTNode<Key, Type>* iterator;

	// Constructors
	// constant
	ST() { 
		nodeCount = 0;
	}; 

	// Destructor
	// linear in the size of the ST
	~ST(){
		clear();
	};

	// access or insert specifies element
	// inserts if the key is not present and returns a reference to
	// the value corresponding to that key
	// O(LogN), N size of ST
	Type& operator[](const Key& key) {
		iterator newIt = this->find(key);
		if(newIt == nullptr) {
			insert(key, Type());
			newIt = this->find(key);
			return newIt->value;
		}
		else{
			return newIt->value;
		}
	}; 

	// insert a (key, value) pair, if the key already exists
	// set the new value to the existing key
	// O(LogN), N size of ST
	void insert(const Key& key, const Type& value) {
		iterator newIt = this->find(key);
		if(newIt == nullptr) {
        	iterator newNode = this->Insert(key, value);
			nodeCount++;
		}
		else{
			newIt->value = value;
		}
	};

	// remove element at the given position
	// amortized constant
	void remove(iterator position) {
		this->RemoveNode(position);
		nodeCount--;
	};

    // remove element with keyvalue key and 
	// return number of elements removed (either 0 or 1)
	// O(logN), N size of ST
	std::size_t remove(const Key& key) {
		iterator newIt = this->find(key);
		if(newIt){
			this->Remove(key);
			nodeCount--;
			return 1;
		}
		else{
			return 0;
		}
	};  

	// removes all elements from the ST, after this size() returns 0
	// linear in the size of the ST
	void clear() {
		this->DeleteTree(this->root);
		this->root = nullptr;
		nodeCount = 0;
	}; 

	// checks if ST has no elements; true is empty, false otherwise
	// constant
	bool empty() const {
		return nodeCount == 0;
	}; 

	// returns number of elements in ST
	// constant
	std::size_t size() const {	
		return nodeCount;
	}; 

	// returns number of elements that match keyvalue key
	// value returned is 0 or 1 since keys are unique
	// O(LogN), N size of ST
	std::size_t count(const Key& key) {
		iterator newIt = this->Search(key);
		if(newIt){
			return 1;
		}
		else{
			return 0;
		}
	}; 

	// find an element with keyvalue key and return 
	// the iterator to the element found, nullptr if not found
	// O(LogN), N size of ST
	iterator find(const Key& key) {
		iterator newIt = this->Search(key);
		return newIt;
	};

	// check if key exists in ST
	// O(LogN), N size of ST
	bool contains(const Key& key) {
		iterator newIt = this->Search(key);
		if(newIt){
			return true;
		}
		else{
			return false;
		}
	};

	// return contents of ST as a vector of (key,value) pairs
	// O(N), N size of ST
	std::vector<std::pair<Key, Type> > toVector() {
		std::vector<std::pair<Key, Type> > vec;
		traverse(this->root, vec);
		return vec;
	};

	// print the symbol table as Red-Black Tree
	// O(N), N size of ST
	void displayTree() {
		std::cout << RBTPrint<Key,Type>::TreeToString(RedBlackTree<Key,Type>::root) << std::endl;
	};

	// print the symbol table in sorted order
	// O(N), N size of ST
	void display() {
		std::vector<std::pair<Key, Type> > vec;
		traverse(this->root, vec);
		for (int i = 0; i < vec.size(); i++) {
			std::cout << vec[i].first << " " << vec[i].second << std::endl;
		}
	};

private:
	std::size_t nodeCount;

	void traverse(iterator root, std::vector<std::pair<Key, Type> >& vec) {
		if(root != nullptr) {
			traverse(root->left, vec);
			vec.push_back(std::make_pair(root->key, root->value));
			traverse(root->right, vec);
		}
	};
};

#endif

