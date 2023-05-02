#ifndef ORDEREDSUBSETLIST_H
#define ORDEREDSUBSETLIST_H
#include <bits/stdc++.h>
using namespace std;

// Define the node structure for the ordered subset list
struct Node {

    int data;
    Node* prev;
    Node* next;

};

// Define the ordered subset list class
class OrderedSubsetList {
    public:
        OrderedSubsetList(vector<int> v = vector<int>(), int limit = 0);

        // Add a node to the front of the list
        void pushFront(int data); 

        // Add a node to the back of the list
        void pushBack(int data);

        // Remove the node at the front of the list
        void popFront();

        // Remove the node at the back of the list
        void popBack();

        // Remove a specific node from the list
        void removeNode(int index);

        // Rollback the most recent deletion operation
        void rollback();

        // Print the list forwards
        void printForward();

        // Print the list backwards
        void printBackward();

        // Returns all the elements in the list
        vector<int> elements();

        int getSize();

    private:
        Node* head;
        Node* tail;
        stack<Node*> deletedNodes;
        vector<Node*> nodeAddress;
        vector<int> positions;
        vector<bool> alive;
        int size;

};

#endif