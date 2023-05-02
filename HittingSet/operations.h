#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <bits/stdc++.h>
#include "orderedSubsetList.h"
using namespace std;
using pii = pair<int, int>;

// Prints the instance
void printAll(int n, int m, OrderedSubsetList *vertices, OrderedSubsetList *edges);

// Erase the edge from its vertices
void eraseEdge(int edge, OrderedSubsetList *vertices, OrderedSubsetList *edges, 
    OrderedSubsetList &validEdges, stack<pii> &operations);

// Place the vertex in the answer set
void takeVertex(int node, OrderedSubsetList *vertices, OrderedSubsetList *edges, 
    bool *valid, OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations);

// Fix the node out of the answer
void ignoreVertex(int node, OrderedSubsetList *vertices, OrderedSubsetList *edges,
    bool *valid, OrderedSubsetList &validVertices, stack<pii> &operations);

// Undo the last deletion of vertex/edge
void undo(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    bool *valid, OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations, vector<vector<int>> &bucket);

#endif