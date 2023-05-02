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

// Design Decision: We are maintaining a few global structures to avoid memory redundancy
// We need to be able to reset these structures properly before using each one
// We could also test always going through every vertex and edge instead of getting vectors with active elements


bool valid[10];

// We need to reset maxLowerBound properly 
int n, m, maxDegree[10][2], maxDegreeNode[10][2], maxLowerBound[10];
vector<vector<int>> bucket;

// blockedEdges by each vertex in the costly discard packing bound
vector<vector<int>> blockedEdges;

// edges[i] are the vertices of the i-th edge
// vertices[i] are the edges of the i-th vertex
OrderedSubsetList edges[10], vertices[10], validVertices, validEdges;
stack<pii> operations;

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

    printAll(n, m, vertices, edges);

    cout << calculateUpperbound(m, vertices, edges, valid, bucket, validVertices, validEdges, operations) << '\n';

    eraseEdge(3, vertices, edges, validEdges, operations);

    ignoreVertex(0, vertices, edges, valid, validVertices, operations);

    calculateUpperbound(m, vertices, edges, valid, bucket, validVertices, validEdges, operations);

    takeVertex(2, vertices, edges, valid, validVertices, validEdges, operations);

    eraseEdge(2, vertices, edges, validEdges, operations);

    calculateUpperbound(m, vertices, edges, valid, bucket, validVertices, validEdges, operations);

    eraseEdge(1, vertices, edges, validEdges, operations);

    ignoreVertex(1, vertices, edges, valid, validVertices, operations);

    calculateUpperbound(m, vertices, edges, valid, bucket, validVertices, validEdges, operations);

    takeVertex(4, vertices, edges, valid, validVertices, validEdges, operations);

    while (!operations.empty()) undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);

    cout << calculateUpperbound(m, vertices, edges, valid, bucket, validVertices, validEdges, operations) << '\n';

    printAll(n, m, vertices, edges);

    return 0;
}