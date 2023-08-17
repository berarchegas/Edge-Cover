// Generates a random tree using Cayleys Formula
// And then adds random edges

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

int c[MAXN], d[MAXN];
vector<int> v[MAXN];
set<pii> st;

void dfs(int node, int p = 0) {
	for (int x : v[node]) {
        if (x != p) {
            st.insert({x, node});
            st.insert({node, x});
            dfs(x, node);
		}
    }
}

int main () { 
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n-2; i++) {
		d[i] = rng() % n + 1;
		c[d[i]]++;
	}
	priority_queue<int, vector<int>, greater<int>> fila;
	for (int i = 1; i <= n; i++) {
		if (!c[i]) fila.push(i);
	}
	for (int i = 0; i < n-2; i++) {
        v[fila.top()].push_back(d[i]);
        v[d[i]].push_back(fila.top());
		fila.pop();
		c[d[i]]--;
		if (!c[d[i]]) fila.push(d[i]);
	}
    int a = fila.top();
	fila.pop();
    int b = fila.top();
    fila.pop();
    v[a].push_back(b);
    v[b].push_back(a);
    dfs(1);
    for (int i = n; i <= m; i++) {
        while (true) {
            int a = rng() % n + 1;
            int b = rng() % n + 1;
            if (st.count({a, b})) continue;
            v[a].push_back(b);
            v[b].push_back(a);
            st.insert({a, b});
            st.insert({b, a});
            break;
        }
    }
	cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++) {
        for (int x : v[i]) if (x > i) cout << i << ' ' << x << '\n';
    }
    return 0;
}