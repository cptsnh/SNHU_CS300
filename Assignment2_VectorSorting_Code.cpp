//============================================================================
// Name        : VectorSorting.cpp
// Author      : Chris Trimmer (with base code provided by SNHU)
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Vector Sorting Algorithms
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
// Static methods used for testing
//============================================================================

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
 * Prompt user for bid information using console (std::in)
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
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // this line is used for testing. it outputs the bid object to output as it is created
            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end of the vector
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;

    // pick the middle element as pivot point
    int mid = low + (high - low) / 2;

    // set the pivot object to be the object at the middle index of vector
    Bid pivot = bids.at(mid);

    // set control variable for the while loop
    bool done = false;
    while (!done) {

        // keep incrementing low index while bids[low] < bids[pivot]
        while (bids.at(low).title < pivot.title) {
            low += 1;
        }

        // keep decrementing high index while bids[pivot] < bids[high]
        while (pivot.title < bids.at(high).title) {
            high -= 1;
        }

        // end the while loop now that we have the partition
        if (low >= high) {
            done = true;
        }
        else {

            // swap the low and high bids, and move low and high counters closer
            // in preparation for next iteration
            std::swap(bids.at(low), bids.at(high));

            low += 1;
            high -= 1;
        }

    }

    // all bids are partitioned, so return high
    return high;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {

    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (bids.size() <= 1)
        return;
    
    if (begin >= end) {
     //   cout << "\nThere is nothing to sort.\n";
        return;
    }

    /* Partition bids into low and high such that
     midpoint is location of last element in low */
    int partitionIndex = partition(bids, begin, end);

    // recursively sort low partition (begin to mid)
    quickSort(bids, begin, partitionIndex);

    // recursively sort high partition (mid+1 to end)
    quickSort(bids, partitionIndex + 1, end);


}

// FIXME (1a): Implement the selection sort logic over bid.title
/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {

    //define min as int (index of the current minimum bid)
    int min;

    // check size of bids vector
    // set size_t platform-neutral result equal to bid.size()
    size_t result = bids.size();

    // if there are no elements, then inform the user to load data
    if (result <= 0) {
        cout << "There is no data to sort\n";
        cout << "Please load the data\n";
        return;
    }

    // This is the outer for loop. It's set to loop the number of times
    // equal to the amount of elements in the vector.
    for (auto pos = 0; pos < result; ++pos) {
        
        // before starting inner loop, set min to position that
        // will demarcate where we have sorted the list
        min = pos;

        // This is the inner loop. It controls the comparison
        // of the current position value with the value at the
        // index that has the current minimum value
        for (auto j = pos + 1; j < result; ++j) {

            // if titles are the same, then set min based on lower bidId
            if (bids.at(j).title == bids.at(min).title) {
                if (bids.at(j).bidId < bids.at(min).bidId) {
                    min = j;

                }

                continue;
            }

            // set min to a new index if it finds a lower value for title
            if (bids.at(j).title < bids.at(min).title)
                min = j;

            
        }

        // swap the current position and index that has current 
        // minium value
        if (min != pos)
            std::swap(bids.at(pos), bids.at(min));

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
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        // these are files used for testing.
        // Note that the eBid_Monthly_Sales.csv file has the columns in the wrong order
        // So I created a .csv file with the columns in the correct order
        //csvPath = "ebid_Monthly_Sales.csv";
       // csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        csvPath = "eBid_Monthly_Sales - Correct Columns.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    // Main Menu
    int choice = 0;
    while (choice != 9) {
        cout << "\n\nMenu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        // Menu selection to load the bids data from .csv file
        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        // Menu selection to diplay the bids vector
        case 2:

            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;
        
        // Menu selection to sort the bids vector using selection sort
        case 3:

            // Initialize a timer variable before calling selection sort
            ticks = clock();

            // call to selection sort passing the bids vector
            selectionSort(bids);
            
            // Calculate elapsed time and display result
            ticks = clock() - ticks;
            cout << "\n\n" << bids.size() << " bids sorted.\n\n";
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        // Menu selection to sort the bids vector using quick sort
        case 4:

            // Initialize a timer variable before calling quick sort
            ticks = clock();

            // call to quick sort passing the bids vector
            quickSort(bids, 0, bids.size()-1);

            // calculate elapsed time and display the result
            ticks = clock() - ticks;
            cout << "\n\n" << bids.size() << " bids sorted.\n\n";
            cout << "bids size: " << bids.size() << "\n";
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        default:
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
