// Tries to color using 3 colors a block3 instance
// It is probably always impossible

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

vector<pii> v[MAXN];
int cor[MAXN], n;

bool dfs(int node) {
    if (node == n + 1) return true;
    bool ok = false;
    set<int> danger;
    for (pii x : v[node]) {
        danger.insert(cor[x.first]);
        danger.insert(cor[x.second]);
    }
    danger.erase(0);
    if (danger.size() == 3) return false;
    for (int i = 1; i <= 3 && !ok; i++) {
        if (!danger.count(i)) {
            cor[node] = i;
            ok |= dfs(node + 1);
        }
    }
    return ok;
}

int main () { 
    int k;
    cin >> k;
    n = 27 * k;
    for (int i = 0; i < n; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        v[a].push_back({b, c});
        v[b].push_back({a, c});
        v[c].push_back({a, b});
    }
    cout << dfs(1) << '\n';
    return 0;
}