/* 
Name: Riley Fee
Email: rfee@crimson.ua.edu
Course Section: Fall 2024 CS 201 
Homework #: 4
To Compile: Run g++ -Wall -std=c++20 HW4.cpp
To Run: Run ./a.out input1.txt
		Run ./a.out input2.txt
*/ 


#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <limits>

class Vertex {
public:
	bool visited;
	int distance;
	int previous;
	int finish;
	std::list<int> adj;
};

class Graph {
public:
	Graph(int V, int E, std::pair<int, int> *edges) {
		_V = V;
		_E = E;
		vertices = new Vertex[_V];
		for (int i = 0; i < _V; i++) {
                    vertices[i].visited = false;
                    vertices[i].distance = 65535; // Instead using INFINITY, you can use a large int number to initialize your graph
                    vertices[i].previous = -1;
		}
		for (int i = 0; i < _E; i++) {
		    addEdge(edges[i].first, edges[i].second);
		}
	}

	virtual ~Graph() {
		for (int i=0; i<_V; ++i) {
		    auto adj = vertices[i].adj;
		    adj.clear(); // clear list
		}

		delete[] vertices; // delete array of vertices
	}

	int V() {
		return _V;
	}

	int E() {
		return _E;
	}

	void addEdge(int u, int v) {
		vertices[u].adj.push_back(v);
	}

	std::list<int> getEdges(int u) {
		return vertices[u].adj;
	}

	int degree(int u) {
		return vertices[u].adj.size();
	}

	void bfs(int s) {
        std::list<int> queue;

        queue.push_back(s);
		vertices[s].visited = true;

        while (queue.empty() == false) {
            int u = queue.front();
            queue.pop_front();
			
            for (int v : vertices[u].adj) {
                if (vertices[v].visited == false) {
                    vertices[v].visited = true;
                    vertices[v].previous = u;
                    queue.push_back(v);
                }
            }
        }
	}

	void dfs() {
		for (int i = 0; i < _V; i++) {
            vertices[i].visited = false;
        }

        for (int u = 0; u < _V; u++) {
            if (vertices[u].visited == false) {
                dfs_visit(u);
            }
        }
	}

	void dfs_visit(int u) {

        vertices[u].visited = true;
        std::cout << u << " ";

        for (int v : vertices[u].adj) {
            if (vertices[v].visited == false) {
                vertices[v].previous = u;
                dfs_visit(v);
            }
        }
        std::cout << u << " ";
	}

	void print_path(int s, int v) {
		if (v == s)
		   std::cout << s;
		else if (vertices[v].previous == -1)
		   std::cout << "not connected";
		else {
		   print_path(s,vertices[v].previous);
		   std::cout << "->" << v;
		}
	}

	std::string toString() {
		std::stringbuf buffer;
		std::ostream os(&buffer);
		os << "Vertices = " << _V << ", Edges = " << _E << std::endl;
		for (int i = 0; i < _V; ++i) {
		    os << i << "(" << degree(i) << "): ";
		    for (const auto& l : vertices[i].adj) 
			os << l << " ";
		    os << std::endl;
		}

		return buffer.str();
	}
private:
	int _V; // no. of vertices
	int _E; // no. of edges
	Vertex *vertices; // array of vertices
};

#endif
