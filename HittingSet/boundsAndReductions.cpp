#include <bits/stdc++.h>
#include "boundsAndReductions.h"
#include "orderedSubsetList.h"
#include "operations.h"
#include "setTrie.h"
using namespace std;
using pii = pair<int, int>;

// Calculate the Upperbound
int calculateUpperbound(int m, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    vector<vector<int>> &bucket, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges, stack<pii> &operations, vector<int> &ans) {

    for (int i = 0; i <= m; i++) bucket[i].clear();
    vector<int> validos = validVertices.elements();
    for (int x : validos) {
        bucket[m].push_back(x);
    }

    int resp = 0;
    int initialSize = operations.size();
    
    for (int i = m; i > 0; i--) {
        while (!bucket[i].empty()) {
            int id = bucket[i].back();
            bucket[i].pop_back();
            if (vertices[id].getSize() == i && validVertices.getState(id)) {
                resp++;
                takeVertex(id, vertices, edges, validVertices, validEdges, operations, ans);
            }
            else if (validVertices.getState(id)) {
                bucket[vertices[id].getSize()].push_back(id);
            }
        }
    }


    while (operations.size() > initialSize) {
        undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);
    }    

    return resp;
}

// return ceil of a / b
int ceiling(int a, int b) {
    return a / b + !!(a % b);
}

int maxDegreeBound(vector<OrderedSubsetList> &vertices, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges) {
    int maxDegree = 0;
    vector<int> valid = validVertices.elements();
    for (int x : valid) {
        maxDegree = max(maxDegree, vertices[x].getSize());
    }

    return ceiling(validEdges.getSize(), maxDegree);
}

// returns the lowerBound and calculates for every vertex the new lowerbound if we remove it
int efficiencyBound(vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validEdges, OrderedSubsetList &validVertices,
    vector<pii> &maxDegree, vector<pii> &maxDegreeNode, 
    vector<int> &maxLowerBound) {

    double ans = 0;
    vector<int> valid = validEdges.elements();

    for (int x : valid) {

        vector<int> nodes = edges[x].elements();
        maxDegree[x].first = maxDegree[x].second = 0;
        maxDegreeNode[x].first = maxDegreeNode[x].second = -1;
        
        for (int y : nodes) {
        
            if (vertices[y].getSize() > maxDegree[x].first) {
                maxDegree[x].second = maxDegree[x].first;
                maxDegreeNode[x].second = maxDegreeNode[x].first;
                maxDegree[x].first = vertices[y].getSize();
                maxDegreeNode[x].first = y;
            }
            else if (vertices[y].getSize() > maxDegree[x].second) {
                maxDegree[x].second = vertices[y].getSize();
                maxDegreeNode[x].second = y;
            }
        
        }
        
        ans += 1.0 / maxDegree[x].first;
    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) {
    
        double bonus = 0;
        vector<int> edges = vertices[x].elements();
    
        for (int y : edges) {
            if (maxDegreeNode[y].first == x) {
                bonus += 1.0 / maxDegree[y].second - 1.0 / maxDegree[y].first;
            }
        }

        maxLowerBound[x] = max(maxLowerBound[x], (int)ceil(ans + bonus));
        
    }

    return (int)ceil(ans);
}

int packingBound(int n, vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges,
    vector<OrderedSubsetList> &vertices, vector<int> &sumOfDegrees) {

    vector<int> e = validEdges.elements(), packing;
    for (int x : e) {
        sumOfDegrees[x] = 0;
        vector<int> v = edges[x].elements();
        for (int y : v) sumOfDegrees[x] += vertices[y].getSize();
    }

    sort(e.begin(), e.end(), [&] (int a, int b) {
        return sumOfDegrees[a] < sumOfDegrees[b];
    });

    vector<bool> valid(n, true);

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

    return packing.size();
}

void repacking(int n, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    stack<pii> &operations, vector<int> &maxLowerBound,
    vector<vector<int>> &bucket, vector<int> &ans, vector<int> &sumOfDegrees) {

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
            ignoreVertex(grau[i].second, vertices, edges, validVertices, operations);
            maxLowerBound[grau[i].second] = max(maxLowerBound[grau[i].second], 
                packingBound(n, edges, validEdges, vertices, sumOfDegrees));
            while (operations.size() > initialSize) 
                undo(vertices, edges, validVertices, validEdges, operations, bucket, ans);
        }
    
    }

}

void costlyDiscardPackingBound(int n, vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges,
    vector<OrderedSubsetList> &vertices, OrderedSubsetList &validVertices, 
    vector<vector<int>> &blockedEdges, vector<int> &maxLowerBound, vector<int> &sumOfDegrees) {

    vector<int> e = validEdges.elements(), packing;
    for (int x : e) {
        sumOfDegrees[x] = 0;
        vector<int> v = edges[x].elements();
        for (int y : v) sumOfDegrees[x] += vertices[y].getSize();
    }

    vector<int> v = validVertices.elements();
    for (int x : v) blockedEdges[x].clear();

    sort(e.begin(), e.end(), [&] (int a, int b) {
        return sumOfDegrees[a] < sumOfDegrees[b];
    });

    vector<bool> valid(n, true);

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

        // if the edge only has one vertex it is already in the packing
        if (cnt == 1 && nodes.size() > 1) {
            blockedEdges[block].push_back(x);
        }

    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) {

        vector<int> extraPacking;
        sort(blockedEdges[x].begin(), blockedEdges[x].end(), [&] (int a, int b) {
            return sumOfDegrees[a] < sumOfDegrees[b];
        });
        
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
                    if (z == x) continue;
                    valid[z] = false;
                }
            }
        }

        for (int y : extraPacking) {
            vector<int> verts = edges[y].elements();

            for (int z : verts) {
                if (z == x) continue;
                valid[z] = true;
            } 

        }

        maxLowerBound[x] = max(maxLowerBound[x], (int)(packing.size() + extraPacking.size()));

    }

}

int sumOverPackingBound(int n, vector<OrderedSubsetList> &vertices, vector<OrderedSubsetList> &edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges, vector<int> &sumOfDegrees) {

    vector<int> e = validEdges.elements(), packing;
    for (int x : e) {
        sumOfDegrees[x] = 0;
        vector<int> v = edges[x].elements();
        for (int y : v) sumOfDegrees[x] += vertices[y].getSize();
    }

    sort(e.begin(), e.end(), [&] (int a, int b) {
        return sumOfDegrees[a] < sumOfDegrees[b];
    });

    vector<bool> valid(n, true);

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

    int goal = validEdges.getSize() - packing.size(), sum = 0, ans = 0;
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

    return ans;
}

// Returns edges with size <= 1
// If it has size 1, we pick the corresponding vertex
// If it has size 0, we delete it (Is it possible to have size 0? )
vector<int> unitEdgeRule(vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges) {
    vector<int> ans;

    vector<int> e = validEdges.elements();
    for (int x : e) {
        if (edges[x].getSize() <= 1) ans.push_back(x);
    }

    return ans;
}

vector<int> dominatedEdges(int n, vector<OrderedSubsetList> &edges, OrderedSubsetList &validEdges) {
    vector<int> ans;

    SetTrie validos;
    vector<int> e = validEdges.elements();
    sort(e.begin(), e.end(), [&] (int a, int b){
        return edges[a].getSize() < edges[b].getSize();
    });
    for (int x : e) {
        vector<int> vertices = edges[x].elements();
        vector<bool> elementos(n);
        for (int y : vertices) elementos[y] = true;
        if (!validos.subsetSearch(elementos)) 
            validos.insert(elementos);
        else 
            ans.push_back(x);
    }

    return ans;
}

vector<int> dominatedVertices(int m, vector<OrderedSubsetList> &vertices, OrderedSubsetList &validVertices) {
    vector<int> ans;

    SetTrie validos;
    vector<int> e = validVertices.elements();
    sort(e.begin(), e.end(), [&] (int a, int b){
        return vertices[a].getSize() > vertices[b].getSize();
    });
    for (int x : e) {
        vector<int> arestas = vertices[x].elements();
        vector<bool> elementos(m);
        for (int y : arestas) elementos[y] = true;
        if (!validos.supersetSearch(elementos)) 
            validos.insert(elementos);
        else 
            ans.push_back(x);
    }

    return ans;
}