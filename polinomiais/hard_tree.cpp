#include <bits/stdc++.h>
using namespace std;
const int MAXN = 5e3 + 5;

template<typename T = int> struct hungarian {
	int n;
	vector<vector<T>> a;
	vector<T> u, v;
	vector<int> p, way;
	T inf;

	hungarian(int n_) : n(n_), u(n+1), v(n+1), p(n+1), way(n+1) {
		a = vector<vector<T>>(n, vector<T>(n));
		inf = numeric_limits<T>::max();
	}
	T assignment() {
		for (int i = 1; i <= n; i++) {
			p[0] = i;
			int j0 = 0;
			vector<T> minv(n+1, inf);
			vector<int> used(n+1, 0);
			do {
				used[j0] = true;
				int i0 = p[j0], j1 = -1;
				T delta = inf;
				for (int j = 1; j <= n; j++) if (!used[j]) {
					T cur = a[i0-1][j-1] - u[i0] - v[j];
					if (cur < minv[j]) minv[j] = cur, way[j] = j0;
					if (minv[j] < delta) delta = minv[j], j1 = j;
				}
				for (int j = 0; j <= n; j++)
					if (used[j]) u[p[j]] += delta, v[j] -= delta;
					else minv[j] -= delta;
				j0 = j1;
			} while (p[j0] != 0);
			do {
				int j1 = way[j0];
				p[j0] = p[j1];
				j0 = j1;
			} while (j0);
		}
		return -v[0];
	}
};

// g[i][j] eh o custo de consertar a subarvore do pai de i apenas com os filhos i e j
int g[MAXN][MAXN];

// dp0[i] eh o custo de consertar a subarvore de i
int dp0[MAXN];

// dp1[i] é o custo de consertar a subarvore do pai de i sem o filho i
int dp1[MAXN];

// profundidade de cada vertice
int dep[MAXN];

// up[i] sao os filhos que sobem pra i e down[i] os filhos que descem de i
vector<int> up[MAXN], down[MAXN];

// lista de adjacencia nao direcionada
vector<pair<int, int>> v[MAXN];

// sobe[i] sao os caminhos que sobem ate o pai de i
// desce[i] sao os caminhos que descem do pai de i
// sobedesce[i][j] sao os caminhos que vem da subarvore de i
// passam pelo pai de i e descem pela subarvore de j
vector<vector<int>> sobe[MAXN], desce[MAXN], sobedesce[MAXN][MAXN];

void dfs(int node, int pai = 0) {
    // calcula a profundidade de cada vertice
    // monta a lista de adjacencia direcionada

    dep[node] = 1 + dep[pai];
    for (auto x : v[node]) {
        if (x.first != pai) {
            if (!x.second) down[node].push_back(x.first);
            else up[node].push_back(x.first);
            dfs(x.first, node);
        }
    }
}

void calcDP(int node, int pai = 0) {
    for (auto x : v[node]) {
        if (x.first != pai) calcDP(x.first, node);
    }

    // calcula g[x][0]
    for (int x : up[node]) {
        g[x][0] = 1 + dp0[x];
        for (auto y : sobe[x]) {
            int tenta = dp0[y[0]];
            for (int i = 0; i < y.size() - 2; i++) {
                tenta += dp1[y[i]];
            }
            g[x][0] = min(g[x][0], tenta + 1);
        }
    }

    // calcula g[0][x]
    for (int x : down[node]) {
        g[0][x] = 1 + dp0[x];
        for (auto y : desce[x]) {
            int tenta = dp0[y.back()];
            for (int i = y.size() - 1; i >= 2; i--) {
                tenta += dp1[y[i]];
            }
            g[0][x] = min(g[0][x], tenta + 1);
        }
    }

    // calcula g[x][y]
    for (int x : up[node]) {
        for (int y : down[node]) {
            g[x][y] = g[x][0] + g[0][y];
            for (auto z : sobedesce[x][y]) {
                int pos;
                for (int i = 0; i < z.size(); i++) {
                    if (z[i] == node) {
                        pos = i;
                        break;
                    }
                }
                int tenta = dp0[z[0]];
                for (int i = 0; i < pos - 1; i++) {
                    tenta += dp1[z[i]];
                }
                for (int i = pos + 2; i < z.size(); i++) {
                    tenta += dp1[z[i]];
                }
                tenta += dp0[z.back()];
                g[x][y] = min(g[x][y], tenta + 1);
            }
        }
    }

    // calcula dp0[node]
    while (up[node].size() < down[node].size()) up[node].push_back(0);
    while (down[node].size() < up[node].size()) down[node].push_back(0);
    int n = up[node].size();

    // depois podemos trocar o hungaro por outro algoritmo de matching
    // basta usar a matriz h.a que montamos como os pesos das arestas do matching
    // ai podemos usar qualquer algoritmo blackbox
    hungarian h(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            h.a[i][j] = g[up[node][i]][down[node][j]];
        }
    }
    dp0[node] = h.assignment();

    // calcula dp1[filho]
    for (int i = 0; i < n; i++) {
        h.p.clear();
        h.u.clear();
        h.v.clear();
        h.way.clear();
        h.p.resize(n + 1);
        h.u.resize(n + 1);
        h.v.resize(n + 1);
        h.way.resize(n + 1);
        for (int j = 0; j < n; j++) {
            h.a[i][j] = g[0][down[node][j]];
        }
        dp1[up[node][i]] = h.assignment();
        for (int j = 0; j < n; j++) {
            h.a[i][j] = g[up[node][i]][down[node][j]];
        }
    }
    for (int j = 0; j < n; j++) {
        h.p.clear();
        h.u.clear();
        h.v.clear();
        h.way.clear();
        h.p.resize(n + 1);
        h.u.resize(n + 1);
        h.v.resize(n + 1);
        h.way.resize(n + 1);
        for (int i = 0; i < n; i++) {
            h.a[i][j] = g[up[node][i]][0];
        }
        dp1[down[node][j]] = h.assignment();
        for (int i = 0; i < n; i++) {
            h.a[i][j] = g[up[node][i]][down[node][j]];
        }
    }
}

// timer T; T() -> retorna o tempo em ms desde que declarou
using namespace chrono;
struct timer : high_resolution_clock {
	const time_point start;
	timer(): start(now()) {}
	int operator()() {
		return duration_cast<milliseconds>(now() - start).count();
	}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    // timer T;
    int n, m, p, a, b;
    cin >> n >> m >> p;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        v[a].emplace_back(b, 0);
        v[b].emplace_back(a, 1);
    }
    dfs(1);

    for (int i = 0; i < p; i++) {
        int tam;
        cin >> tam;        
        vector<int> path(tam);
        bool cima = false, baixo = false;
        for (int j = 0; j < tam; j++) {
            cin >> path[j];
            if (j && dep[path[j - 1]] > dep[path[j]]) cima = true;
            if (j && dep[path[j - 1]] < dep[path[j]]) baixo = true;
        }
        if (cima && baixo) {

            // sobedesce
            for (int j = 0; j < tam - 2; j++) {
                if (dep[path[j]] > dep[path[j + 1]] && dep[path[j + 1]] < dep[path[j + 2]]) {
                    sobedesce[path[j]][path[j + 2]].push_back(path);
                }
            }
        }
        else if (cima) {

            // sobe
            sobe[path[tam - 2]].push_back(path);
        }
        else if (baixo) {
            
            // desce
            desce[path[1]].push_back(path);
        }
    }

    calcDP(1);
    
    cout << dp0[1] << '\n';
    // cout << T() << '\n';
    return 0;
}