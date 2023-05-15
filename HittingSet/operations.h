#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <bits/stdc++.h>
#include "orderedSubsetList.h"
using namespace std;
using pii = pair<int, int>;

// Prints the instance
void printAll(int n, int m, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges, vector<vector<int>> &bucket);

// Erase the edge from its vertices
void eraseEdge(int edge, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges, 
    OrderedSubsetList &validEdges, stack<pii> &operations);

// Place the vertex in the answer set
void takeVertex(int node, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges, 
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations, vector<int> &ans);

// Fix the node out of the answer
void ignoreVertex(int node, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, stack<pii> &operations);

// Undo the last deletion of vertex/edge
void undo(vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations, vector<vector<int>> &bucket, vector<int> &ans);

#endif