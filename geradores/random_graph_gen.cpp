#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e3 + 5;

vector<int> w[MAXN];

int main () { 
	freopen("in.txt", "w", stdout);
	int n;
	cin >> n;
	int m = 2 * n;
	vector<pii> v;
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++) {
			v.emplace_back(i, j);
		}
	}
	random_shuffle(v.begin(), v.end());
	for (int i = 0; i < m; i++) {
		if (rng() % 2)
			swap(v[i].first, v[i].second);			
		w[v[i].first].push_back(v[i].second);
	}
	int p = 2 * n;
	set<vector<int>> paths;
	while (paths.size() < p) {
		int start = rng() % n + 1;
		vector<int> vis(n + 1), path;
		vis[start] = 1;
		path.push_back(start);
		for (int i = 0; i < 10; i++) {
			random_shuffle(w[start].begin(), w[start].end());
			bool ok = false;
			for (int i = 0; i < w[start].size(); i++) {
				if (!vis[w[start][i]]) {
					vis[w[start][i]] = 1;
					start = w[start][i];
					path.push_back(start);
					ok = true;
					break;
				}
			}
			if (!ok) break;
		}
		if (path.size() > 2)
			paths.insert(path);
	}
	cout << n << ' ' << m << ' ' << p << '\n';
	for (int i = 0; i < m; i++) {
		cout << v[i].first << ' ' << v[i].second << '\n';
	}
	for (auto x : paths) {
		cout << x.size() << '\n';
		for (auto y : x) cout << y << ' ';
			cout << '\n';
	}
	return 0;
}