#include <iostream>
#include <string>
#include <utility>

#include "ST.hpp"
#include "RBTPrint.hpp"

int main(int argc, char *argv[]) {
	// use default constructor
	ST<std::string, int> st;

	// use [] operator to insert new (key,value) pair
	st["alpha"] = 1;
	st["beta"] = 2;
	st["theta"] = 3;
	st["delta"] = 10;
	st["gamma"] = 11;

	// use [] operator to update an existing key with new value
	st["delta"] = 4;

	// use count method
	std::cout << "no. of nodes with key = Red: " << st.count("delta") << std::endl;

	// use [] operator to get a reference to the value and update it
	st["gamma"]++;

	// use insert method
	st.insert("sigma", 6);
	st.insert("theta", 5);

	// display symbol table as red-black tree
	st.displayTree();

	// use remove method
	st.remove("alpha");

	// display symbol table as red-black tree
	std::cout << "after removing alpha" << std::endl;
	st.displayTree();

	// display symbol table as (key,value) pair
	std::cout << "symbol table as (key, value) pair" << std::endl;
	st.display();

	// use size method
	std::cout << "no. of nodes = " << st.size() << std::endl;

	// use contains method
	std::cout << "alpha present? " << std::boolalpha << st.contains("alpha") << std::endl;

	// use find method and print subtree from that position
	std::cout << RBTPrint<std::string,int>::TreeToString(st.find("beta")) << std::endl;

	// user toVector to create a vector of (key,value) pairs
	std::vector<std::pair<std::string, int> > vec = st.toVector();

	// display vector
	for (const auto & p : vec)
	    std::cout << p.first << ": " << p.second << std::endl;

	// use clear method to delete all nodes
	st.clear();

	// use empty method to check if the ST is empty
	std::cout << "tree empty? " << std::boolalpha << st.empty() << std::endl;

	return 0;
}
