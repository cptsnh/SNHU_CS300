//============================================================================
// Name        : LinkedList.cpp
// Author      : Chris Trimmer with SNHU provided base code
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();


    bool Empty();
};

// function to return bool based on whether list size is 0 or not
bool LinkedList::Empty() {
    return this->Size() <= 0;
}

/**
 * Default constructor
 */
LinkedList::LinkedList() {

    // set head and tail pointers to null
    head = nullptr;
    tail = nullptr;
    
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {

    // create pointer to the head
    Node* current = head;

    // create temp pointer to hold the node we are going to delete
    Node* temp;

    // walk the list, deleting each node one at a time
    while (current != nullptr) {

        // copy node we are going to delete to temp
        temp = current;

        // set current to the next node
        current = current->next;

        // now delete temp node
        delete temp;

    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {

    // Create new node
    Node* newNode = new Node(bid);

    // if their is not a head node, then the new node is the head and tail
    if (!head) {
        this->head = newNode;
        this->tail = newNode;
    }

    // else the new node is added to the end
    else {
        this->tail->next = newNode;
        this->tail = newNode;
    }

    // increment the size of the list
    this->size += 1;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {

    // if the list is empty, then call append with the bid
    // because append already has logic for adding new node
    // to the empty list
    if (this->Empty()) {
        Append(bid);
        return;
    }

    // create the new node using the new bid
    Node* temp = new Node(bid);

    // next pointer of new node will point to current head
    temp->next = head;

    // make new bid be the new head
    head = temp;

    // increment size of the list
    this->size += 1;

}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {

    // create count variable for counting
    int count{};

    // create pointer to the head node
    Node* currNode = this->head;

    cout << "\nList of bids:\n";

    // walk the list and print each node
    while (currNode != nullptr) {
        cout << currNode->bid.bidId << ": "
            << currNode->bid.title << " | " 
            << currNode->bid.amount << " | "
            << currNode->bid.fund << endl;

        currNode = currNode->next;
        ++count;
    }

    cout << "\n[[ " << count << " records displayed ]]\n";

}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {

    // search to verify if the bid exists
    Bid temp = Search(bidId);
    if (temp.bidId.empty()) {
        cout << "\nBid id " << bidId << " not found\n\n";
        return;
    }

    // if list empty, just return to caller as list is empty
    if (head == nullptr)
        return;

    // special case if the bidId matches the head
    if (bidId == head->bid.bidId) {

        // save the head temporarily
        Node* temp = head;

        // move the pointer to the next node, which will be the new head
        head = head->next;


        cout << "\n\n[[ Deleted Node: " << temp->bid.bidId << " ]]\n";


        // delete the original head and set it to null
        delete temp;
        temp = nullptr;

        // decrement size of list to account for deletion
        this->size -= 1;

        return;
    }

    // now set up the prev and curr temporary pointers
    // prev will be the node we keep after deletion of a node
    // curr will be the node we delete
    // we will walk the list incrementing both pointers while
    // searching for the bidId
    Node* prev = this->head;
    Node* curr = this->head->next;

    // special case if the bidId matches the tail
    if (bidId == tail->bid.bidId) {

        // traverse the list with both pointers
        // until we get to the end of the list with curr
        // prev will end up being the node before the tail,
        // and curr will be the tail
        while (curr->next != nullptr) {
            prev = prev->next;
            curr = curr->next;
        }

        // set prev->next to nullptr since this node is the new tail
        prev->next = nullptr;

        // set tail to prev
        tail = prev;

        cout << "\n\n[[ Deleted node: " << curr->bid.bidId << " ]]\n";

        // now delete original tail and set the pointer to null
        delete curr;
        curr = nullptr;

        // decrement size to account for deletion
        this->size -= 1;

        return;
    }

    // if bidID is not the head and not the tail,
    // then we traverse the list with prev and curr
    // until we find the node to be deleted
    while (curr->next != nullptr) {

        if (bidId == curr->bid.bidId) {

            // temporary pointer to curr,
            // because curr will be deleted
            Node* temp = curr;

            // set the node after curr to be the
            // new neighbor of prev
            prev->next = temp->next;

            cout << "\n\n[[ Deleting node: " << temp->bid.bidId
                << " ]]\n\n";

            // delete the node and set temp to null
            delete temp;
            temp = nullptr;

            // decrement size of list
            this->size -= 1;

            return;

        }
        else {

            // walk the list with both pointers
            // continue the loop until we get to 
            // the end of the list or we find the node
            prev = prev->next;
            curr = curr->next;
        }

    }

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {

    // Return empty bid if the list is empty
    if (head == nullptr) {
        return {};
    }

    // Set currNode to the list head
    Node* currNode = this->head;

    // Loop through the list. If we find the bidId,
    // then return the node.
    while (currNode != nullptr) {
        if (bidId == currNode->bid.bidId) {
            return currNode->bid;
        }

        currNode = currNode->next;
    }

    // Return an empty bid if the key is not found
    return {};
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << "[[ " << bid.bidId << ": " 
        << bid.title << " | " 
        << bid.amount << " | " 
        << bid.fund << " ]]\n\n";
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    getline(cin, bid.fund);
//    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.clear();
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}


/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {

    if (!list->Empty()) {
        cout << "\n\nFile has already been loaded\n\n";
        return;
    }

    cout << "\nLoading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

           // cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;


            //// this will delete each bid
            //// because of the special condition in search function
            //Bid temp = list->Search(bid.bidId);
            //if (temp.bidId != bid.bidId)
            //    list->Append(bid);


            // add this bid to the end
            list->Append(bid);

        }

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


// function to determine if a bid search is valide or not
bool isBidValid(LinkedList& list, Bid& bid) {
    
    Bid temp = list.Search(bid.bidId);

    if (!temp.bidId.empty())
        return false;

    return true;
}


/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */

using std::cout;

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
//        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        csvPath = "ebid_Monthly_Sales - Correct Columns.csv";
        bidKey = "97990"; // head node

    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "\n\nMenu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Prepend a Bid" << endl;
        cout << "  3. Load Bids" << endl;
        cout << "  4. Display All Bids" << endl;
        cout << "  5. Find Bid" << endl;
        cout << "  6. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;


        string input{};

        switch (choice) {

        // append a bid to end of list
        case 1:

            // call to create a new bid
            bid = getBid();

            if (isBidValid(bidList, bid)) {

                // set up timer
                ticks = clock();

                // call append function
                bidList.Append(bid);

                // determine number of ticks
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " milliseconds" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                cout << "\n";

                // display the new bid
                displayBid(bid);
            }
            else {
                cout << "\ncannot have duplicate bidId's\n";
                
            }

            break;

        // prepend new node to list
        case 2:

            // call to create the new bid
            bid = getBid();

            if (isBidValid(bidList, bid)) {

                // set up timer
                ticks = clock();

                // call prepend function
                bidList.Prepend(bid);


                // determine number of ticks
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " milliseconds" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                cout << "\n";

                // display the bid
                displayBid(bid);
            }
            else {
                cout << "\ncannot have duplicate bidId's\n";

            }
            break;

        // load the bids from file
        case 3:

            // set up timer
            ticks = clock();

            // call to load bids from file
            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            // determine number of ticks
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        // display the bids to output
        case 4:

            // set up timer
            ticks = clock();

            // call to print the list of bids
            bidList.PrintList();

            cout << bidList.Size() << " records read" << endl;

            // determine number of ticks
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        // search for a bid
        case 5:

            // enable customer to enter the bid id they want to search for
            cout << "\nEnter the bid id: ";
            cin.ignore();
            getline(cin, input);

            // set up timer
            ticks = clock();

            // test to see if the bid exists
            bid = bidList.Search(input);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            // if bid exists, then display it
            // else inform customer bid does not exist
            if (!bid.bidId.empty()) {
                cout << "\nBid found:\n";
                displayBid(bid);
            } else {
            	cout << "\nBid Id " << input << " not found." << endl;
            }

            // determine number of ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;


            // reset input variable
            input = "";

            break;

        // remove a node
        case 6:

            // enable customer to enter the bid they want to delete
            // based on bid id
            cout << "\nEnter the bid id: ";
            cin.ignore();
            getline(cin, input);

            // test to see if the bid exists
            bid = bidList.Search(input);

            // if bid exists, then display it
            // else inform customer bid does not exist
            if (!bid.bidId.empty()) {

                // set up timer
                ticks = clock();

                // call to remove function
                bidList.Remove(input);

                // determine number of ticks
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            }
            else {
                cout << "\nBid Id " << input << " not found." << endl;
            }

            // reset input variable
            input = {};

            break;

        default:
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
