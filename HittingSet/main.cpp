#include <bits/stdc++.h>
#include "orderedSubsetList.h"
#include "setTrie.h"
#include "operations.h"
#include "boundsAndReductions.h"
using namespace std;
using pii = pair<int, int>;

// I am creating a lot of auxiliar Nodes* and maybe I can just do a global one?
// I keep passing vector through the elements function, but this is not optimized
// Pass vectors by reference

// We are using kind of arbitrarily vectors and arrays. We could change it to all vector or all array
// Custom Sort Function

// Using a lot of memory so we can do the O(1) insertions and deletions

// Do we create a new Set Trie every time or do we create an erase operation ? 

// Order of operations

// Design Decision: We are maintaining a few global structures to avoid memory redundancy
// We need to be able to reset these structures properly before using each one
// We could also test always going through every vertex and edge instead of getting vectors with active elements

int upperBound = 1000000;

bool *valid;

// We need to reset maxLowerBound properly 
int n, m, **maxDegree, **maxDegreeNode, *maxLowerBound;
vector<vector<int>> bucket;

// blockedEdges by each vertex in the costly discard packing bound
vector<vector<int>> blockedEdges;

// edges[i] are the vertices of the i-th edge
// vertices[i] are the edges of the i-th vertex
OrderedSubsetList *edges, *vertices, validVertices, validEdges;
stack<pii> operations;


// if we decide something because of the bounds we dont have to branch duh
int branchAndBound(int shift) {

    if (validEdges.getSize() == 0) 
        return shift;

    int initialSize = operations.size();

    upperBound = min(upperBound, calculateUpperbound(m, vertices, edges, 
        valid, bucket, validVertices, validEdges, operations) + shift);

    int lowerBound = 0;

    lowerBound = max(lowerBound, maxDegreeBound(vertices, validVertices, validEdges) + shift);

    if (lowerBound > upperBound) return upperBound;

    lowerBound = max(lowerBound, efficiencyBound(vertices, edges, validEdges, 
        validVertices, maxDegree, maxDegreeNode, maxLowerBound) + shift);

    if (lowerBound > upperBound) return upperBound;
    
    lowerBound = max(lowerBound, packingBound(edges, validEdges, valid) + shift);

    if (lowerBound > upperBound) return upperBound;
    
    lowerBound = max(lowerBound, sumOverPackingBound(vertices, 
        edges, validVertices, validEdges, valid) + shift);

    if (lowerBound > upperBound) return upperBound;

    vector<int> unitEdges = unitEdgeRule(edges, validEdges);

    for (int x : unitEdges) {
        if (edges[x].getSize() == 1) {
            int vertice = edges[x].elements()[0];

            int beforeSize = operations.size();
            takeVertex(vertice, vertices, edges, valid, validVertices, validEdges, operations);
            upperBound = max(upperBound, branchAndBound(shift + 1));
            while (operations.size() > beforeSize) 
                undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);

        }
        else {
            eraseEdge(x, vertices, edges, validEdges, operations);
        }
    }

    vector<int> nodes = validVertices.elements();

    for (int x : nodes) {
        if (maxLowerBound[x] > upperBound) {

            int beforeSize = operations.size();
            takeVertex(x, vertices, edges, valid, validVertices, validEdges, operations);
            upperBound = max(upperBound, branchAndBound(shift + 1));
            while (operations.size() > beforeSize) 
                undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);

        }
    }

    costlyDiscardPackingBound(edges, validEdges, validVertices, valid, blockedEdges, maxLowerBound);

    repacking(vertices, edges, validVertices, validEdges, valid, operations, maxLowerBound, bucket);

    for (int x : nodes) {
        if (maxLowerBound[x] > upperBound) {

            int beforeSize = operations.size();
            takeVertex(x, vertices, edges, valid, validVertices, validEdges, operations);
            upperBound = max(upperBound, branchAndBound(shift + 1));
            while (operations.size() > beforeSize) 
                undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);

        }
    }

    vector<int> invalidEdges = dominatedEdges(edges, validEdges);

    for (int x : invalidEdges) {
        eraseEdge(x, vertices, edges, validEdges, operations);
    }

    vector<int> invalidVertices = dominatedVertices(vertices, validVertices);

    for (int x : invalidVertices) {
        int beforeSize = operations.size();
        ignoreVertex(x, vertices, edges, valid, validVertices, operations);
        upperBound = max(upperBound, branchAndBound(shift));
        while (operations.size() > beforeSize) 
            undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);
    }

    while (operations.size() > initialSize) 
        undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);
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

    cout << branchAndBound(0) << '\n';

    return 0;
}