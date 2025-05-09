// To Compile: g++ -std=c++20 HW3.cpp
// To Run: ./a.out dbfile(x).txt query.txt output.txt

// NOTE: THIS PROGRAM OUTPUTS TO A FILE CALLED output.txt. 
// IT WILL OVERWITE ANY FILE WITH THE SAME NAME OR CREATE A NEW FILE IF IT DOES NOT EXIST.

// NOTE2: I HAD A PROBLEM WITH THE ANTIVIRUS, SO YOU MAY HAVE TO DISABLE WINDOWS REAL-TIME PROTECTION
// TO RUN THE PROGRAM. I APOLOGIZE FOR THE INCONVENIENCE. I EXPECT THIS IS BECAUSE OF THE OUTPUT FILE.

/*  
   Name: Riley Fee
   Email: rfee@crimson.ua.edu
   Course Section: Fall 2024 CS 201
   Homework #: 3
*/  

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {

	if (argc != 4) {
		cout << "Usage: " << argv[0] << " dbfile queryfile output.txt" << endl;
		exit(-1);
	}

	auto starttime = chrono::steady_clock::now();

	string line, name;
	regex delim("/");
	ifstream dbfile(argv[1]);
	if (!dbfile.is_open()) {
		cout << "Unable to open file: " << argv[1] << endl;
		exit(-1);
	}

	unordered_map<string, vector<string>> movieSearch;
	unordered_map<string, vector<string>> actorSearch;
	while (getline(dbfile, line)) {
		auto begin = sregex_token_iterator(line.begin(), line.end(), delim, -1);
		auto movie = *begin;
		auto end = sregex_token_iterator();
		++begin;
		//Creates 2 maps, one for movies and one for actors. The key is the movie or actor name and the value is a vector of the other
		for (sregex_token_iterator word = begin; word != end; ++word) {
			movieSearch[movie].push_back(*word);
			actorSearch[*word].push_back(movie);
		}
	}
	dbfile.close();

	auto dbendtime = chrono::steady_clock::now();
	chrono::duration<double> dbtimetaken = dbendtime - starttime;

	ifstream queryfile(argv[2]);
	if (!queryfile.is_open()) {
		cout << "Unable to open file: " << argv[2] << endl;
		exit(-1);
	}

	ofstream outputfile(argv[3]);
	if (!outputfile.is_open()) {
		cout << "Unable to open file: output.txt" << endl;
		exit(-1);
	}

	auto searchstarttime = chrono::steady_clock::now();

	// Change outputfile to cout to print to console
	while (getline(queryfile, name)) {
		if (name.find('(') != string::npos){  //If the name contains a '(' then it is a movie
			auto foundMovie = movieSearch.find(name);
			if(foundMovie == movieSearch.end()){
				outputfile << name << " not found" << endl;
			}
			else{
				outputfile << name << " stars:" << endl;
				for(int i = 0; i < foundMovie->second.size(); i++){
					outputfile << "\t" << foundMovie->second[i] << endl;
				}
			}
	   }
		else { //If the name does not contain a '(' then it is an actor
			auto foundActor = actorSearch.find(name);
			if(foundActor == actorSearch.end()){
				outputfile << name << " not found" << endl;
			}
			else{
				//Outputs all of the movies in the actorSearch map that the actor has acted in
				outputfile << name << " acted in:" << endl;
				for(int i = 0; i < foundActor->second.size(); i++){
					outputfile << "\t" << foundActor->second[i] << endl;
				}
			}
		}
	}
	queryfile.close();

	auto searchendtime = chrono::steady_clock::now();
	chrono::duration<double> searchtimetaken = searchendtime - searchstarttime;

	// Change outputfile to cout to print to console
	outputfile << "Time taken to generate data structure: " << dbtimetaken.count() << "s" << endl;
	outputfile << "Time taken to process queries: " << searchtimetaken.count() << "s" << endl;
	outputfile << "Total time taken: " << dbtimetaken.count() + searchtimetaken.count() << "s" << endl;

	outputfile.close();
	return 0;
}
