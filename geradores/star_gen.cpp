#include <bits/stdc++.h>
using namespace std;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    freopen("in.txt", "w", stdout);
    int n;
    cin >> n;
    int p = 2 * n;
    int m = n - 1;
    cout << n << ' ' << m << ' ' << p << '\n';
    for (int i = 1; i <= n / 2; i++) {
        cout << i << ' ' << n << '\n';
    }    
    for (int i = n / 2 + 1; i < n; i++) {
        cout << n << ' ' << i << '\n';
    }
    for (int i = 0; i < p; i++) {
        int a = rng() % (n / 2) + 1;
        int b = rng() % (n - n / 2 - 1) + n / 2 + 1;
        cout << "3\n" << a << ' ' << n << ' ' << b << '\n';
    }
    return 0;
}