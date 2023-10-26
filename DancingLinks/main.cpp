#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;

// we can optimize here
// only the head nodes need the fields left, right and len
// hiding a line two times ?
struct Node {
	int left, right, up, down, item, len, option;
} table[MAXN];

void hideOption(int p) {
    // hide option p

    int q = p + 1;
    while (q != p) {
        if (table[q].item <= 0) {
            q = table[q].up;
        }
        else {
            table[table[q].up].down = table[q].down;
            table[table[q].down].up = table[q].up;
            table[table[q].item].len--;
            q++;
        }
    }
}

void unhideOption(int p) {
    // unhide option p

    int q = p - 1;
    while (q != p) {
        if (table[q].item <= 0) {
            q = table[q].down;
        }
        else {
            table[table[q].up].down = q;
            table[table[q].down].up = q;
            table[table[q].item].len++;
            q--;
        }
    }
}

void coverItem(int i) {
    // cover item i

    int p = table[i].down;
    while (p != i) {
        hideOption(p);
        p = table[p].down;
    }
    table[table[i].left].right = table[i].right;
    table[table[i].right].left = table[i].left;
}

void uncoverItem(int i) {
    // uncover item i

    table[table[i].left].right = i;
    table[table[i].right].left = i;
    int p = table[i].up;
    while (p != i) {
        unhideOption(p);
        p = table[p].up;
    }
}

vector<vector<int>> options;
int tail[MAXN], solution[MAXN], items;

void search(int k) {

    if (table[0].right == 0) {
        for (int i = 0; i < k; i++) {
            int id = table[solution[i]].option;
            for (int x : options[id]) {
                cout << x << ' ';
            }
            cout << ",\n"[i == k - 1];
        }
        return;
    }
    

	// Choose Item Deterministically: Choose the column with the smallest Size
	int col = table[0].right;
    for (int aux = table[col].right; aux != 0; aux = table[aux].right)
		if (table[aux].len < table[col].len)
			col = aux;

	coverItem(col);

    for (int aux = table[col].down; aux != col; aux = table[aux].down) {
        solution[k] = aux;

        for (int node = aux + 1; node != aux;) {
            if (table[node].item <= 0) {
                node = table[node].up;
            }
            else {
                coverItem(table[node].item);
                node++;
            }
        }

        search(k + 1);

        aux = solution[k];
        col = table[aux].item;
        for (int node = aux - 1; node != aux;) {
            if (table[node].item <= 0) {
                node = table[node].down;
            }
            else {
                uncoverItem(table[node].item);
                node--;
            }
        }
    }

	uncoverItem(col);
}

int main() {

    int n, m, p;
    cin >> n >> m >> p;

    vector<vector<int>> ed(n, vector<int> (n));
    int a, b, cnt = 1;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        ed[a][b] = ed[b][a] = cnt++;
    }
    for (int i = 0; i < p; i++) {
        int tam;
        cin >> tam;
        vector<int> option(tam - 1);
        int ini, fim; 
        cin >> ini;
        for (int j = 0; j < tam - 1; j++) {
            cin >> fim;
            option[j] = ed[ini][fim];
            items = max(items, option[j]);
            ini = fim;
        }
        sort(option.begin(), option.end());
        options.push_back(option);
    }

    for (int i = 0; i <= items; i++) {
        table[i].down = table[i].up = table[i].item = i;
        table[i].len = 0;
        table[i].left = (i + items) % (items + 1);
        table[i].right = (i + 1) % (items + 1);
        tail[i] = i;
    }
    int at = items + 1, last = 0;
    for (int i = 0; i < (int)options.size(); i++) {
        // add a spacer node
        table[at].item = -i;
        table[at].up = last;
        table[at].down = at + options[i].size();
        last = at + 1;
        at++;
        for (int j = 0; j < (int)options[i].size(); j++) {
            table[at].item = options[i][j];
            table[at].down = options[i][j];
            table[at].up = tail[options[i][j]];
            table[options[i][j]].up = at;
            table[tail[options[i][j]]].down = at;
            tail[options[i][j]] = at;
            table[at].option = i;
            at++;
        }
    }
    // last spacer node
    table[at].item = - (int)options.size();
    table[at].up = last;
    table[at].down = 0;
    search(0);
}