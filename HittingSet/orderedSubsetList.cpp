#include <bits/stdc++.h>
#include "orderedSubsetList.h"
using namespace std;

// Constructor for the OrderedSubsetList class
OrderedSubsetList::OrderedSubsetList(vector<int> v, int limit) {

    positions = vector<int> (limit, -1);
    alive = vector<bool> (limit, false);
    size = v.size();

    if (v.empty()) {
        head = nullptr;
        tail = nullptr;
        return;
    }

    head = new Node;
    head->data = v[0];
    head->prev = nullptr;
    head->next = nullptr;
    tail = head;
    positions[head->data] = nodeAddress.size();
    alive[head->data] = true;
    nodeAddress.push_back(head);
    
    for (int i = 1; i < v.size(); i++) {
        Node* newNode = new Node;
        newNode->data = v[i];
        newNode->prev = tail;
        newNode->next = nullptr;

        tail->next = newNode;
        tail = newNode;
        positions[newNode->data] = nodeAddress.size();
        alive[newNode->data] = true;
        nodeAddress.push_back(newNode);
    }

}

// Add a node to the front of the list
void OrderedSubsetList::pushFront(int data) {

    Node* newNode = new Node;
    newNode->data = data;
    newNode->prev = nullptr;
    newNode->next = head;

    if (head != nullptr) {
        head->prev = newNode;
    } 
    else {
        tail = newNode;
    }

    head = newNode;
    if (positions[data] == -1) {
        positions[data] = nodeAddress.size();
        nodeAddress.push_back(newNode);
    }
    alive[data] = true;
    size++;

}

// Add a node to the back of the list
void OrderedSubsetList::pushBack(int data) {

    Node* newNode = new Node;
    newNode->data = data;
    newNode->prev = tail;
    newNode->next = nullptr;

    if (tail != nullptr) {
        tail->next = newNode;
    } 
    else {
        head = newNode;
    }

    tail = newNode;
    if (positions[data] == -1) {
        positions[data] = nodeAddress.size();
        nodeAddress.push_back(newNode);
    }
    alive[data] = true;
    size++;

}

// Remove the node at the front of the list
void OrderedSubsetList::popFront() {

    if (head == nullptr) {
        return;
    }

    Node* temp = head;

    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } 
    else {
        head = head->next;
        head->prev = nullptr;
    }

    alive[temp->data] = false;
    deletedNodes.push(temp);
    size--;

}

// Remove the node at the back of the list
void OrderedSubsetList::popBack() {

    if (tail == nullptr) {
        return;
    }

    Node* temp = tail;

    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } 
    else {
        tail = tail->prev;
        tail->next = nullptr;
    }

    alive[temp->data] = false;
    deletedNodes.push(temp);
    size--;

}

// Remove a specific node from the list
void OrderedSubsetList::removeNode(int index) {

    Node* node = nodeAddress[positions[index]];
    
    if (node == nullptr) {
        return;
    }

    if (node == head) {
        popFront();
        return;
    }

    if (node == tail) {
        popBack();
        return;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;

    alive[node->data] = false;
    deletedNodes.push(node);
    size--;

}

// Rollback the most recent deletion operation
void OrderedSubsetList::rollback() {

    if (deletedNodes.empty()) {
        return;
    }

    Node* node = deletedNodes.top();
    deletedNodes.pop();

    if (node->prev == nullptr) {
        node->next = head;

        if (head != nullptr) {
            head->prev = node;
        } 
        else {
            tail = node;
        }

        head = node;
    } 
    else if (node->next == nullptr) {
        node->prev = tail;

        if (tail != nullptr) {
            tail->next = node;
        } 
        else {
            head = node;
        }

        tail = node;
    } 
    else {
        node->prev->next = node;
        node->next->prev = node;
    }
    alive[node->data] = true;
    size++;

}

// Print the list forwards
void OrderedSubsetList::printForward() {

    Node* current = head;

    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }

    cout << endl;

}

// Print the list backwards
void OrderedSubsetList::printBackward() {

    Node* current = tail;

    while (current != nullptr) {
        cout << current->data << " ";
        current = current->prev;
    }

    cout << endl;

}

// Returns all the elements in the list
vector<int> OrderedSubsetList::elements() {

    vector<int> ans;
    Node* current = head;

    while (current != nullptr) {
        ans.push_back(current->data);
        current = current->next;
    }

    return ans;
}

// Returns a booleanif id is alive
bool OrderedSubsetList::getState(int pos) {
    return alive[pos];
}

int OrderedSubsetList::getSize() { 
    return size; 
}