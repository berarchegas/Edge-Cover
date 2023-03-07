#include <bits/stdc++.h>
using namespace std;
using ll = long long;
mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());
const int INF = 1e9;
const int MAXN = 1e3 + 5;

struct graph {
    int hashing, n, cost;
    vector<vector<bool>> adjacency;
    set<int> availablePaths;
    graph(int _n = 0) {
        n = _n, cost = 0, hashing = 0;
        adjacency = vector<vector<bool>> (n, vector<bool> (n));
        availablePaths = set<int> ();
    }
};

int lowerBoundEuler(graph &g) {
    vector<int> vis(g.n);
    int ans = 0;
    for (int i = 0; i < g.n; i++) {
        if (!vis[i]) {
            int qtd = 0;
            vis[i] = 1;
            queue<int> fila;
            fila.push(i);
            while (!fila.empty()) {
                int at = fila.front();
                fila.pop();
                int dif = 0;
                for (int j = 0; j < g.n; j++) {
                    if (g.adjacency[at][j]) {
                        dif++;
                        if (!vis[j]) {
                            vis[j] = 1;
                            fila.push(j);
                        }
                    }
                    else if (g.adjacency[j][at]) {
                        dif--;
                        if (!vis[j]) {
                            vis[j] = 1;
                            fila.push(j);
                        }
                    }
                }
                qtd += max(0, dif);
            }
            ans += max(1, qtd);
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, p, a, b;
    cin >> n >> m;
    graph g(n);
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        a--, b--;
        g.adjacency[a][b] = true;
    }
    cout << lowerBoundEuler(g) << '\n';
    return 0;
}