// Generates a random instance of 3DMR

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

vector<vector<int>> triplas;

vector<int> bro[3];

int main () { 
    
    // n * 3 is the number of elements in each set
    int n;
    cin >> n;
    n *= 3;
    for (int i = 0; i < n; i++) {
        triplas.push_back({i, i, i});
        for (int j = 0; j < 3; j++) {
            bro[j].push_back(i);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        vector<int> randomVal(n);
        for (int &x : randomVal) x = rng();
        sort(bro[i].begin(), bro[i].end(), [&] (int a, int b) {
            return randomVal[a] < randomVal[b];
        });
    }

    for (int i = 0; i < n; i++) {
        triplas.push_back({bro[0][i], bro[1][i], bro[2][i]});
    }

    for (int i = 0; i < 3; i++) {
        vector<int> randomVal(n);
        for (int &x : randomVal) x = rng();
        sort(bro[i].begin(), bro[i].end(), [&] (int a, int b) {
            return randomVal[a] < randomVal[b];
        });
    }

    for (int i = 0; i < n; i++) {
        triplas.push_back({bro[0][i], bro[1][i], bro[2][i]});
    }

    // fazer o mapeamento
    for (int i = 0; i < 3; i++) {
        vector<int> randomVal(n);
        for (int &x : randomVal) x = rng();
        vector<int> mapping(n);
        iota(mapping.begin(), mapping.end(), 0);
        sort(mapping.begin(), mapping.end(), [&] (int a, int b) {
            return randomVal[a] < randomVal[b];
        });
        for (auto &x : triplas) {
            x[i] = mapping[x[i]];
        }
    }

    random_shuffle(triplas.begin(), triplas.end());
    cout << n << '\n';
    for (auto x : triplas) {
        cout << x[0] << ' ' << x[1] + n << ' ' << x[2] + 2 * n << '\n';
    }
    return 0;
}