#include <bits/stdc++.h>
using namespace std;
const int MAXN = 2e5 + 5;

vector<int> v[MAXN];
vector<vector<int>> path[MAXN], par_path[MAXN];
int dp[MAXN][2];

void dfs(int node) {
    for (int x : v[node]) {
        dfs(x);
        dp[node][0] += dp[x][1];
    }
    
    // calc dp[node][1]
    dp[node][1] = dp[node][0] + 1;
    for (auto x : par_path[node]) {
        // aqui deveria ser tenta = dp[node][0] + 1?
        int tenta = dp[node][1] + 1;
        for (int y : x) {
            tenta += dp[y][0] - dp[y][1];
        }
        dp[node][1] = min(dp[node][1], tenta);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, p, a, b;
    cin >> n >> m >> p;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        v[a].push_back(b);
    }
    for (int i = 0; i < p; i++) {
        int tam, ini, filho;
        cin >> tam >> ini;
        vector<int> p(tam - 1);
        for (int &x : p) cin >> x;
        filho = p[0];
        path[ini].push_back(p);
        par_path[filho].push_back(p);
    }
    dfs(1);
    cout << dp[1][0] << '\n';
    return 0;
}
