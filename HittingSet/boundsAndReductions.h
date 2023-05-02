#ifndef BOUNDSANDREDUCTIONS_H
#define BOUNDSANDREDUCTIONS_H
#include <bits/stdc++.h>
#include "orderedSubsetList.h"
using namespace std;
using pii = pair<int, int>;

// Calculate the Upperbound
int calculateUpperbound(int m, OrderedSubsetList *vertices, OrderedSubsetList *edges,
    bool *valid, vector<vector<int>> &bucket, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges, stack<pii> &operations);

// return ceil of a / b
int ceiling(int a, int b);

int maxDegreeBound(OrderedSubsetList *vertices, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges);

// returns the lowerBound and calculates for every vertex the new lowerbound if we remove it
int efficiencyBound(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    OrderedSubsetList &validEdges, OrderedSubsetList &validVertices,
    int **maxDegree, int **maxDegreeNode, int *maxLowerBound);

int packingBound(OrderedSubsetList *edges, OrderedSubsetList &validEdges, bool *valid);

void repacking(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    bool *valid, stack<pii> &operations, int *maxLowerBound,
    vector<vector<int>> &bucket);

void costlyDiscardPackingBound(OrderedSubsetList *edges, OrderedSubsetList &validEdges,
    OrderedSubsetList &validVertices, bool *valid, vector<vector<int>> &blockedEdges, 
    int *maxLowerBound);

int sumOverPackingBound(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges, 
    bool *valid);

// Returns edges with size <= 1
// If it has size 1, we pick the corresponding vertex
// If it has size 0, we delete it (Is it possible to have size 0? )
vector<int> unitEdgeRule(OrderedSubsetList *edges, OrderedSubsetList &validEdges);

vector<int> dominatedEdges(OrderedSubsetList *edges, OrderedSubsetList &validEdges);

vector<int> dominatedVertices(OrderedSubsetList *vertices, OrderedSubsetList &validVertices);

#endif