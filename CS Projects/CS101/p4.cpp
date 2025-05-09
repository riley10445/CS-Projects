#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <list>

using namespace std;

uint64_t djb2(string str) {
    const char *ptr = str.c_str();
    uint64_t hash = 5381;
    int c;
    while ((c = *ptr++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

class Person {
public:
    string id;
    string first;
    string last;
    string email;
    string phone;
    string city;
    string state;
    string postalCode;
};

class HashTable {
private:
    vector<list<Person> > table;

    uint64_t primaryHash(string str, int size) {
        return djb2(str) % size;
    }

    uint64_t secondaryHash(string str) {
        return 43 - (djb2(str) % 43);
    }
    
    string getKeyString(Person& person, string& keyType) {
        if (keyType == "Id") return person.id;
        else if (keyType == "FirstName") return person.first;
        else if (keyType == "LastName") return person.last;
        else if (keyType == "Email") return person.email;
        else if (keyType == "Phone") return person.phone;
        else if (keyType == "City") return person.city;
        else if (keyType == "State") return person.state;
        else if (keyType == "PostalCode") return person.postalCode;
    }

public:
    HashTable(int size) : table(size) {}

    void insert(Person person, string key) {
        string keyValue = getKeyString(person, key);

        uint64_t primaryIndex = primaryHash(keyValue, table.size());
        uint64_t secondaryIndex = secondaryHash(keyValue);

        while (!table[primaryIndex].empty() && getKeyString(table[primaryIndex].front(), key) != keyValue) {
            primaryIndex = (primaryIndex + secondaryIndex) % table.size();
        }

        table[primaryIndex].push_back(person);
    }

    void print(string key) {
        for (int index = 0; index < table.size(); ++index) {
            if (!table[index].empty()) { 
                cout << index << ": ";
                string key_value_printed;

                for (Person& person : table[index]) {
                    string current_key_value;
                    current_key_value = getKeyString(person, key);

                    if (current_key_value != key_value_printed) {
                        if (!key_value_printed.empty()) {
                            cout << endl;
                        }
                        cout << current_key_value;
                        key_value_printed = current_key_value;
                    }
                }
                cout << " (" << table[index].size() << ")," << endl;
            }
        }
    }


    void lookup(string key, string keyValue) {
        bool found = false;
        for (int index = 0; index < table.size(); ++index) {
            for (Person& person : table[index]) {
                string current_key_value;
                current_key_value = getKeyString(person, key);
                if (current_key_value == keyValue) {
                    if (!found) {
                        cout << "Id,FirstName,LastName,Email,Phone,City,State,PostalCode" << endl;
                        found = true;
                    }
                    cout << person.id << ", " << person.first << ", " << person.last << ", "
                        << person.email << ", " << person.phone << ", " << person.city << ", "
                        << person.state << ", " << person.postalCode << endl;
                }
            }
        }
        if (!found) {
            cout << "No results" << endl;
        }
    }

};

void getVal(istream &is, string &str) {
    char ch;
    string line;    

    is >> ch;
    getline(is, line);

    str = ch + line;
}

bool isValidKey(const string &key) {
    string validKeys[8] = { "Id", "FirstName", "LastName", "Email", "Phone", "City", "State", "PostalCode" };
    for (int i = 0; i < 8; i++) {
        if (key == validKeys[i]) {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: ./a.out filename.txt table_size key" << endl;
        return 1;
    }

    string filename = argv[1];
    int tableSize = stoi(argv[2]);
    string key = argv[3];

    ifstream file(filename);
    if (!file) {
        cout << "Unable to open " << filename << endl;
        return 2;
    }

    if (!isValidKey(key)) {
        cout << "Invalid key: " << key << endl;
        return 3;
    }
   
    // This is an example of how to retreive the tokens from the input file
    // You will need to modify this to fit your needs to build the hash table
    HashTable hashTable(tableSize); 
    string line, token;
    getline(file, line); // consume header line
    string id, first, last, email, phone, city, state, postalCode;
    while (getline(file, line)) {
        Person person;
        istringstream iss(line);
        getline(iss, person.id, '\t');
        getline(iss, person.first, '\t');
        getline(iss, person.last, '\t');
        getline(iss, person.email, '\t');
        getline(iss, person.phone, '\t');
        getline(iss, person.city, '\t');
        getline(iss, person.state, '\t');
        getline(iss, person.postalCode, '\t');
        hashTable.insert(person, key);
    }

    cout << "Commands:" << endl << "\tprint" << endl << "\tlookup <key>" << endl << "\tquit" << endl;
    string cmd, val;
    while (1) {
        cout << endl << "Enter a command:" << endl;
        cin >> cmd;
        if (cmd == "quit") {
            break;
        }
        else if (cmd == "print") {
            hashTable.print(key);
        }
        else if (cmd == "lookup") {
            getVal(cin, val);
            hashTable.lookup(key, val);
        }
        else {
            getline(cin, val);
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}
