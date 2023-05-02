#include <bits/stdc++.h>
#include "operations.h"
#include "orderedSubsetList.h"
using namespace std;
using pii = pair<int, int>;

void printAll(int n, int m, OrderedSubsetList *vertices, OrderedSubsetList *edges) {
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
void eraseEdge(int edge, OrderedSubsetList *vertices, OrderedSubsetList *edges, 
    OrderedSubsetList &validEdges, stack<pii> &operations) {

    vector<int> list = edges[edge].elements();
    for (int x : list) {
        vertices[x].removeNode(edge);
    }

    validEdges.removeNode(edge);
    operations.push({0, edge});

}

// Place the vertex in the answer set
void takeVertex(int node, OrderedSubsetList *vertices, OrderedSubsetList *edges, 
    bool *valid, OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations) {

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
        eraseEdge(x, vertices, edges, validEdges, operations);
    }

}

// Fix the node out of the answer
void ignoreVertex(int node, OrderedSubsetList *vertices, OrderedSubsetList *edges,
    bool *valid, OrderedSubsetList &validVertices, stack<pii> &operations) {

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
void undo(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    bool *valid, OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations, vector<vector<int>> &bucket) {

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