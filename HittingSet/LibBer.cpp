#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

// I am creating a lot of auxiliar Nodes* and maybe I can just do a global one?
// I keep passing vector through the elements function, but this is not optimized
// Pass vectors by reference

// Design Decision: We are maintaining a few global structures to avoid memory redundancy
// We need to be able to reset these structures properly before using each one
// We could also test always going through every vertex and edge instead of getting vectors with active elements

// Define the node structure for the ordered subset list
struct Node {

    int data;
    Node* prev;
    Node* next;

};

// Define the ordered subset list class
class OrderedSubsetList {
    public:
        OrderedSubsetList(vector<int> v = vector<int>(), int limit = 0) {

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
            alive[data] = true;
            size++;

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
            alive[data] = true;
            size++;

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

            alive[temp->data] = false;
            deletedNodes.push(temp);
            size--;

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

            alive[temp->data] = false;
            deletedNodes.push(temp);
            size--;

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

            alive[node->data] = false;
            deletedNodes.push(node);
            size--;

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
            alive[node->data] = true;
            size++;

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

        int getSize() { 
            return size; 
        }

    private:
        Node* head;
        Node* tail;
        stack<Node*> deletedNodes;
        vector<Node*> nodeAddress;
        vector<int> positions;
        vector<bool> alive;
        int size;

};


// Defines the nodes of the Set Trie class
class TrieNode {
    public:
        unordered_map<int, TrieNode*> children;
        bool is_end_of_word;
        int qtd;

        TrieNode() {
            is_end_of_word = false;
            qtd = 0;
        }
};

// Defines the SetTrie class
class SetTrie {
    private:
        TrieNode* root;

    public:
        SetTrie() {
            root = new TrieNode();
        }

        void insert(vector<bool> &word) {
            TrieNode* current = root;

            for (int i = 0; i < word.size(); i++) {
                if (!word[i]) continue;

                if (current->children.find(i) == current->children.end()) {
                    current->children[i] = new TrieNode();
                }

                current->qtd++;
                current = current->children[i];
            }

            current->qtd++;
            current->is_end_of_word = true;
        }

        // Returns if there is a subset of word in the trie
        bool subsetSearch(vector<bool> &word) {

            queue<TrieNode*> fila;
            fila.push(root);

            while (!fila.empty()) {
                TrieNode* at = fila.front();
                fila.pop();

                if (!at->qtd) continue;
                if (at->is_end_of_word) return true;

                for (auto x : at->children) {
                    if (word[x.first]) {
                        fila.push(x.second);
                    }
                }
            }

            return false;
        }

        // Returns if there is a superset of word in the trie
        bool supersetSearch(vector<bool> &word) {

            queue<pair<TrieNode*, int>> fila;
            fila.push({root, 0});

            while (!fila.empty()) {
                pair<TrieNode*, int> aux = fila.front();
                fila.pop();

                TrieNode* at = aux.first;
                int index = aux.second;

                if (!at->qtd) continue;
                
                while (index < word.size() && !word[index]) index++;
                if (index == word.size()) return true;

                for (auto x : at->children) {
                    if (x.first < index) {
                        fila.push({x.second, index});
                    }
                    else if (x.first == index) {
                        fila.push({x.second, index + 1});
                    }
                }
            }

            return false;

        }

};

bool valid[10];

// We need to reset maxLowerBound properly 
int n, m, maxDegree[10][2], maxDegreeNode[10][2], maxLowerBound[10];
vector<vector<int>> bucket;

// blockedEdges by each vertex in the costly discard packing bound
vector<int> blockedEdges[10];

// edges[i] are the vertices of the i-th edge
// vertices[i] are the edges of the i-th vertex
OrderedSubsetList edges[10], vertices[10], validVertices, validEdges;
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
    }

    validEdges.removeNode(edge);
    operations.push({0, edge});

}

// Place the vertex in the answer set
void takeVertex(int node) {

    // Erase it from every edge that contains it
    vector<int> list = vertices[node].elements();
    for (int x : list) {
        edges[x].removeNode(node);
    }

    valid[node] = false;
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

    valid[node] = false;
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
        }

        validEdges.rollback();
    }
    else if (op == 1) {

        // undo an eraseVertex operation
        vector<int> list = vertices[id].elements();
        for (int x : list) {
            edges[x].rollback();
        }

        valid[id] = true;
        validVertices.rollback();
        bucket[vertices[id].getSize()].push_back(id);
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
            if (vertices[id].getSize() == i && valid[id]) {
                ans++;
                takeVertex(id);
            }
            else if (valid[id]) {
                bucket[vertices[id].getSize()].push_back(id);
            }
        }
    }


    while (operations.size() > initialSize) {
        undo();
    }    

    bucket[0].clear();
    vector<int> valid = validVertices.elements();
    for (int x : valid) {
        bucket[vertices[x].getSize()].push_back(x);
    }

    return ans;
}

// return ceil of a / b
int ceiling(int a, int b) {
    return a / b + !!(a % b);
}

int maxDegreeBound() {
    int maxDegree = 0;
    vector<int> valid = validVertices.elements();
    for (int x : valid) {
        maxDegree = max(maxDegree, vertices[x].getSize());
    }

    return ceiling(validEdges.getSize(), maxDegree);
}

// returns the lowerBound and calculates for every vertex the new lowerbound if we remove it
int efficiencyBound() {

    double ans = 0;
    vector<int> valid = validEdges.elements();

    for (int x : valid) {

        vector<int> nodes = edges[x].elements();
        maxDegree[x][0] = maxDegree[x][1] = 0;
        maxDegreeNode[x][0] = maxDegreeNode[x][1] = -1;
        
        for (int y : nodes) {
        
            if (vertices[y].getSize() > maxDegree[x][0]) {
                maxDegree[x][1] = maxDegree[x][0];
                maxDegreeNode[x][1] = maxDegreeNode[x][0];
                maxDegree[x][0] = vertices[y].getSize();
                maxDegreeNode[x][0] = y;
            }
            else if (vertices[y].getSize() > maxDegree[x][1]) {
                maxDegree[x][1] = vertices[y].getSize();
                maxDegreeNode[x][1] = y;
            }
        
        }
        
        ans += 1.0 / maxDegree[x][0];
    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) {
    
        double bonus = 0;
        vector<int> edges = vertices[x].elements();
    
        for (int y : edges) {
            if (maxDegreeNode[y][0] == x) {
                bonus += 1.0 / maxDegree[y][1] - 1.0 / maxDegree[y][0];
            }
        }

        maxLowerBound[x] = max(maxLowerBound[x], (int)ceil(ans + bonus));
        
    }

    // dont know if this breaks
    return (int)ceil(ans);
}

// Heuristic sort for good packing
// I just did a simple sort, we are suppose to change this function to something good
bool customSort(int a, int b) {
    return edges[a].getSize() > edges[b].getSize();
}

int packingBound() {
    vector<int> e = validEdges.elements(), packing;
    sort(e.begin(), e.end(), customSort);

    for (int x : e) {
        bool ok = true;
        vector<int> nodes = edges[x].elements();
        
        for (int y : nodes) {
            ok &= valid[y];
        }

        if (ok) {
            packing.push_back(x);
            for (int y : nodes) valid[y] = false;
        }
    
    }

    for (int x : packing) {
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            valid[y] = true;
        } 

    }

    return packing.size();
}

int repacking() {

    vector<int> nodes = validVertices.elements();

    // Find the 3 greatest degrees
    vector<pii> grau = {{0, -1}, {0, -1}, {0, -1}};
    for (int x : nodes) {
        if (vertices[x].getSize() > grau[0].first) {
            grau[2] = grau[1];
            grau[1] = grau[0];
            grau[0] = {vertices[x].getSize(), x};
        }
        else if (vertices[x].getSize() > grau[1].first) {
            grau[2] = grau[1];
            grau[1] = {vertices[x].getSize(), x};
        }
        else if (vertices[x].getSize() > grau[2].first) {
            grau[2] = {vertices[x].getSize(), x};
        }
    }

    // Repack these 3
    for (int i = 0; i < 3; i++) {
    
        if (grau[i].second != -1) {
            int initialSize = operations.size();
            ignoreVertex(grau[i].second);
            maxLowerBound[grau[i].second] = max(maxLowerBound[grau[i].second], packingBound());
            while (operations.size() > initialSize) undo();
        }
    
    }

}

void costlyDiscardPackingBound() {
    vector<int> e = validEdges.elements(), packing;
    sort(e.begin(), e.end(), customSort);

    for (int x : e) {
        bool ok = true;
        vector<int> nodes = edges[x].elements();
        
        for (int y : nodes) {
            ok &= valid[y];
        }

        if (ok) {
            packing.push_back(x);
            for (int y : nodes) valid[y] = false;
        }
    
    }

    for (int x : e) {
        int cnt = 0, block = -1;
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            if (!valid[y]) {
                cnt++;
                block = y;
            }
        }

        // if the edge only has one vertex 
        // then if we remove this vertex the edge becomes insignificant
        if (cnt == 1 && nodes.size() > 1) {
            blockedEdges[block].push_back(x);
        } 

    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) {
        vector<int> extraPacking;
        sort(blockedEdges[x].begin(), blockedEdges[x].end(), customSort);
        
        for (int y : blockedEdges[x]) {
            bool ok = true;
            vector<int> verts = edges[y].elements();
            
            for (int z : verts) {
                if (z == x) continue;
                ok &= valid[z];
            }

            if (ok) {
                extraPacking.push_back(y);
                for (int z : verts) {
                    valid[z] = false;
                }
            }
        }

        for (int y : extraPacking) {
            vector<int> verts = edges[y].elements();

            for (int z : verts) {
                valid[z] = true;
            } 

        }

        maxLowerBound[x] = max(maxLowerBound[x], (int)(packing.size() + extraPacking.size()));

    }

    for (int x : packing) {
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            valid[y] = true;
        } 

    }

}

int sumOverPackingBound(vector<int> &packing) {
    vector<int> e = validEdges.elements(), packing;
    sort(e.begin(), e.end(), customSort);

    for (int x : e) {
        bool ok = true;
        vector<int> nodes = edges[x].elements();
        
        for (int y : nodes) {
            ok &= valid[y];
        }

        if (ok) {
            packing.push_back(x);
            for (int y : nodes) valid[y] = false;
        }
    
    }

    int goal = validEdges.getSize(), sum = 0, ans = 0;
    for (int x : packing) {
        vector<int> nodes = edges[x].elements();
        int maxDegree = 0;

        for (int y : nodes) {
            maxDegree = max(maxDegree, vertices[y].getSize() - 1);
        } 

        sum += maxDegree;
        ans++;

    }

    if (sum < goal) {
        vector<int> nodes = validVertices.elements();
        vector<int> nodeChoices;

        for (int x : nodes) {
            if (valid[x]) {
                nodeChoices.push_back(x);
            }
        }

        sort(nodeChoices.begin(), nodeChoices.end(), [&] (int a, int b) {
            return vertices[a].getSize() > vertices[b].getSize();
        });

        for (int x : nodeChoices) {
            if (sum >= goal) break;
            sum += vertices[x].getSize();
            ans++;
        }

    }

    for (int x : packing) {
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            valid[y] = true;
        } 

    }

    return ans;
}

// Returns edges with size <= 1
// If it has size 1, we pick the corresponding vertex
// If it has size 0, we delete it (Is it possible to have size 0? )
vector<int> unitEdgeRule() {
    vector<int> ans;

    vector<int> e = validEdges.elements();
    for (int x : e) {
        if (edges[x].getSize() <= 1) ans.push_back(x);
    }

    return ans;
}

vector<int> edgeDomination() {

}

int main() {
    cin >> n >> m;

    validVertices = OrderedSubsetList(vector<int>(), n);
    for (int i = 0; i < n; i++) {
        valid[i] = true;
        validVertices.pushBack(i);
        vertices[i] = OrderedSubsetList(vector<int>(), m);
    }
    validEdges = OrderedSubsetList(vector<int>(), m);
    for (int i = 0; i < m; i++) {
        validEdges.pushBack(i);
        edges[i] = OrderedSubsetList(vector<int>(), n);
    }

    for (int i = 0; i < m; i++) {
        int tam, x;
        cin >> tam;
        for (int j = 0; j < tam; j++) {
            cin >> x;
            vertices[x].pushBack(i);
            edges[i].pushBack(x);
        }
    }

    bucket = vector<vector<int>> (m + 1);
    for (int i = 0; i < n; i++) {
        bucket[vertices[i].getSize()].push_back(i);
    }

    printTudo();

    cout << calculateUpperbound() << '\n';

    eraseEdge(3);

    ignoreVertex(0);

    calculateUpperbound();

    takeVertex(2);

    eraseEdge(2);

    calculateUpperbound();

    eraseEdge(1);

    ignoreVertex(1);

    calculateUpperbound();

    takeVertex(4);

    while (!operations.empty()) undo();

    cout << calculateUpperbound() << '\n';

    printTudo(); 

    return 0;
}