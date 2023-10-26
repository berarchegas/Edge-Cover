#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int INF = 1e9;
using pii = pair<int, int>;

// we can optimize here
// only the head nodes need the fields left, right and len
struct Node {
	int left, right, up, down, item, len, option;
} table[2][MAXN];

int pos[MAXN];

void hideLine(bool t, int p) {
    // hide line p

    int q = p + 1;
    while (q != p) {
        if (table[t][q].item <= 0) {
            q = table[t][q].up;
        }
        else {
            table[t][table[t][q].up].down = table[t][q].down;
            table[t][table[t][q].down].up = table[t][q].up;
            table[t][table[t][q].item].len--;
            q++;
        }
    }
}

void unhideLine(bool t, int p) {
    // unhide line p

    int q = p - 1;
    while (q != p) {
        if (table[t][q].item <= 0) {
            q = table[t][q].down;
        }
        else {
            table[t][table[t][q].up].down = q;
            table[t][table[t][q].down].up = q;
            table[t][table[t][q].item].len++;
            q--;
        }
    }
}

void coverColumn(int i) {
    // cover column i

    hideLine(1, pos[i]);

    int p = table[0][i].down;
    while (p != i) {
        hideLine(0, p);
        p = table[0][p].down;
    }
    table[0][table[0][i].left].right = table[0][i].right;
    table[0][table[0][i].right].left = table[0][i].left;
}

void uncoverColumn(int i) {
    // uncover column i

    table[0][table[0][i].left].right = i;
    table[0][table[0][i].right].left = i;
    int p = table[0][i].up;
    while (p != i) {
        unhideLine(0, p);
        p = table[0][p].up;
    }

    unhideLine(1, pos[i]);
}

vector<vector<int>> options;
int tail[2][MAXN], vis[MAXN], ed[500][500], items;
pii endpoint[MAXN];

// passa por todos os vertices da componente e devolve a menor aresta
pii dfs(int node, stack<int> &reset) {
    reset.push(node);
    vis[node] = 1;
    pii ans = {INF, 0};
    for (int aux = table[1][node].down; aux != node; aux = table[1][aux].down) {
        int edge = table[1][aux].option;
        ans = min(ans, {table[0][edge].len, edge});
        if (!vis[endpoint[edge].first]) {
            ans = min(ans, dfs(endpoint[edge].first, reset));
        }
        if (!vis[endpoint[edge].second]) {
            ans = min(ans, dfs(endpoint[edge].second, reset));
        }
    }
    return ans;
}

int search(int rep) {

    // se o representante nao tem mais nenhuma aresta
    if (table[1][rep].down == rep) {
        return 0;
    }

    int ans = 1;
    stack<int> reset;
    int edge = dfs(rep, reset).second;
    
    while (!reset.empty()) {
        vis[reset.top()] = 0;
        reset.pop();
    } 

	coverColumn(edge);

    for (int aux = table[0][edge].down; aux != edge; aux = table[0][aux].down) {
        for (int node = aux + 1; node != aux;) {
            if (table[0][node].item <= 0) {
                node = table[0][node].up;
            }
            else {
                coverColumn(table[0][node].item);
                node++;
            }
        }
        for (int node = aux + 1; node != aux;) {
            if (table[0][node].item <= 0) {
                node = table[0][node].up;
            }
            else {
                int a = endpoint[table[0][node].item].first;
                int b = endpoint[table[0][node].item].second;
                if (!vis[a]) {
                    int tenta = min(search(a), INF);
                    ans = min(ans + tenta, INF);
                    dfs(a, reset);
                }
                if (!vis[b]) {
                    int tenta = min(search(b), INF);
                    ans = min(ans + tenta, INF);
                    dfs(b, reset);
                }
                node++;
            }
        }

        while (!reset.empty()) {
            vis[reset.top()] = 0;
            reset.pop();
        }

        for (int node = aux - 1; node != aux;) {
            if (table[0][node].item <= 0) {
                node = table[0][node].down;
            }
            else {
                uncoverColumn(table[0][node].item);
                node--;
            }
        }
    }

	uncoverColumn(edge);
    return ans;
}

int main() {

    int n, m, p;
    cin >> n >> m >> p;

    int a, b, cnt = 1;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        if (a > b) swap(a, b);
        endpoint[cnt] = {a, b};
        ed[a][b] = ed[b][a] = cnt++;
    }
    items = cnt - 1;
    for (int i = 0; i < p; i++) {
        int tam;
        cin >> tam;
        vector<int> option(tam - 1);
        int ini, fim; 
        cin >> ini;
        for (int j = 0; j < tam - 1; j++) {
            cin >> fim;
            option[j] = ed[ini][fim];
            ini = fim;
        }
        sort(option.begin(), option.end());
        options.push_back(option);
    }

    for (int i = 0; i <= items; i++) {
        table[0][i].down = table[0][i].up = table[0][i].item = i;
        table[0][i].len = 0;
        table[0][i].left = (i + items) % (items + 1);
        table[0][i].right = (i + 1) % (items + 1);
        tail[0][i] = i;
    }
    int at = items + 1, last = 0;
    for (int i = 0; i < (int)options.size(); i++) {
        // add a spacer node
        table[0][at].item = -i;
        table[0][at].up = last;
        table[0][at].down = at + options[i].size();
        last = at + 1;
        at++;
        for (int j = 0; j < (int)options[i].size(); j++) {
            table[0][at].item = options[i][j];
            table[0][options[i][j]].len++;
            table[0][at].down = options[i][j];
            table[0][at].up = tail[0][options[i][j]];
            table[0][options[i][j]].up = at;
            table[0][tail[0][options[i][j]]].down = at;
            tail[0][options[i][j]] = at;
            table[0][at].option = i;
            at++;
        }
    }
    // last spacer node
    table[0][at].item = - (int)options.size();
    table[0][at].up = last;
    table[0][at].down = 0;

    items = n;
    for (int i = 0; i <= items; i++) {
        table[1][i].down = table[1][i].up = table[1][i].item = i;
        table[1][i].len = 2;
        table[1][i].left = (i + items) % (items + 1);
        table[1][i].right = (i + 1) % (items + 1);
        tail[1][i] = i;
    }
    at = items + 1, last = 0;
    for (int i = 1; i <= m; i++) {
        // add a spacer node
        table[1][at].item = -i + 1;
        table[1][at].up = last;
        table[1][at].down = at + 2;
        last = at + 1;
        at++;

        pos[i] = at;
        table[1][at].item = endpoint[i].first;
        table[1][at].down = endpoint[i].first;
        table[1][at].up = tail[1][endpoint[i].first];
        table[1][endpoint[i].first].up = at;
        table[1][tail[1][endpoint[i].first]].down = at;
        tail[1][endpoint[i].first] = at;
        table[1][at].option = i;
        at++;

        table[1][at].item = endpoint[i].second;
        table[1][at].down = endpoint[i].second;
        table[1][at].up = tail[1][endpoint[i].second];
        table[1][endpoint[i].second].up = at;
        table[1][tail[1][endpoint[i].second]].down = at;
        tail[1][endpoint[i].second] = at;
        table[1][at].option = i;
        at++;
    }
    // last spacer node
    table[1][at].item = -m;
    table[1][at].up = last;
    table[1][at].down = 0;

    cout << search(1) << '\n';
}