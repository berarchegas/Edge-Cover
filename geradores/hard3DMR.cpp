// Generates instances of 3DMR that have only one hitting set

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

// Idea is to have the answer be {(0, 0, 0), (1, 1, 1)... (i, i, i)}
// The rest of the sets will be of the format (i, i, j) or (i, j, i) or (j, i, i)
// where i % 3 is 0 or 1 and j % 3 is 2
// In the end we will do a mapping of the numbers so that is seems random

vector<vector<int>> triplas;

// em qual posicao ta e qual posicao ta faltando
vector<int> simples[3], duplas[3];

int main () { 
    
    // n * 3 is the number of elements in each set
    int n;
    cin >> n;
    n *= 3;
    for (int i = 0; i < n; i++) {
        triplas.push_back({i, i, i});
        for (int j = 0; j < 3; j++) {
            if (i % 3 == 2) {
                simples[j].push_back(i);
                simples[j].push_back(i);
            }
            else {
                duplas[j].push_back(i);
            }
        }
    }
    
    vector<int> randomVal(n);
    for (int i = 0; i < 3; i++) {
        for (int &x : randomVal) x = rng();
        sort(simples[i].begin(), simples[i].end(), [&] (int a, int b) {
            return randomVal[a] < randomVal[b];
        });
        for (int &x : randomVal) x = rng();
        sort(duplas[i].begin(), duplas[i].end(), [&] (int a, int b) {
            return randomVal[a] < randomVal[b];
        });
    }

    for (int i = 0; i < 2 * n / 3; i++) {
        triplas.push_back({simples[0][i], duplas[0][i], duplas[0][i]});
    }
    for (int i = 0; i < 2 * n / 3; i++) {
        triplas.push_back({duplas[1][i], simples[1][i], duplas[1][i]});
    }
    for (int i = 0; i < 2 * n / 3; i++) {
        triplas.push_back({duplas[2][i], duplas[2][i], simples[2][i]});
    }

    // fazer o mapeamento
    for (int i = 0; i < 3; i++) {
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