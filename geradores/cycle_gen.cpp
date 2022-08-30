#include <bits/stdc++.h>
using namespace std;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

bool dir[MAXN], esq[MAXN];
vector<vector<int>> paths;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        if (rng() % 2) {
            dir[i] = true;
        }
        else {
            esq[(i + 1) % n] = true;
        }
    }
    for (int i = 0; i < n; i++) {
        if (dir[i] && dir[(i + 1) % n]) {
            vector<int> aux;
            aux.push_back(i + 1);
            aux.push_back((i + 1) % n + 1);
            int at = (i + 1) % n, cnt = 0;
            while (dir[at] && cnt < n) {
                at = (at + 1) % n;
                aux.push_back(at + 1);
                paths.push_back(aux);
                cnt++;
            }
        }
    }
    for (int i = n; i > 2; i--) {
        if (esq[i] && esq[(i - 1 + n) % n]) {
            vector<int> aux;
            aux.push_back(i + 1);
            aux.push_back((i - 1 + n) % n + 1);
            int at = (i - 1 + n) % n, cnt = 0;
            while (esq[at] && cnt < n) {
                at = (at - 1 + n) % n;
                aux.push_back(at + 1);
                paths.push_back(aux);
                cnt++;
            }
        }
    }
    random_shuffle(paths.begin(), paths.end());
    int p;
    if (paths.empty()) p = 0;
    else p = rng() % (int)paths.size() + 1;
    cout << n << ' ' << n << ' ' << p << '\n';
    for (int i = 0; i < n; i++) {
        if (dir[i]) cout << i + 1 << ' ' << (i + 1) % n + 1 << '\n';
        else cout << (i + 1) % n + 1 << ' ' << i + 1 << '\n';
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