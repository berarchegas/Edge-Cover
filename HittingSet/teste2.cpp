#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using ll = long long;

mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());

int main() {
    freopen("in.txt", "w", stdout);
    int n, m;
    cin >> n >> m;
    set<vector<int>> st;
    while (st.size() < m) {
        vector<int> v;
        for (int i = 0; i < n; i++) {
            if (rng() % 30 == 0) v.push_back(i);
        }
        if (v.empty()) continue;
        st.insert(v);
    }
    cout << n << ' ' << m << '\n';
    for (vector<int> x : st) {
        cout << x.size() << '\n';
        for (int y : x) cout << y << ' ';
        cout << '\n';
    }
    return 0;
}