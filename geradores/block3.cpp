// Generates instances where every element appears in 3 sets 
// and sets have size 3, but it is not 3dmr
// because they are not separated in X Y Z

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

// Idea is to build building blocks of size 3k with each node with one triplet
// Then we join 3 blocks so that each node is in 2 triplets
// Then we join 3 blocks of 3 so that each node is in 3 triplets

vector<array<int, 3>> triplets;

int main () { 
    int k;
    cin >> k;
    vector<int> val(27 * k);
    for (int &x : val) {
        x = rng();
    }
    for (int i = 0; i < 9; i++) {
        // each block has 3k elements
        vector<int> elements;
        for (int j = 3 * i * k + 1; j <= 3 * (i + 1) * k; j++) elements.push_back(j);
        sort(elements.begin(), elements.end(), [&] (int a, int b) {
            return val[a] < val[b];
        });
        for (int j = 0; j < k; j++) {
            triplets.push_back({elements[3 * j], elements[3 * j + 1], elements[3 * j + 2]});
        }
    }
    for (int &x : val) {
        x = rng();
    }
    for (int i = 0; i < 3; i++) {
        vector<int> elements[3];
        for (int j = 0; j < 3; j++) {
            for (int l = 9 * i * k + 1 + j * 3 * k; l <= 9 * i * k + (j + 1) * 3 * k; l++) elements[j].push_back(l);
            sort(elements[j].begin(), elements[j].end(), [&] (int a, int b) {
                return val[a] < val[b];
            });
        }
        for (int i = 0; i < elements[0].size(); i++) {
            triplets.push_back({elements[0][i], elements[1][i], elements[2][i]});
        }
    }
    for (int &x : val) {
        x = rng();
    }
    vector<int> elements[3];
    for (int j = 0; j < 3; j++) {
        for (int l = j * 9 * k + 1; l <= (j + 1) * 9 * k; l++) elements[j].push_back(l);
        sort(elements[j].begin(), elements[j].end(), [&] (int a, int b) {
            return val[a] < val[b];
        });
    }
    for (int i = 0; i < elements[0].size(); i++) {
        triplets.push_back({elements[0][i], elements[1][i], elements[2][i]});
    }
    cout << triplets.size() << '\n';
    for (auto x : triplets) cout << x[0] << ' ' << x[1] << ' ' << x[2] << '\n';
    return 0;
}