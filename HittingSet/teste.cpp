#include <bits/stdc++.h>
#include "setTrie.h"
using namespace std;
using pii = pair<int, int>;

int main() {
    int n, m;
    cin >> n >> m;
    SetTrie ber;
    for (int i = 0; i < n; i++) {
        vector<bool> oda(m);
        int tam, x;
        cin >> tam;
        for (int j = 0; j < tam; j++) {
            cin >> x;
            oda[x] = true;
        }
        cout << ber.subsetSearch(oda) << ' ' << ber.supersetSearch(oda) << endl;
        ber.insert(oda);
    }
    return 0;
}