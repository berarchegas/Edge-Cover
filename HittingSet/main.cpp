#include <bits/stdc++.h>
#include "orderedSubsetList.h"
#include "setTrie.h"
#include "operations.h"
#include "boundsAndReductions.h"
using namespace std;
using pii = pair<int, int>;

// Initial upperBound
int upperBound = 1000000;

// Number of Vertices and Number of Hyperedges 
int n, m;

// Structure for the greedy UpperBound calculation
vector<vector<int>> bucket; 

// Maintains the top two vertex degree for every edge
vector<pii> maxDegree;

// Maintains the top two vertices with greatest degree for every edge
vector<pii> maxDegreeNode;

// What the LowerBound would become if we ignored vertex i
vector<int> maxLowerBound;

// Sum of Vertex Degree in each edge, used for packing
vector<int> sumOfDegrees;

// blockedEdges by each vertex in the costly discard packing bound
vector<vector<int>> blockedEdges;

// edges[i] are the vertices of the i-th edge
vector<OrderedSubsetList> edges;

// vertices[i] are the edges of the i-th vertex
vector<OrderedSubsetList> vertices;

OrderedSubsetList validVertices, validEdges;

// Stack with all the operations we made, so we can rollback
stack<pii> operations;

vector<int> ans;

int cnt = 0;


int branchAndBound(int shift) {

    if (validEdges.getSize() == 0) {
        return shift;
    }

    if (validVertices.getSize() == 0) {
        return upperBound;
    }

    cnt++;

    int initialSize = operations.size();

    upperBound = min(upperBound, calculateUpperbound(m, vertices, edges, 
        bucket, validVertices, validEdges, operations, ans) + shift);

    int lowerBound = 0;

    lowerBound = max(lowerBound, maxDegreeBound(vertices, validVertices, validEdges) + shift);

    if (lowerBound >= upperBound) {
        return upperBound;
    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) maxLowerBound[x] = 0;

    lowerBound = max(lowerBound, efficiencyBound(vertices, edges, validEdges, 
        validVertices, maxDegree, maxDegreeNode, maxLowerBound) + shift);

    if (lowerBound >= upperBound) {
        return upperBound;
    }
    
    lowerBound = max(lowerBound, packingBound(n, edges, validEdges, vertices, sumOfDegrees) + shift);

    if (lowerBound >= upperBound) {
        return upperBound;
    }
    
    lowerBound = max(lowerBound, sumOverPackingBound(n, vertices, 
        edges, validVertices, validEdges, sumOfDegrees) + shift);

    if (lowerBound >= upperBound) {
        return upperBound;
    }

    vector<int> unitEdges = unitEdgeRule(edges, validEdges);

    int took = 0;
    for (int x : unitEdges) {
        if (edges[x].getSize() == 1) {

            int vertice = edges[x].elements()[0];
            took++;
            takeVertex(vertice, vertices, edges, validVertices, validEdges, operations, ans);
        
        }
    }

    if (took) {
        upperBound = min(upperBound, branchAndBound(shift + took));
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    nodes = validVertices.elements();

    costlyDiscardPackingBound(n, edges, validEdges, vertices, validVertices, blockedEdges, maxLowerBound, sumOfDegrees);

    repacking(n, vertices, edges, validVertices, validEdges, 
        operations, maxLowerBound, bucket, ans, sumOfDegrees);

    took = 0;
    for (int x : nodes) {
        if (maxLowerBound[x] + shift >= upperBound) {

            took++;
            takeVertex(x, vertices, edges, validVertices, validEdges, operations, ans);

        }
    }

    if (took) {
        upperBound = min(upperBound, branchAndBound(shift + took));
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    vector<int> invalidEdges = dominatedEdges(n, edges, validEdges);

    for (int x : invalidEdges) {
        eraseEdge(x, vertices, edges, validEdges, operations);
    }

    unitEdges = unitEdgeRule(edges, validEdges);

    took = 0;
    for (int x : unitEdges) {
        if (edges[x].getSize() == 1) {

            int vertice = edges[x].elements()[0];
            took++;
            takeVertex(vertice, vertices, edges, validVertices, validEdges, operations, ans);
        
        }
    }
    
    if (took) {
        upperBound = min(upperBound, branchAndBound(shift + took));
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }


    ////////////////////

    upperBound = min(upperBound, calculateUpperbound(m, vertices, edges, 
        bucket, validVertices, validEdges, operations, ans) + shift);

    lowerBound = max(lowerBound, maxDegreeBound(vertices, validVertices, validEdges) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    nodes = validVertices.elements();
    for (int x : nodes) maxLowerBound[x] = 0;

    lowerBound = max(lowerBound, efficiencyBound(vertices, edges, validEdges, 
        validVertices, maxDegree, maxDegreeNode, maxLowerBound) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }
    
    lowerBound = max(lowerBound, packingBound(n, edges, validEdges, vertices, sumOfDegrees) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }
    
    lowerBound = max(lowerBound, sumOverPackingBound(n, vertices, 
        edges, validVertices, validEdges, sumOfDegrees) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    costlyDiscardPackingBound(n, edges, validEdges, vertices, validVertices, blockedEdges, maxLowerBound, sumOfDegrees);

    repacking(n, vertices, edges, validVertices, validEdges, 
        operations, maxLowerBound, bucket, ans, sumOfDegrees);

    took = 0;
    for (int x : nodes) {
        if (maxLowerBound[x] + shift >= upperBound) {

            took++;
            takeVertex(x, vertices, edges, validVertices, validEdges, operations, ans);

        }
    }

    if (took) {
        upperBound = min(upperBound, branchAndBound(shift + took));
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    vector<int> invalidVertices = dominatedVertices(m, vertices, validVertices);

    for (int x : invalidVertices) {
        ignoreVertex(x, vertices, edges, validVertices, operations);
    }

    unitEdges = unitEdgeRule(edges, validEdges);

    took = 0;
    for (int x : unitEdges) {
        if (edges[x].getSize() == 1) {

            int vertice = edges[x].elements()[0];
            took++;
            takeVertex(vertice, vertices, edges, validVertices, validEdges, operations, ans);
        
        }
    }
    
    if (took) {
        upperBound = min(upperBound, branchAndBound(shift + took));
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }


    ////////////////////

    upperBound = min(upperBound, calculateUpperbound(m, vertices, edges, 
        bucket, validVertices, validEdges, operations, ans) + shift);

    lowerBound = max(lowerBound, maxDegreeBound(vertices, validVertices, validEdges) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    nodes = validVertices.elements();
    for (int x : nodes) maxLowerBound[x] = 0;

    lowerBound = max(lowerBound, efficiencyBound(vertices, edges, validEdges, 
        validVertices, maxDegree, maxDegreeNode, maxLowerBound) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }
    
    lowerBound = max(lowerBound, packingBound(n, edges, validEdges, vertices, sumOfDegrees) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }
    
    lowerBound = max(lowerBound, sumOverPackingBound(n, vertices, 
        edges, validVertices, validEdges, sumOfDegrees) + shift);

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    costlyDiscardPackingBound(n, edges, validEdges, vertices, validVertices, blockedEdges, maxLowerBound, sumOfDegrees);

    repacking(n, vertices, edges, validVertices, validEdges, 
        operations, maxLowerBound, bucket, ans, sumOfDegrees);

    took = 0;
    for (int x : nodes) {
        if (maxLowerBound[x] + shift >= upperBound) {

            took++;
            takeVertex(x, vertices, edges, validVertices, validEdges, operations, ans);

        }
    }

    if (took) {
        upperBound = min(upperBound, branchAndBound(shift + took));
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }


    ////////////////////


    int branchNode = -1, maximumDegree = -1;
    nodes = validVertices.elements();

    for (int x : nodes) {
        if (vertices[x].getSize() > maximumDegree) {
            maximumDegree = vertices[x].getSize();
            branchNode = x;
        }
    }

    int branchingSize = operations.size();
    takeVertex(branchNode, vertices, edges, validVertices, validEdges, operations, ans);
    upperBound = min(upperBound, branchAndBound(shift + 1));

    if (lowerBound >= upperBound) {
        while (operations.size() > initialSize) 
            undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

        return upperBound;
    }

    while (operations.size() > branchingSize) 
        undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

    ignoreVertex(branchNode, vertices, edges, validVertices, operations);
    upperBound = min(upperBound, branchAndBound(shift));

    while (operations.size() > initialSize) 
        undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);

    return upperBound;
}

int main() {
    freopen("in.txt", "r", stdin);
    cin >> n >> m;

    validVertices = OrderedSubsetList(vector<int>(), n);
    vertices = vector<OrderedSubsetList> (n);
    maxLowerBound = vector<int> (n);
    blockedEdges = vector<vector<int>> (n);
    for (int i = 0; i < n; i++) {
        validVertices.pushBack(i);
        vertices[i] = OrderedSubsetList(vector<int>(), m);
    }

    validEdges = OrderedSubsetList(vector<int>(), m);
    edges = vector<OrderedSubsetList> (m);
    maxDegree = vector<pii> (m);
    maxDegreeNode = vector<pii> (m);
    sumOfDegrees = vector<int> (m);
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
        bucket[m].push_back(i);
    }

    cout << branchAndBound(0) << '\n';
    cout << cnt << '\n';
    return 0;
}