#ifndef BOUNDSANDREDUCTIONS_H
#define BOUNDSANDREDUCTIONS_H
#include <bits/stdc++.h>
#include "orderedSubsetList.h"
using namespace std;
using pii = pair<int, int>;

// Calculate the Upperbound
int calculateUpperbound(int m, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    vector<vector<int>> &bucket, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges, stack<pii> &operations, vector<int> &ans);

// return ceil of a / b
int ceiling(int a, int b);

int maxDegreeBound(vector<OrderedSubsetList> &vertices, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges);

// returns the lowerBound and calculates for every vertex the new lowerbound if we remove it
int efficiencyBound(vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validEdges, OrderedSubsetList &validVertices,
    vector<pii> &maxDegree, vector<pii> &maxDegreeNode, 
    vector<int> &maxLowerBound);

int packingBound(int n, vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges);

void repacking(int n, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations, vector<int> &maxLowerBound,
    vector<vector<int>> &bucket, vector<int> &ans);

void costlyDiscardPackingBound(int n, vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges,
    OrderedSubsetList &validVertices, vector<vector<int>> &blockedEdges, 
    vector<int> &maxLowerBound);

int sumOverPackingBound(int n, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges);

// Returns edges with size <= 1
// If it has size 1, we pick the corresponding vertex
// If it has size 0, we delete it (Is it possible to have size 0? )
vector<int> unitEdgeRule(vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges);

vector<int> dominatedEdges(int n, vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges);

vector<int> dominatedVertices(int m, vector<OrderedSubsetList> &vertices, OrderedSubsetList &validVertices);

#endif