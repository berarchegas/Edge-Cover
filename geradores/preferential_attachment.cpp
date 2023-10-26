// Generates a random graph using the Preferential Attachment method
// We create a new node at each iteration and add edges with probability p
// p = (deg(u) + 1) / (V + E)

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e5 + 5;

vector<int> graph[MAXN];

int main () { 
    freopen("in.txt", "w", stdout);
	int n;
	cin >> n;
    int e = 0;
    for (int v = 2; v <= n; v++) {
        for (int u = 1; u < v; u++) {
            int a = rng() % (v + e + 1);
            if (graph[u].size() + 1 > a) {
                graph[u].push_back(v);
                graph[v].push_back(u);
                e++;
            }
        }
        if (graph[v].empty()) {
            int pai = rng() % (v - 1) + 1;
            graph[pai].push_back(v);
            graph[v].push_back(pai);
            e++;
        }
    }
    cout << n << ' ' << e << '\n';
    for (int i = 1; i <= n; i++) {
        for (int x : graph[i]) if (x > i) cout << i << ' ' << x << '\n';
    }
    return 0;
}