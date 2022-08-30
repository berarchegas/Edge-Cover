#include <bits/stdc++.h>
using namespace std;
const int MAXN = 2e5 + 5;

int dir[MAXN], esq[MAXN], ansd[MAXN], anse[MAXN];
vector<int> pathd[MAXN], pathe[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, p, a, b;
    cin >> n >> m >> p;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        if (a > b) esq[a] = b;
        else dir[a] = b;
    }
    for (int i = 0; i < p; i++) {
        int tam, ini, fim;
        cin >> tam;
        cin >> ini;
        for (int j = 1; j < tam - 1; j++) {
            cin >> fim;
        }
        cin >> fim;
        if (ini > fim) pathe[ini].push_back(fim);
        else pathd[ini].push_back(fim);
    }
    for (int i = n; i >= 1; i--) {
        if (!dir[i]) ansd[i] = ansd[i + 1];
        else ansd[i] = 1 + ansd[i + 1];
        for (int x : pathd[i]) ansd[i] = min(ansd[i], 1 + ansd[x]);
    }
    for (int i = 1; i <= n; i++) {
        if (!esq[i]) anse[i] = anse[i - 1];
        else anse[i] = 1 + anse[i - 1];
        for (int x : pathe[i]) anse[i] = min(anse[i], 1 + anse[x]);
    }
    cout << ansd[1] + anse[n] << '\n';
    return 0;
}