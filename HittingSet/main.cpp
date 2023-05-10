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

// blockedEdges by each vertex in the costly discard packing bound
vector<vector<int>> blockedEdges;

// edges[i] are the vertices of the i-th edge
vector<OrderedSubsetList> edges;

// vertices[i] are the edges of the i-th vertex
vector<OrderedSubsetList> vertices;

OrderedSubsetList validVertices, validEdges;

// Stack with all the operations we made, so we can rollback
stack<pii> operations;


int branchAndBound(int shift) {
    cout << "Start"<< endl;

    if (validEdges.getSize() == 0) {
        cout << "Found answer = " << shift << endl;
        return shift;
    }

    int initialSize = operations.size();

    cout << "Calculate UpperBound" << endl;

    upperBound = min(upperBound, calculateUpperbound(m, vertices, edges, 
        bucket, validVertices, validEdges, operations) + shift);

    cout << "Calculate Max Degree Bound" << endl;

    int lowerBound = 0;

    lowerBound = max(lowerBound, maxDegreeBound(vertices, validVertices, validEdges) + shift);

    if (lowerBound > upperBound) {
        cout << "Pruning" << endl;
        return upperBound;
    }

    cout << "Calculate Efficiency Bound" << endl;

    lowerBound = max(lowerBound, efficiencyBound(vertices, edges, validEdges, 
        validVertices, maxDegree, maxDegreeNode, maxLowerBound) + shift);

    if (lowerBound > upperBound) {
        cout << "Pruning" << endl;
        return upperBound;
    };
    
    // lowerBound = max(lowerBound, packingBound(edges, validEdges, validVertices.getAliveElements()) + shift);

    // if (lowerBound > upperBound) return upperBound;
    
    // lowerBound = max(lowerBound, sumOverPackingBound(vertices, 
    //     edges, validVertices, validEdges, validVertices.getAliveElements()) + shift);

    // if (lowerBound > upperBound) return upperBound;

    cout << "Unit Edge Rule" << endl;

    vector<int> unitEdges = unitEdgeRule(edges, validEdges);

    for (int x : unitEdges) {
        if (edges[x].getSize() == 1) {
            int vertice = edges[x].elements()[0];

            takeVertex(vertice, vertices, edges, validVertices, validEdges, operations);
            upperBound = min(upperBound, branchAndBound(shift + 1));
            while (operations.size() > initialSize) 
                undo(vertices, edges, validVertices, validEdges, operations, bucket);

            return upperBound;
        }
        else {
            eraseEdge(x, vertices, edges, validEdges, operations);
        }
    }

    // vector<int> nodes = validVertices.elements();

    // for (int x : nodes) {
    //     if (maxLowerBound[x] > upperBound) {

    //         takeVertex(x, vertices, edges, validVertices, validEdges, operations);
    //         upperBound = min(upperBound, branchAndBound(shift + 1));
    //         while (operations.size() > initialSize) 
    //             undo(vertices, edges, validVertices, validEdges, operations, bucket);

    //         return upperBound;

    //     }
    // }

    // costlyDiscardPackingBound(edges, validEdges, validVertices, validVertices.getAliveElements(), blockedEdges, maxLowerBound);

    // for (int x : nodes) {
    //     if (maxLowerBound[x] > upperBound) {

    //         takeVertex(x, vertices, edges, validVertices, validEdges, operations);
    //         upperBound = min(upperBound, branchAndBound(shift + 1));
    //         while (operations.size() > initialSize) 
    //             undo(vertices, edges, validVertices, validEdges, operations, bucket);

    //         return upperBound;

    //     }
    // }

    // repacking(vertices, edges, validVertices, validEdges, 
    //     validVertices.getAliveElements(), operations, maxLowerBound, bucket);

    // for (int x : nodes) {
    //     if (maxLowerBound[x] > upperBound) {

    //         takeVertex(x, vertices, edges, validVertices, validEdges, operations);
    //         upperBound = min(upperBound, branchAndBound(shift + 1));
    //         while (operations.size() > initialSize) 
    //             undo(vertices, edges, validVertices, validEdges, operations, bucket);

    //         return upperBound;

    //     }
    // }

    // vector<int> invalidEdges = dominatedEdges(n, edges, validEdges);

    // for (int x : invalidEdges) {
    //     eraseEdge(x, vertices, edges, validEdges, operations);
    // }

    // vector<int> invalidVertices = dominatedVertices(m, vertices, validVertices);

    // for (int x : invalidVertices) {
    //     ignoreVertex(x, vertices, edges, validVertices, operations);
    // }

    int branchNode = -1, maximumDegree = -1;
    vector<int> nodes = validVertices.elements();

    for (int x : nodes) {
        if (vertices[x].getSize() > maximumDegree) {
            maximumDegree = vertices[x].getSize();
            branchNode = x;
        }
    }

    cout << "Pick " << branchNode << endl;

    int branchingSize = operations.size();
    takeVertex(branchNode, vertices, edges, validVertices, validEdges, operations);
    upperBound = min(upperBound, branchAndBound(shift + 1));

    while (operations.size() > branchingSize) 
        undo(vertices, edges, validVertices, validEdges, operations, bucket);

    cout << "Ignore " << branchNode << endl;
 
    ignoreVertex(branchNode, vertices, edges, validVertices, operations);
    upperBound = min(upperBound, branchAndBound(shift));

    while (operations.size() > initialSize) 
        undo(vertices, edges, validVertices, validEdges, operations, bucket);

    return upperBound;
}

int main() {
    // freopen("in.txt", "r", stdin);
    cin >> n >> m;

    validVertices = OrderedSubsetList(vector<int>(), n);
    vertices = vector<OrderedSubsetList> (n);
    maxLowerBound = vector<int> (n);
    for (int i = 0; i < n; i++) {
        validVertices.pushBack(i);
        vertices[i] = OrderedSubsetList(vector<int>(), m);
    }

    validEdges = OrderedSubsetList(vector<int>(), m);
    edges = vector<OrderedSubsetList> (m);
    blockedEdges = vector<vector<int>> (m);
    maxDegree = vector<pii> (m);
    maxDegreeNode = vector<pii> (m);
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