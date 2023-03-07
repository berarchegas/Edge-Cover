// BFS no Branch and Bound
// Checar se estamos em um caso base (floresta)
// Se sim, resolvemos com os algoritmos polinomiais
// Se nao, calculamos o lowerBound baseado no caminho euleriano
// Entao pegamos todos os caminhos disponiveis
// Tiramos as arestas desse caminho e verificamos se o grafo resultante ja foi visitado
// Baseado nisso marcamos o vis e colocamos na queue, continuando a BFS

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());
const int INF = 1e9;
const int MAXN = 1e3 + 5;

int upperBound = 1e6 + 5;
ll hasher[MAXN][MAXN];
map<ll, bool> vis;
vector<vector<int>> paths, paths_1;

struct graph {
    int hashing, n, cost;
    vector<vector<bool>> adjacency;
    set<int> availablePaths;
    graph(int _n = 0) {
        n = _n, cost = 0, hashing = 0;
        adjacency = vector<vector<bool>> (n, vector<bool> (n));
        availablePaths = set<int> ();
    }
};

// NAO ESTAMOS USANDO O DINIC
// Dinic
//
// O(min(m * max_flow, n^2 m))
// Grafo com capacidades 1 -> O(sqrt(n)*m)
// INF ta pra int
// struct dinic {
// 	const bool scaling = false;
// 	int lim;
// 	struct edge {
// 		int to, cap, rev, flow;
// 		bool res;
// 		edge(int to_, int cap_, int rev_, bool res_)
// 			: to(to_), cap(cap_), rev(rev_), flow(0), res(res_) {}
// 	};

// 	vector<vector<edge>> g;
// 	vector<int> lev, beg;
// 	ll F;
// 	dinic(int n) : g(n), F(0) {}

// 	void add(int a, int b, int c) {
// 		g[a].emplace_back(b, c, g[b].size(), false);
// 		g[b].emplace_back(a, 0, g[a].size()-1, true);
// 	}
// 	bool bfs(int s, int t) {
// 		lev = vector<int>(g.size(), -1); lev[s] = 0;
// 		beg = vector<int>(g.size(), 0);
// 		queue<int> q; q.push(s);
// 		while (q.size()) {
// 			int u = q.front(); q.pop();
// 			for (auto& i : g[u]) {
// 				if (lev[i.to] != -1 or (i.flow == i.cap)) continue;
// 				if (scaling and i.cap - i.flow < lim) continue;
// 				lev[i.to] = lev[u] + 1;
// 				q.push(i.to);
// 			}
// 		}
// 		return lev[t] != -1;
// 	}
// 	int dfs(int v, int s, int f = INF) {
// 		if (!f or v == s) return f;
// 		for (int& i = beg[v]; i < g[v].size(); i++) {
// 			auto& e = g[v][i];
// 			if (lev[e.to] != lev[v] + 1) continue;
// 			int foi = dfs(e.to, s, min(f, e.cap - e.flow));
// 			if (!foi) continue;
// 			e.flow += foi, g[e.to][e.rev].flow -= foi;
// 			return foi;
// 		}
// 		return 0;
// 	}
// 	ll max_flow(int s, int t) {
// 		for (lim = scaling ? (1<<30) : 1; lim; lim /= 2)
// 			while (bfs(s, t)) while (int ff = dfs(s, t)) F += ff;
// 		return F;
// 	}
// };

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

struct tree {
    // vetor de visitados para o caso de mais de uma componente conexa
    vector<int> vis;

    // g[i][j] eh o custo de consertar a subarvore do pai de i apenas com os filhos i e j
    vector<vector<int>> g;

    // dp0[i] eh o custo de consertar a subarvore de i
    vector<int> dp0;

    // dp1[i] é o custo de consertar a subarvore do pai de i sem o filho i
    vector<int> dp1;

    // profundidade de cada vertice
    vector<int> dep;

    // up[i] sao os filhos que sobem pra i e down[i] os filhos que descem de i
    vector<vector<int>> up, down;

    // lista de adjacencia nao direcionada
    vector<vector<pair<int, int>>> v;

    // sobe[i] sao os caminhos que sobem ate o pai de i
    // desce[i] sao os caminhos que descem do pai de i
    // sobedesce[i][j] sao os caminhos que vem da subarvore de i
    // passam pelo pai de i e descem pela subarvore de j
    vector<vector<vector<int>>> sobe, desce;
    vector<vector<vector<vector<int>>>> sobedesce;

    graph grafo;

    tree(graph _grafo) {
        grafo = _grafo;
        grafo.n++;
        int n = grafo.n;
        g = vector<vector<int>> (n, vector<int> (n));
        up = vector<vector<int>> (n);
        down = vector<vector<int>> (n);
        dp0 = vector<int> (n);
        dp1 = vector<int> (n);
        dep = vector<int> (n);
        vis = vector<int> (n);
        v = vector<vector<pair<int, int>>> (n);
        sobe = vector<vector<vector<int>>> (n);
        desce = vector<vector<vector<int>>> (n);
        sobedesce = vector<vector<vector<vector<int>>>> (n, vector<vector<vector<int>>> (n));
    }

    int solve() {
        int n = grafo.n;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1; j++) {
                if (grafo.adjacency[i][j]) {
                    v[i + 1].emplace_back(j + 1, 0);
                    v[j + 1].emplace_back(i + 1, 1);
                }
            }
        }
        for (int i = 1; i < n; i++) {
            if (!vis[i]) dfs(i);
        }
        for (int x : grafo.availablePaths) {
            int tam = paths_1[x].size();
            bool cima = false, baixo = false;
            for (int j = 1; j < tam; j++) {
                if (dep[paths_1[x][j - 1]] > dep[paths_1[x][j]]) cima = true;
                if (dep[paths_1[x][j - 1]] < dep[paths_1[x][j]]) baixo = true;
            }
            if (cima && baixo) {

                // sobedesce
                for (int j = 0; j < tam - 2; j++) {
                    if (dep[paths_1[x][j]] > dep[paths_1[x][j + 1]] && 
                        dep[paths_1[x][j + 1]] < dep[paths_1[x][j + 2]]) {
                            sobedesce[paths_1[x][j]][paths_1[x][j + 2]].push_back(paths_1[x]);
                    }
                }
            }
            else if (cima) {

                // sobe
                sobe[paths_1[x][tam - 2]].push_back(paths_1[x]);
            }
            else if (baixo) {
                
                // desce
                desce[paths_1[x][1]].push_back(paths_1[x]);
            }
        }
        vis = vector<int> (n);
        int ans = 0;
        for (int i = 1; i < n; i++) {
            if (!vis[i]) {
                calcDP(i);
                ans += dp0[i];
            }
        }
        return ans;
    }

    void dfs(int node, int pai = 0) {
        // calcula a profundidade de cada vertice
        // monta a lista de adjacencia direcionada

        vis[node] = 1;
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
        vis[node] = 1;
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
};

int lowerBoundEuler(graph &g) {
    vector<int> vis(g.n);
    int ans = 0;
    for (int i = 0; i < g.n; i++) {
        if (!vis[i]) {
            int qtd = 0;
            vis[i] = 1;
            queue<int> fila;
            fila.push(i);
            while (!fila.empty()) {
                int at = fila.front();
                fila.pop();
                int dif = 0;
                for (int j = 0; j < g.n; j++) {
                    if (g.adjacency[at][j]) {
                        dif++;
                        if (!vis[j]) {
                            vis[j] = 1;
                            fila.push(j);
                        }
                    }
                    else if (g.adjacency[j][at]) {
                        dif--;
                        if (!vis[j]) {
                            vis[j] = 1;
                            fila.push(j);
                        }
                    }
                }
                qtd += max(0, dif);
            }
            ans += max(1, qtd);
        }
    }
    return ans;
}

// NAO ESTAMOS USANDO O LOWERBOUNDFLOW
// int lowerBoundFlow(graph &g) {
//     int at = 0;
//     map<pair<int, int>, int> edgesId;
//     for (int i = 0; i < g.n; i++) {
//         for (int j = 0; j < g.n; j++) {
//             if (g.adjacency[i][j]) edgesId[{i, j}] = at++;
//         }
//     }
//     dinic matching(2 * at + 2);
//     for (int i = 0; i < g.n; i++) {
//         for (int j = 0; j < g.n; j++) {
//             for (int k = 0; k < g.n; k++) {
//                 if (g.adjacency[i][j] && g.adjacency[j][k]) {
//                     matching.add(edgesId[{i, j}], at + edgesId[{j, k}], 1);
//                 }
//             }
//         }
//     }
//     int source = 2 * at, sink = 2 * at + 1;
//     for (int i = 0; i < at; i++) matching.add(source, i, 1);
//     for (int i = at; i < 2 * at; i++) matching.add(i, sink, 1);
//     return at - matching.max_flow(source, sink);
// }

// NAO ESTAMOS USANDO O getHash
// ll getHash(graph &g) {
//     ll ans = 0;
//     for (int i = 0; i < g.n; i++) {
//         for (int j = 0; j < g.n; j++) {
//             if (g.adjacency[i][j]) ans ^= hasher[i][j];
//         }
//     }
//     return ans;
// }

bool findTree(graph &g, vector<int> &visited, int root) {
    // retorna se a componente a qual root pertence eh uma arvore
    queue<int> fila;
    fila.push(root);
    visited[root] = 1;
    int nodes = 0, edges = 0;
    while (!fila.empty()) {
        int at = fila.front();
        fila.pop();
        nodes++;
        for (int i = 0; i < g.n; i++) {
            if ((g.adjacency[at][i] || g.adjacency[i][at]) && !visited[i]) {
                visited[i] = 1;
                fila.push(i);
            }
            if ((g.adjacency[at][i] || g.adjacency[i][at]) && at < i) edges++;
        }
    }
    return nodes == edges + 1;
}

bool baseCase(graph &g) {
    // verificar se toda componente conexa do grafo subjacente eh uma arvore
    vector<int> visited(g.n);
    bool tree = true;
    for (int i = 0; i < g.n && tree; i++) {
        if (!visited[i]) {
            tree &= findTree(g, visited, i);
        }
    }
    return tree;
}

void bfs(graph &g) {
    queue<graph> fila;
    fila.push(g);
    vis[g.hashing] = 1;
    int cnt = 1;
    while (!fila.empty()) {
        graph at = fila.front();
        fila.pop();
        if (baseCase(at)) {
            tree floresta(at);
            upperBound = min(upperBound, at.cost + floresta.solve());
        }
        else if (lowerBoundEuler(at) < upperBound) {
            for (int x : at.availablePaths) {
                graph nat = at;
                nat.cost++;
                for (int i = 0; i < paths[x].size() - 1; i++) {
                    nat.adjacency[paths[x][i]][paths[x][i + 1]] = false;
                    nat.hashing ^= hasher[paths[x][i]][paths[x][i + 1]];
                }
                for (int y : at.availablePaths) {
                    for (int i = 0; i < paths[y].size() - 1; i++) {
                        if (!nat.adjacency[paths[y][i]][paths[y][i + 1]]) {
                            nat.availablePaths.erase(y);
                            break;
                        }
                    }
                }
                if (!vis[nat.hashing]) {
                    vis[nat.hashing] = 1;
                    fila.push(nat);
                }
            }
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
    timer T;
    int n, m, p, a, b;
    cin >> n >> m >> p;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            hasher[i][j] = rng();
        }
    }
    graph g(n);
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        a--, b--;
        g.adjacency[a][b] = true;
        g.hashing ^= hasher[a][b];
        vector<int> path = {a, b};
        paths.push_back(path);
        path[0]++, path[1]++;
        paths_1.push_back(path);
        g.availablePaths.insert(i);
    }
    for (int i = 0; i < p; i++) {
        int tam;
        cin >> tam;
        vector<int> path(tam);
        for (int &x : path) {
            cin >> x;
            x--;
        }
        paths.push_back(path);
        for (int &x : path) {
            x++;
        }
        paths_1.push_back(path);
        g.availablePaths.insert(i + m);
    }
    stable_sort(paths.begin(), paths.end(), [&] (vector<int> a, vector<int> b) {
        return a.size() > b.size();
    });
    stable_sort(paths_1.begin(), paths_1.end(), [&] (vector<int> a, vector<int> b) {
        return a.size() > b.size();
    });
    bfs(g);
    cout << upperBound << ' ' << T() << '\n';
    return 0;
}