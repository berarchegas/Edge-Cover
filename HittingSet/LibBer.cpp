#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

// Maybe i am removing a node more than once and then rollback doesnt work properly
// I am creating a lot of auxiliar Nodes* and maybe I can just do a global one?

// Define the node structure for the ordered subset list
struct Node {

    int data;
    Node* prev;
    Node* next;

};

// Define the ordered subset list class
class OrderedSubsetList {
    public:
        OrderedSubsetList(vector<int> v = vector<int>(), int size = 0) {

            positions = vector<int> (size, -1);

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
            nodeAddress.push_back(head);
            
            for (int i = 1; i < v.size(); i++) {
                Node* newNode = new Node;
                newNode->data = v[i];
                newNode->prev = tail;
                newNode->next = nullptr;

                tail->next = newNode;
                tail = newNode;
                positions[newNode->data] = nodeAddress.size();
                nodeAddress.push_back(newNode);
            }

        }

        // Add a node to the front of the list
        void pushFront(int data) {

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

        }

        // Add a node to the back of the list
        void pushBack(int data) {

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
            } 
            else {
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
            } 
            else {
                tail = tail->prev;
                tail->next = nullptr;
            }

            deletedNodes.push(temp);

        }

        // Remove a specific node from the list
        void removeNode(int index) {

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

        }

        // Print the list forwards
        void printForward() {

            Node* current = head;

            while (current != nullptr) {
                cout << current->data << " ";
                current = current->next;
            }

            cout << endl;

        }

        // Print the list backwards
        void printBackward() {

            Node* current = tail;

            while (current != nullptr) {
                cout << current->data << " ";
                current = current->prev;
            }

            cout << endl;

        }

        // Returns all the elements in the list
        vector<int> elements() {

            vector<int> ans;
            Node* current = head;

            while (current != nullptr) {
                ans.push_back(current->data);
                current = current->next;
            }

            return ans;
        }

    private:
        Node* head;
        Node* tail;
        stack<Node*> deletedNodes;
        vector<Node*> nodeAddress;
        vector<int> positions;

};

int nodeDegree[10], n, m;
vector<vector<int>> bucket;

// edges[i] are the vertices of the i-th edge
// vertices[i] are the edges of the i-th vertex
OrderedSubsetList edges[10], vertices[10], validVertices;
stack<pii> operations;

void printTudo() {
    for (int i = 0; i < n; i++) {
        cout << "Node " << i << " = ";
        vertices[i].printForward();
    }
    cout << '\n';
    for (int i = 0; i < m; i++) {
        cout << "Edge " << i << " = ";
        edges[i].printForward();
    }
    cout << '\n';
    cout << endl;
}

// Erase the edge from its vertices
void eraseEdge(int edge) {
    
    vector<int> list = edges[edge].elements();
    for (int x : list) {
        vertices[x].removeNode(edge);
        nodeDegree[x]--;
    }

    operations.push({0, edge});

}

// Place the vertex in the answer set
void takeVertex(int node) {
    
    // Erase it from every edge that contains it
    vector<int> list = vertices[node].elements();
    for (int x : list) {
        edges[x].removeNode(node);
    }

    validVertices.removeNode(node);
    operations.push({1, node});

    // Erase every edge
    for (int x : list) {
        eraseEdge(x);
    }

}

// Fix the node out of the answer
void ignoreVertex(int node) {

    // Erase it from every edge that contains it
    vector<int> list = vertices[node].elements();
    for (int x : list) {
        edges[x].removeNode(node);
    }

    validVertices.removeNode(node);
    operations.push({1, node});

}

// Undo the last deletion of vertex/edge
void undo() {

    if (operations.empty()) {
        return;
    }

    pii last = operations.top();
    operations.pop();

    int op = last.first, id = last.second;

    if (!op) {

        // undo an eraseEdge operation
        vector<int> list = edges[id].elements();
        for (int x : list) {
            vertices[x].rollback();
            nodeDegree[x]++;
        }
    }
    else if (op == 1) {

        // undo an eraseVertex operation
        vector<int> list = vertices[id].elements();
        for (int x : list) {
            edges[x].rollback();
        }

        validVertices.rollback();
    }

}

// Calculate the Upperbound
int calculateUpperbound() {
    int ans = 0;
    int initialSize = operations.size();
    
    for (int i = m; i > 0; i--) {
        while (!bucket[i].empty()) {
            int id = bucket[i].back();
            bucket[i].pop_back();
            if (nodeDegree[id] == i) {
                ans++;
                takeVertex(id);
            }
            else {

                bucket[nodeDegree[id]].push_back(id);
            }
        }
    }


    while (operations.size() > initialSize) {
        undo();
    }    

    bucket[0].clear();
    vector<int> valid = validVertices.elements();
    for (int x : valid) {
        bucket[nodeDegree[x]].push_back(x);
    }

    return ans;
}

int main() {
    cin >> n >> m;

    validVertices = OrderedSubsetList(vector<int>(), n);
    for (int i = 0; i < n; i++) {
        validVertices.pushBack(i);
        vertices[i] = OrderedSubsetList(vector<int>(), m);
    }
    for (int i = 0; i < m; i++) {
        edges[i] = OrderedSubsetList(vector<int>(), n);
    }

    for (int i = 0; i < m; i++) {
        int tam, x;
        cin >> tam;
        for (int j = 0; j < tam; j++) {
            cin >> x;
            nodeDegree[x]++;
            vertices[x].pushBack(i);
            edges[i].pushBack(x);
        }
    }

    bucket = vector<vector<int>> (m + 1);
    for (int i = 0; i < n; i++) {
        bucket[nodeDegree[i]].push_back(i);
    }

    printTudo();

    cout << calculateUpperbound() << '\n';

    ignoreVertex(4);

    printTudo();   

    cout << calculateUpperbound() << '\n';

    printTudo(); 

    return 0;
}