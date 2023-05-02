#include <bits/stdc++.h>
#include "boundsAndReductions.h"
#include "orderedSubsetList.h"
#include "operations.h"
#include "setTrie.h"
using namespace std;
using pii = pair<int, int>;

// Calculate the Upperbound
int calculateUpperbound(int m, OrderedSubsetList *vertices, OrderedSubsetList *edges,
    bool *valid, vector<vector<int>> &bucket, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges, stack<pii> &operations) {
    int ans = 0;
    int initialSize = operations.size();
    
    for (int i = m; i > 0; i--) {
        while (!bucket[i].empty()) {
            int id = bucket[i].back();
            bucket[i].pop_back();
            if (vertices[id].getSize() == i && valid[id]) {
                ans++;
                takeVertex(id, vertices, edges, valid, validVertices, validEdges, operations);
            }
            else if (valid[id]) {
                bucket[vertices[id].getSize()].push_back(id);
            }
        }
    }


    while (operations.size() > initialSize) {
        undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);
    }    

    bucket[0].clear();
    vector<int> validos = validVertices.elements();
    for (int x : validos) {
        bucket[vertices[x].getSize()].push_back(x);
    }

    return ans;
}

// return ceil of a / b
int ceiling(int a, int b) {
    return a / b + !!(a % b);
}

int maxDegreeBound(OrderedSubsetList *vertices, OrderedSubsetList &validVertices,
    OrderedSubsetList &validEdges) {
    int maxDegree = 0;
    vector<int> valid = validVertices.elements();
    for (int x : valid) {
        maxDegree = max(maxDegree, vertices[x].getSize());
    }

    return ceiling(validEdges.getSize(), maxDegree);
}

// returns the lowerBound and calculates for every vertex the new lowerbound if we remove it
int efficiencyBound(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    OrderedSubsetList &validEdges, OrderedSubsetList &validVertices,
    int **maxDegree, int **maxDegreeNode, int *maxLowerBound) {

    double ans = 0;
    vector<int> valid = validEdges.elements();

    for (int x : valid) {

        vector<int> nodes = edges[x].elements();
        maxDegree[x][0] = maxDegree[x][1] = 0;
        maxDegreeNode[x][0] = maxDegreeNode[x][1] = -1;
        
        for (int y : nodes) {
        
            if (vertices[y].getSize() > maxDegree[x][0]) {
                maxDegree[x][1] = maxDegree[x][0];
                maxDegreeNode[x][1] = maxDegreeNode[x][0];
                maxDegree[x][0] = vertices[y].getSize();
                maxDegreeNode[x][0] = y;
            }
            else if (vertices[y].getSize() > maxDegree[x][1]) {
                maxDegree[x][1] = vertices[y].getSize();
                maxDegreeNode[x][1] = y;
            }
        
        }
        
        ans += 1.0 / maxDegree[x][0];
    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) {
    
        double bonus = 0;
        vector<int> edges = vertices[x].elements();
    
        for (int y : edges) {
            if (maxDegreeNode[y][0] == x) {
                bonus += 1.0 / maxDegree[y][1] - 1.0 / maxDegree[y][0];
            }
        }

        maxLowerBound[x] = max(maxLowerBound[x], (int)ceil(ans + bonus));
        
    }

    // dont know if this breaks
    return (int)ceil(ans);
}

int packingBound(OrderedSubsetList *edges, OrderedSubsetList &validEdges, bool *valid) {
    vector<int> e = validEdges.elements(), packing;
    sort(e.begin(), e.end(), [&] (int a, int b) {
        return edges[a].getSize() > edges[b].getSize();
    });

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

    for (int x : packing) {
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            valid[y] = true;
        } 

    }

    return packing.size();
}

void repacking(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges,
    bool *valid, stack<pii> &operations, int *maxLowerBound,
    vector<vector<int>> &bucket) {

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
            ignoreVertex(grau[i].second, vertices, edges, valid, validVertices, operations);
            maxLowerBound[grau[i].second] = max(maxLowerBound[grau[i].second], packingBound(edges, validEdges, valid));
            while (operations.size() > initialSize) 
                undo(vertices, edges, valid, validVertices, validEdges, operations, bucket);
        }
    
    }

}

void costlyDiscardPackingBound(OrderedSubsetList *edges, OrderedSubsetList &validEdges,
    OrderedSubsetList &validVertices, bool *valid, vector<vector<int>> &blockedEdges, 
    int *maxLowerBound) {
    vector<int> e = validEdges.elements(), packing;
    sort(e.begin(), e.end(), [&] (int a, int b) {
        return edges[a].getSize() > edges[b].getSize();
    });

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

        // if the edge only has one vertex 
        // then if we remove this vertex the edge becomes insignificant
        if (cnt == 1 && nodes.size() > 1) {
            blockedEdges[block].push_back(x);
        } 

    }

    vector<int> nodes = validVertices.elements();
    for (int x : nodes) {
        vector<int> extraPacking;
        sort(blockedEdges[x].begin(), blockedEdges[x].end(), [&] (int a, int b) {
            return edges[a].getSize() > edges[b].getSize();
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
                    valid[z] = false;
                }
            }
        }

        for (int y : extraPacking) {
            vector<int> verts = edges[y].elements();

            for (int z : verts) {
                valid[z] = true;
            } 

        }

        maxLowerBound[x] = max(maxLowerBound[x], (int)(packing.size() + extraPacking.size()));

    }

    for (int x : packing) {
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            valid[y] = true;
        } 

    }

}

int sumOverPackingBound(OrderedSubsetList *vertices, OrderedSubsetList *edges,
    OrderedSubsetList &validVertices, OrderedSubsetList &validEdges, 
    bool *valid) {
    vector<int> e = validEdges.elements();
    vector<int> packing;
    sort(e.begin(), e.end(), [&] (int a, int b) {
        return edges[a].getSize() > edges[b].getSize();
    });

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

    int goal = validEdges.getSize(), sum = 0, ans = 0;
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

    for (int x : packing) {
        vector<int> nodes = edges[x].elements();

        for (int y : nodes) {
            valid[y] = true;
        } 

    }

    return ans;
}

// Returns edges with size <= 1
// If it has size 1, we pick the corresponding vertex
// If it has size 0, we delete it (Is it possible to have size 0? )
vector<int> unitEdgeRule(OrderedSubsetList *edges, OrderedSubsetList &validEdges) {
    vector<int> ans;

    vector<int> e = validEdges.elements();
    for (int x : e) {
        if (edges[x].getSize() <= 1) ans.push_back(x);
    }

    return ans;
}

vector<int> dominatedEdges(int n, OrderedSubsetList *edges, OrderedSubsetList &validEdges) {
    vector<int> ans;

    SetTrie validos;
    vector<int> e = validEdges.elements();
    for (int x : e) {
        vector<int> vertices = edges[x].elements();
        vector<bool> elementos(n);
        for (int y : vertices) elementos[y] = true;
        if (!validos.supersetSearch(elementos)) 
            validos.insert(elementos);
        else 
            ans.push_back(x);
    }

    return ans;
}

vector<int> dominatedVertices(int m, OrderedSubsetList *vertices, OrderedSubsetList &validVertices) {
    vector<int> ans;

    SetTrie validos;
    vector<int> e = validVertices.elements();
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