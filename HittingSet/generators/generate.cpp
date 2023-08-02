#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using ll = long long;

mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());

int main() {
    freopen("in.txt", "w", stdout);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> opts;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                opts.push_back({i, j, k});
            }
        }
    }
    set<vector<int>> st;
    while (st.size() < m) {
        int id = rng() % ((int)opts.size());
        st.insert(opts[id]);
    }
    cout << n << ' ' << m << '\n';
    for (vector<int> x : st) {
        cout << x.size() << ' ';
        for (int y : x) cout << y << ' ';
        cout << '\n';
    }
    return 0;
}