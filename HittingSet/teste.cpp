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
        ll msk = rng() % ((1ll << n) - 1) + 1;
        if (__builtin_popcountll(msk) > n/2 - 10) continue;
        vector<int> ans;
        for (int i = 0; i < n; i++) {
            if (msk & (1ll << i)) ans.push_back(i);
        }
        st.insert(ans);
    }
    cout << n << ' ' << m << '\n';
    for (vector<int> x : st) {
        cout << x.size() << '\n';
        for (int y : x) cout << y << ' ';
        cout << '\n';
    }
    return 0;
}