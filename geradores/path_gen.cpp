#include <bits/stdc++.h>
using namespace std;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e6 + 5;

bool dir[MAXN], esq[MAXN];
vector<vector<int>> paths;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 1; i < n; i++) {
        if (rng() % 2) {
            dir[i] = true;
        }
        else {
            esq[i + 1] = true;
        }
    }
    for (int i = 1; i < n - 1; i++) {
        if (dir[i] && dir[i + 1]) {
            vector<int> aux;
            aux.push_back(i);
            aux.push_back(i + 1);
            int at = i + 1;
            while (dir[at]) {
                aux.push_back(++at);
                paths.push_back(aux);
            }
        }
    }
    for (int i = n; i > 2; i--) {
        if (esq[i] && esq[i - 1]) {
            vector<int> aux;
            aux.push_back(i);
            aux.push_back(i - 1);
            int at = i - 1;
            while (esq[at]) {
                aux.push_back(--at);
                paths.push_back(aux);
            }
        }
    }
    random_shuffle(paths.begin(), paths.end());
    int p;
    if (paths.empty()) p = 0;
    else p = rng() % (int)paths.size() + 1;
    cout << n << ' ' << n - 1 << ' ' << p << '\n';
    for (int i = 1; i < n; i++) {
        if (dir[i]) cout << i << ' ' << i + 1 << '\n';
        else cout << i + 1 << ' ' << i << '\n';
    }
    for (int i = 0; i < p; i++) {
        vector<int> aux = paths.back();
        paths.pop_back();
        cout << aux.size() << '\n';
        for (int x : aux) cout << x << ' ';
        cout << '\n';
    }
    return 0;
}