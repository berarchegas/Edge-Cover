#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

int c[MAXN], d[MAXN];
vector<int> v[MAXN], aux;
vector<vector<int>> paths;
vector<pii> edges;

void path_find(int node, int pai = 0) {
	aux.push_back(node);
	if (aux.size() > 2) paths.push_back(aux);
	for (int x : v[node]) {
		if (x != pai) {
			path_find(x, node);
		}
	}
	aux.pop_back();
}

void dfs(int node, int pai = 0) {
    for (int x : v[node]) {
        if (x != pai) {
			edges.emplace_back(node, x);
		}
    }
	for (int x : v[node]) {
        if (x != pai) {
			dfs(x, node);
		}
    }
	for (int x : v[node]) {
		if (x != pai) {
			aux.clear();
			path_find(x, node);
		}
	}
}

int main () { 
	int n;
	cin >> n;
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
	path_find(1);
	random_shuffle(paths.begin(), paths.end());
	int p;
	if (paths.empty()) p = 0;
	else p = rng() % (int)paths.size() + 1;
	cout << n << ' ' << n - 1 << ' ' << p << '\n';
	for (auto x : edges) cout << x.first << ' ' << x.second << '\n';
	for (int i = 0; i < p; i++) {
		aux = paths.back();
		paths.pop_back();
		cout << aux.size() << '\n';
		for (int x : aux) cout << x << ' ';
		cout << '\n';
	}
    return 0;
}