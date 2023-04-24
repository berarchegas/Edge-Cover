#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int INF = 1e9;
const int MAXN = 1e3 + 5;

// Define the node structure for the doubly linked list
struct Node {
    int data;
    Node* prev;
    Node* next;
};

// Define the doubly linked list class
class DoublyLinkedList {
    public:
        DoublyLinkedList() {
            head = nullptr;
            tail = nullptr;
        }

        void init(vector<int> &v) {
            for (int x : v) pushBack(x);
        }

        // Add a node to the front of the list
        void pushFront(int data) {
            Node* newNode = new Node;
            newNode->data = data;
            newNode->prev = nullptr;
            newNode->next = head;

            if (head != nullptr) {
                head->prev = newNode;
            } else {
                tail = newNode;
            }

            head = newNode;
        }

        // Add a node to the back of the list
        void pushBack(int data) {
            Node* newNode = new Node;
            newNode->data = data;
            newNode->prev = tail;
            newNode->next = nullptr;

            if (tail != nullptr) {
                tail->next = newNode;
            } else {
                head = newNode;
            }

            tail = newNode;
            nodeAddress.push_back(newNode);
        }

        // Remove the node at the front of the list
        void popFront() {
            if (head == nullptr) {
                return;
            }

            Node* temp = head;

            if (head == tail) {
                head = nullptr;
                tail = nullptr;
            } else {
                head = head->next;
                head->prev = nullptr;
            }

            deletedNodes.push(temp);
        }

        // Remove the node at the back of the list
        void popBack() {
            if (tail == nullptr) {
                return;
            }

            Node* temp = tail;

            if (head == tail) {
                head = nullptr;
                tail = nullptr;
            } else {
                tail = tail->prev;
                tail->next = nullptr;
            }

            deletedNodes.push(temp);
        }

        // Remove a specific node from the list
        void removeNode(int position) {
            Node* node = nodeAddress[position];
            
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

            deletedNodes.push(node);
        }

        // Rollback the most recent deletion operation
        void rollback() {
            if (deletedNodes.empty()) {
                return;
            }

            Node* node = deletedNodes.top();
            deletedNodes.pop();

            if (node->prev == nullptr) {
                pushFront(node->data);
            } else if (node->next == nullptr) {
                pushBack(node->data);
            } else {
                node->prev->next = node;
                node->next->prev = node;
            }
        }

        // Print the contents of the list
        void printList() {
            Node* current = head;

            while (current != nullptr) {
                cout << current->data << " ";
                current = current->next;
            }

            cout << endl;
        }

    private:
        Node* head;
        Node* tail;
        stack<Node*> deletedNodes;
        vector<Node*> nodeAddress;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    DoublyLinkedList list;

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(5);

    cout << "Original List: ";
    list.printList();

    list.popBack();
    list.popBack();
    list.popBack();
    list.popBack();

    cout << "List after PopBack: ";
    list.printList();

    list.rollback();
    list.rollback();
    list.rollback();

    cout << "List after RollBack: ";
    list.printList();

    return 0;

}