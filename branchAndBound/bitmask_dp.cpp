// Solucao com bitmask para comparar com o branch and bound
// Estamos tratando as arestas como nao direcionadas (nao pode ter aresta a->b e b->a)

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());
const int INF = 1e9;
const int MAXN = 1e8 + 5;

map<pii, int> mp;
vector<int> masks;
int dp[MAXN];

// timer T; T() -> retorna o tempo em ms desde que declarou
using namespace chrono;
struct timer : high_resolution_clock {
    const time_point start;
    timer(): start(now()) {}
    int operator()() {
        return duration_cast<milliseconds>(now() - start).count();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    timer T;
    int n, m, p, a, b, tam;
    cin >> n >> m >> p;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        mp[{a, b}] = mp[{b, a}] = i;
        masks.push_back(1 << i);
    }
    for (int i = 0; i < p; i++) {
        cin >> tam >> a;
        int msk = 0;
        for (int j = 1; j < tam; j++) {
            cin >> b;
            msk += (1 << mp[{a, b}]);
            a = b;
        }
        masks.push_back(msk);
    }
    for (int i = 0; i < (1 << m); i++) {
        dp[i] = MAXN;
    }
    dp[0] = 0;
    for (int i = 0; i < (1 << m); i++) {
        if (dp[i] == MAXN) continue;
        for (int x : masks) {
            if ((i & x) == 0) {
                dp[i | x] = min(dp[i | x], dp[i] + 1);
            }
        }
    }
    cout << dp[(1 << m) - 1] << ' ' << T() << '\n';
    return 0;
}