// Generates a random graph using the Erdos-Renyi method

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e5 + 5;

vector<int> v[MAXN];

int main () { 
    freopen("in.txt", "w", stdout);
    // input p * 10^6
	int n, p, m = 0;
	cin >> n >> p;
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            int a = rng() % 1000001;
            if (a < p) {
                v[i].push_back(j);
                v[j].push_back(i);
                m++;
            }
        }
    }
    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++) {
        for (int x : v[i]) if (x > i) cout << i - 1 << ' ' << x - 1 << '\n';
    }
    return 0;
}