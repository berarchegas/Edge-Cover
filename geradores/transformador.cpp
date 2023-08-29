// Transforms 3dmr instances into PCEC instances

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

// cada vertice cria 4 vertices
// caminhos auxiliares pra X e Y
// caminhos completos pra cada tripla

set<int> v[MAXN];
vector<vector<int>> paths;

int main () { 
    freopen("in.txt", "w", stdout);
    int n, a, b, c;
    cin >> n;
    for (int i = 0; i < 2 * n; i++) {
        // 0 = 0, 1 = 1, 2 = in, 3 = out
        v[4 * i].insert(4 * i + 3);
        v[4 * i + 1].insert(4 * i + 3);
        v[4 * i + 2].insert(4 * i + 3);
    }
    for (int i = 2 * n; i < 3 * n; i++) {
        // 0 = in, 1 = out
        v[8 * n + 2 * (i - 2 * n)].insert(8 * n + 2 * (i - 2 * n) + 1);
    }
    for (int i = 0; i < 3 * n; i++) {
        cin >> a >> b >> c;
        v[4 * a + 3].insert(4 * b + 2);
        v[4 * b + 3].insert(8 * n + 2 * (c - 2 * n));
        paths.push_back({4 * a + 2, 4 * a + 3, 4 * b + 2, 4 * b + 3, 8 * n + 2 * (c - 2 * n), 8 * n + 2 * (c - 2 * n) + 1});
    }
    for (int i = 0; i < 2 * n; i++) {
        // auxiliares
        paths.push_back({4 * i, 4 * i + 3});
        paths.push_back({4 * i + 1, 4 * i + 3});
        paths.push_back({4 * i + 2, 4 * i + 3});
        for (int x : v[4 * i + 3]) {
            paths.push_back({4 * i, 4 * i + 3, x});
            paths.push_back({4 * i + 1, 4 * i + 3, x});
            paths.push_back({4 * i + 2, 4 * i + 3, x});
        }
    }
    for (int i = 2 * n; i < 3 * n; i++) {
        // auxiliares
        paths.push_back({8 * n + 2 * (i - 2 * n), 8 * n + 2 * (i - 2 * n) + 1});
    }

    int m = 0;
    for (int i = 0; i < 10 * n; i++) m += v[i].size();

    cout << 10 * n << ' ' << m << ' ' << paths.size() << '\n';
    for (int i = 0; i < 10 * n; i++) {
        for (int x : v[i]) cout << i << ' ' << x << '\n';
    }
    for (auto x : paths) {
        cout << x.size() << '\n';
        for (int y : x) cout << y << ' ';
        cout << '\n';
    }
    return 0;
}