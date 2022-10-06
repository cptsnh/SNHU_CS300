//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Chris Trimmer with base code from SNHU
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

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

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
            Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */

using std::cout;

class BinarySearchTree {

private:
    Node* root;

    // function to add a node
    void addNode(Node* node, Bid bid);

    // traversal functions
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);

    // helper function to recursively remove a node
    bool removeNodeRecur(Node* tree, Node* parent, Node* node);

    // helper function to recursively delete the tree
    void ClearTree(Node* tree);

    // helper function to get parent node
    Node* GetParentHelper(Node* tree, Node* node);

    // helper function for searching
    Node* SearchHelperA(Node* node, string bidId);

    // function to get size of tree
    size_t sizeHelper(Node* node);



public:

    // constructor
    BinarySearchTree();

    // destructor
    virtual ~BinarySearchTree();

    // traversal definitions
    void InOrder();
    void PostOrder();
    void PreOrder();

    // insert a bid
    void Insert(Bid bid);

    // search for a bid
    Node* SearchA(string bidId);

    // Helper function to get a parent node
    Node* GetParent(Node* node);

    // node removal function
    void removeRecur(string bid);

    // print a bid
    void PrintBid(Bid bid);

    // function to get the size of the tree
    size_t GetSize();

};


/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    // set root to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {

    // recurse from root deleting every node
    // call the clear tree helper function
    ClearTree(root);

}

/// <summary>
/// Helper function to delete every node in the tree
/// primarily  used by the destructor
/// </summary>
/// <param name="node"></param>
void BinarySearchTree::ClearTree(Node* node) {
    
    // if the root is null, then the tree is already empty
    if (node == nullptr)
        return;

    // recursively call ClearTree on left and right side to delete each node
    ClearTree(node->left);
    ClearTree(node->right);
    delete node;
}


// function to get size of tree (calls the sizehelper function)
size_t BinarySearchTree::GetSize() {
    return sizeHelper(root);
}

// recursive function to get the size of the tree
size_t BinarySearchTree::sizeHelper(Node* node) {

    // if the root is null, then the tree size is 0
    if (node == nullptr) {
        return 0;
    }

    // recursively call helper function on the left and right side
    return 1 + sizeHelper(node->left) + sizeHelper(node->right);
}

/// <summary>
/// Function used to get the parent of a node
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Node* BinarySearchTree::GetParent(Node* node) {

    return GetParentHelper(root, node);
}

/// <summary>
/// Helper function for getting parent node of a tree or subtree
/// </summary>
/// <param name="tree"></param>
/// <param name="node"></param>
/// <returns></returns>
Node* BinarySearchTree::GetParentHelper(Node* tree, Node* node) {

    // return null if the tree is empty
    if (tree == nullptr) {
        return nullptr;
    }

    // if the node is the left or right side of parent, then return the parent
    if (tree->left == node || tree->right == node) {
        return tree;
    }

    // if the bidId is less than parent node, then
    // recursively call the left side until we get the parent
    if (node->bid.bidId < tree->bid.bidId) {
        return GetParentHelper(tree->left, node);
    }

    // otherwise, recursively call the right side until we get the parent
    return GetParentHelper(tree->right, node);

}


/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {

    // if the root is null, then this bid will be the root of the tree
    if (root == nullptr) {
        Node* newNode = new Node(bid);
        root = newNode;
    }

    // otherwise, call addNode helper function with the new bid
    else
        addNode(root, bid);
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {

    // if the node bidid is greater than the incoming bidId
    if (node->bid.bidId > bid.bidId) {

        // if the left node is empty, then the new node is left child
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }

        // otherwise, recursively call addNode on left side
        else addNode(node->left, bid);
    }
    else {

        // if the right node is empty, then the new node is right child
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }

        // otherwise, recursively call addNode on right side
        else {
            addNode(node->right, bid);
        }
    }

}


/// <summary>
/// Public function to remove a node
/// It calls helper function to perform recursion to locate and remove the node
/// </summary>
/// <param name="bidId"></param>
void BinarySearchTree::removeRecur(string bidId) {

    // create pointer to the node we want to delete using search function
    Node* node = SearchA(bidId);

    // create pointer to the parent of the node we want to delete
    Node* parent = GetParent(node);

    // call remove helper function
    removeNodeRecur(root, parent, node);
}


/// <summary>
/// Private function that is the remove helper function
/// </summary>
/// <param name="tree"></param>
/// <param name="parent"></param>
/// <param name="node"></param>
/// <returns></returns>
bool BinarySearchTree::removeNodeRecur(Node* tree, Node* parent, Node* node) {
    
    // if the root or tree node is null, then there is nothing to delete
    if (node == nullptr)
        return false;

    // if the node has two children
    if (node->left != nullptr && node->right != nullptr) {

        // set up a pointer to the successor node and parent node
        Node* succNode = node->right;
        Node* succParent = node;

        // walk the left successor side until the end
        while (succNode->left != nullptr) {
            succParent = succNode;
            succNode = succNode->left;
        }

        // set the bid to be the successor bid
        node->bid = succNode->bid;

        // now remove the node
        removeNodeRecur(tree, succParent, succNode);
    }

    // if the node is the root
    else if (node == root) {

        // if there is left subnode, then set it to root
        if (node->left != nullptr) {
            root = node->left;
        }

        // otherwise, the right node is the new root
        else
            root = node->right;
    }

    // if only a left child
    else if (node->left != nullptr) {

        // set the parent to be left or right based on value of node
        if (parent->left == node) {
            parent->left = node->left;
        }
        else
            parent->right = node->left;
    }

    // if the only a right child or a leaf
    // then set the left or right parent node to be value of node
    else {
        if (parent->left == node) {
            parent->left = node->right;
        }
        else {
            parent->right = node->right;
        }
    }

    return true;
}

/// <summary>
/// Function to print a bid within the Tree class
/// </summary>
/// <param name="bid"></param>
void BinarySearchTree::PrintBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << ", " << bid.fund << ", " << bid.amount << "\n";
}


/// <summary>
/// Public accessible search function
/// It calls the private search helper function to perform
/// the recursive search
/// </summary>
/// <param name="bidId"></param>
/// <returns></returns>
Node* BinarySearchTree::SearchA(string bidId) {

    // call to the recursive helper function
    return SearchHelperA(root, bidId);

}

/// <summary>
/// Private search helper function
/// It recursively searches the tree to find the node
/// </summary>
/// <param name="node"></param>
/// <param name="bidId"></param>
/// <returns></returns>
Node* BinarySearchTree::SearchHelperA(Node* node, string bidId) {


    // if the node is null or the incoming node is the bidId we are looking for,
    // then just return the node
    if (node == nullptr || node->bid.bidId == bidId) {
        return node;
    }

    // search left side of tree if the bidId is greater than incoming bidId
    if (node->bid.bidId > bidId) {

            return SearchHelperA(node->left, bidId);
    }

    // search right side if node bidId is less than the incoming bidId
    return SearchHelperA(node->right, bidId);

}


/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {

    // call the inOrder helper function and pass in root
    inOrder(root);

}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {

    // call the postOrder helper function and pass in root
    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {

    // call the preOrder helper function and pass in root
    preOrder(root);
}


/// <summary>
/// Inorder traversal helper
/// Use recursion to print the bids
/// </summary>
/// <param name="node"></param>
void BinarySearchTree::inOrder(Node* node) {

    // if node is null, then return as there is nothing to print
    if (node == nullptr)
        return;

    // recursively call inOrder using left side
    inOrder(node->left);

    // print the bid
    cout << node->bid.bidId << ": " << node->bid.title << ", "
        << node->bid.fund << ", " << node->bid.amount << "\n";

    // recursively call inOrder using right side
    inOrder(node->right);

}

/// <summary>
/// Postorder traversal helper
/// Uses recursion to print the bids
/// </summary>
/// <param name="node"></param>
void BinarySearchTree::postOrder(Node* node) {

    // if node is null, then return as there is nothing to print
    if (node == nullptr)
        return;

    // recursively call postOrder using left and right side
    postOrder(node->left);
    postOrder(node->right);

    // print the bid
    cout << node->bid.bidId << ": " << node->bid.title << ", " 
        << node->bid.fund << ", " << node->bid.amount << "\n";

}

/// <summary>
/// PreOrder traversal helper
/// Uses recursion to print the bids
/// </summary>
/// <param name="node"></param>
void BinarySearchTree::preOrder(Node* node) {

    // if node is null, then return as there is nothing to print
    if (node == nullptr)
        return;

    // print the bid
    cout << node->bid.bidId << ": " << node->bid.title << ", "
        << node->bid.fund << ", " << node->bid.amount << "\n";

    // recursively call preOrder using left and right side
    preOrder(node->left);
    preOrder(node->right);

}

//============================================================================
// Static methods used for testing
//============================================================================



/// <summary>
/// Function used to get user input for adding a bid
/// </summary>
/// <returns></returns>
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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            cout << "Bid: " << bid.bidId << ", Title: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }

        cout << "\n" << bst->GetSize() << " bids loaded.\n";

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
//        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        csvPath = "eBid_Monthly_Sales - Correct Columns.csv";
        bidKey = "98385";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();

    Bid bid;
    Bid searchBid;

    Node* temp = nullptr;
    string input{};

    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display InOrder" << endl;
        cout << "  3. Display PreOrder" << endl;
        cout << "  4. Display PostOrder" << endl;
        cout << "  5. Find Bid" << endl;
        cout << "  6. Add Bid" << endl;
        cout << "  7. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {

        // Load bids
        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "\n";
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        // Display bids using inorder traversal
        case 2:

            // Initialize a timer variable before loading bids
            ticks = clock();

            // call inOrder traversal
            bst->InOrder();

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "\n";
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            cout << "\nBid count: " << bst->GetSize() << "\n\n";
            break;

        // Display bids using preorder traversal
        case 3:

            // Initialize a timer variable before loading bids
            ticks = clock();

            // call preorder traversal
            bst->PreOrder();

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "\n";
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            cout << "\nBid count: " << bst->GetSize() << "\n\n";
            break;

        // Display bids using postorder traversal
        case 4:

            // Initialize a timer variable before loading bids
            ticks = clock();

            // call postorder traversal
            bst->PostOrder();

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "\n";
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            cout << "\nBid count: " << bst->GetSize() << "\n\n";
            break;

        // Search bids
        case 5:

            cout << "\n\nEnter bidId: ";

            cin.ignore();
            getline(cin, input);

            // create a temp bid based on bidId
            temp = bst->SearchA(input);

            // if the bid is not found, return output
            if (temp == nullptr)
                cout << input << " was not found\n";
            else {

                cout << "\n";

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Call to display bid
                displayBid(temp->bid);
                cout << "\n";

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            }

            break;

        // Add a bid
        case 6:

            // get input from user to fill in data for the new bid
            bid = getBid();

            // create a temp bid to search for the bidId
            temp = bst->SearchA(bid.bidId);

            // if the bidid already exists, then inform user and prevent adding the node
            if (temp != nullptr) {
                cout << "\n";
                cout << temp->bid.bidId << " already exists.\n";
            }
            else {

                // Initialize a timer variable before loading bids
                ticks = clock();

                // call insert function to add the bid
                bst->Insert(bid);
                
                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "\n";
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                // output to user to confirm the bid has been added
                cout << bid.bidId << " has been inserted into the tree.\n";
                displayBid(bid);
                cout << "\n";
            }

            break;

        // Remove a bid
        case 7:

            // get user input for the bidid they want to remove
            cout << "\n\nEnter bidId: ";
            cin.ignore();
            getline(cin, input);

            // create temp bid based on bidid the user wants to remove
            temp = bst->SearchA(input);

            // if the result is null, then inform user the bidid was not found
            // and end the branch logic
            if (temp == nullptr) {
                cout << input << " was not found.\n";
            }
            else {

                // Initialize a timer variable before loading bids
                ticks = clock();

                // call remove function
                bst->removeRecur(input);

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "\n";
                cout << "Bid has been removed.\n";
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            }

            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
