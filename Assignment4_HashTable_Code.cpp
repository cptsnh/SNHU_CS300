//============================================================================
// Name        : HashTable.cpp
// Author      : Chris Trimmer with base code from SNHU
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;
//const unsigned int DEFAULT_SIZE = 10000;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
            next = nullptr;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
            next = nullptr;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

using std::cout;

/**
 * Default constructor
 */
HashTable::HashTable() {

    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {

    // initialize tableSize with size parameter and resize the table
    tableSize = size;
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {

    // erase all nodes in vector of nodes
    nodes.erase(nodes.begin(), nodes.end());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {

    // return the hash value determined by the key modulus tableSize
    return key % tableSize;

}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {

    // create variable to store the hash of the bidid
    unsigned key = hash(std::stoi(bid.bidId));

    // retrieve the node at that key
    Node* node = &nodes.at(key);

    // if there is no entry, then assign the node here
    if (node == nullptr) {
        this->nodes.assign(key, bid);
    }

    // else, set the node with the key and bid, and assign next pointer to null
    else if (node->key == UINT_MAX) {
        node->key = key;
        node->next = nullptr;
        node->bid = bid;
    }

    // else, walk the list until we get to the next open node
    else {
        while (node->next != nullptr) {
            node = node->next;
        }

        // assign the node to the end of the vector at this bucket
        node->next = new Node(bid, key);

    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {

    // set a count variable
    int count = 0;

    // loop through the nodes
    for (int i = 0; i < nodes.size(); ++i) {

        // if the key is not an empty bucket, then start printing the nodes
        if (nodes.at(i).key != UINT_MAX) {
            std::cout << nodes.at(i).key << ":  " << nodes.at(i).bid.bidId << "\n";
            count++;
            
            Node* temp = this->nodes.at(i).next;

            // walk through the list of sub-nodes at this bucket
            while (temp != nullptr) {
                std::cout << "\t--> " << temp->bid.bidId << "\n";
                count++;
                temp = temp->next;
            }
        }
    }

    // output the final count
    std::cout << "\nFinal count: " << count << "\n\n";
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {

    // create a variable to store the hash of the bidid
    unsigned bidIdKey = this->hash(std::atoi(bidId.c_str()));

    // get the index where the bid is located
    Node* index = &nodes.at(bidIdKey);

    // if there is only a node here, then erase it
    if (index->next == nullptr) {

        std::cout << "\n\n\t[[ " << bidId << " deleted ]]\n\n";

        nodes.erase(nodes.begin() + bidIdKey);
        index->key = UINT_MAX;
        return;
    }

   
    // if index is the head node, we need to move the chain to the index node
    if (index->bid.bidId == bidId) {

        // create pointer to the next node
        Node* temp = index->next;
        
        // assign that node to be at that bucket
        nodes.at(bidIdKey) = *temp;

        std::cout << "\n\n\t[[ " << bidId << " deleted ]]\n\n";

        // now delete the node
        delete temp;

        return;
    }

    // if there are multiple bids at this bucket, then remove the correct one
    Node* curr = index->next;

    // walk the list until we find the correct bid
    while (curr != nullptr) {
        if (bidId == curr->bid.bidId) {

            // if we found it, then set it to a temp variable and re-assign the pointer
            Node* temp = curr;
            index->next = temp->next;
            
            cout << "\n\n\t[[ " << bidId << " deleted ]]\n\n";

            // now delete it
            delete temp;
            temp = nullptr;

            return;
        }
        else {
            index = index->next;
            curr = curr->next;
        }
    }

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // hash the incoming bidId we are searching for
    unsigned bidIdKey = this->hash(std::atoi(bidId.c_str()));

    // create pointer to the index that matches the bid key
    Node* temp = &(nodes.at(bidIdKey));

    // if the node exists and matches the key, then return the bid that was found
    if (temp != nullptr && temp->key != UINT_MAX && temp->bid.bidId == bidId) {
        return temp->bid;
    }

    // if there is no entry for the key, then return the dummy bid
    if (temp == nullptr || temp->key == UINT_MAX) {
        return bid;
    }

    // walk the sublists looking for the bidId we are searching for
    // if we find it, then return the respective bid
    while (temp != nullptr) {
        if (temp->key != UINT_MAX && temp->bid.bidId == bidId) {
            return temp->bid;
        }

        temp = temp->next;
    }

    // if we walk the whole list and don't find the bid, then return the dummy bid
    return bid;

}

//============================================================================
// Static methods used for testing
//============================================================================


Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    getline(cin, bid.fund);

    cout << "Enter amount: ";
    cin.clear();
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}



/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {

        // create a count variable
        int count = 0;

        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // display the bid and increment the count
            displayBid(bid);
            count++;

            // Insert bid to hash table
            hashTable->Insert(bid);

        }

        cout << count << " records read\n\n";

    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98029";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    // create temp bids used in the menu loop
    Bid bid;
    Bid searchBid;

    string input;
    bidTable = new HashTable();

    // print the main menu and get user input
    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Search Bid" << endl;
        cout << "  4. Add bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        // load the bids
        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        // print the bids
        case 2:

            // Initialize a timer variable before loading bids
            ticks = clock();

            // call to print the bids
            bidTable->PrintAll();

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        // search the bids
        case 3:

            cout << "\nEnter the bid id: ";
            cin.ignore();
            getline(cin, input);
            ticks = clock();

            bid = bidTable->Search(input);


            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                cout << "\n";
                displayBid(bid);
                cout << "\n";
            } else {
                cout << "\nBid Id " << input << " not found.\n" << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;


        // insert a bid
        case 4:

            // get bid info from user input
            bid = getBid();

            // search to make sure the bid does not already exist
            searchBid = bidTable->Search(bid.bidId);
            if (!searchBid.bidId.empty()) {
                cout << "\n\nCannot enter a duplicate bid id\n\n";
                break;
            }
            
            // insert the new bid and display it
            if (!bid.bidId.empty()) {

                bidTable->Insert(bid);

                Bid temp = bidTable->Search(bid.bidId);
                cout << "\n\nBid inserted:\n";
                displayBid(temp);
                cout << "\n";
            }


            break;


        // remove a bid
        case 5:

            // get the bid id from the user input
            cout << "Enter a bid id: ";
            cin.ignore();
            getline(cin, input);

            // search to see if the bidid exists
            bid = bidTable->Search(input);

            // if it exists, then remove it
            if (!bid.bidId.empty()) {
                bidTable->Remove(input);
            }
            else {
                cout << "\nBid id " << input << " not found." << endl;
            }

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
