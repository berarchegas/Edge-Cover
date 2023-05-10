#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using ll = long long;

int main() {
    freopen("in.txt", "r", stdin);
    int n, m;
    cin >> n >> m;

    vector<ll> masks;

    for (int i = 0; i < m; i++) {
        int tam, x;
        cin >> tam;
        ll mask = 0;
        for (int j = 0; j < tam; j++) {
            cin >> x;
            mask |= (1ll << x);
        }
        masks.push_back(mask);
    }

    int ans = n + 1, maskAnswer = -1;
    for (ll i = 0; i < (1ll << n); i++) {
        if (__builtin_popcountll(i) >= ans) continue;
        bool ok = true;
        for (ll x : masks) ok &= !!(x & i);
        if (ok) {
            ans = __builtin_popcountll(i);
            maskAnswer = i;
        }
    }

    cout << ans << '\n';

    for (int i = 0; i < n; i++) {
        if (maskAnswer & (1ll << i)) cout << i << ' ';
    }
    cout << '\n';

    return 0;
}