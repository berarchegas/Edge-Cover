// Receives a graph, outputs all paths of size 3

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

vector<int> v[MAXN], atual;
vector<vector<int>> paths;
int vis[MAXN];

void dfs(int node) {
    atual.push_back(node);
    if (atual.size() == 4) {
        if (!vis[node])
            	paths.push_back(atual);
        atual.pop_back();
        return;
    }
    for (int x : v[node]) {
        if ((int)atual.size() == 1 || (atual[(int)atual.size() - 2] != x && atual[0] != x)) {
            dfs(x);
        }
    }
    atual.pop_back();
}

int main () { 
    string s;
    cin >> s;
    ofstream outfile(s);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        v[a].push_back(b);
        v[b].push_back(a);
    }
    for (int i = 1; i <= n; i++) {
        vis[i] = 1;
        dfs(i);
    }
    outfile << n << ' ' << m << ' ' << paths.size() << '\n';
    for (int i = 1; i <= n; i++) {
        for (int x : v[i]) {
            if (x > i) outfile << i << ' ' << x << '\n';
        }
    }
    for (auto x : paths) {
        outfile << x.size() << ' ';
        for (int y : x) outfile << y << ' ';
        outfile << '\n';
    }
    return 0;
}