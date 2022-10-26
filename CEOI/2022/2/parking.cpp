#include<bits/stdc++.h>

using namespace std;

typedef long long llint;
typedef pair <int, int> pi;

const int MAXN = 400005;

int n, m, moze = 1;
int b[MAXN], t[MAXN], par[MAXN], siz[MAXN], is_cyc[MAXN], cnt_up[MAXN], cnt[MAXN];
set <int> prazni;
vector <int> pos[MAXN], v[MAXN];
vector < pair <pi, int> > red;
vector <pi> sol;

void potez (int x, int y) {
    sol.push_back({x, y});
    cnt[x]--; cnt[y]++;
    if (cnt[y] == 1) prazni.erase(y);
    if (cnt[x] == 0) prazni.insert(x);
}

int nadi (int x) {
    if (x == par[x]) return x;
    return par[x] = nadi(par[x]);
}

void add_edge (int x, int y) {
    v[x].push_back(y);
    v[y].push_back(x);
    bool upp = (x % 2 == 1) && (y % 2 == 1);
    x = nadi(x); y = nadi(y);
    if (upp) cnt_up[y]++;
    if (x == y) {
        is_cyc[x] = 1;
        return;
    }
    par[x] = y;
    siz[y] += siz[x];
    cnt_up[y] += cnt_up[x];
}

void remove_edge (int x, int y) {
    v[x].erase(find(v[x].begin(), v[x].end(), y));
    v[y].erase(find(v[y].begin(), v[y].end(), x));
}

void build_graph () {
    for (int i = 1; i <= n; i++) {
        int x = pos[i][0], y = pos[i][1];
        if (x / 2 != y / 2) add_edge(x, y);
    }
    for (int i = 0; i < m; i++) {
        if (b[i] && t[i] && b[i] != t[i]) add_edge(2 * i, 2 * i + 1);
    }
    for (int i = 0; i < 2 * m; i++) {
        if (i == par[i] && siz[i] > 1) red.push_back({{is_cyc[i], cnt_up[i]}, i});
    }
    sort(red.begin(), red.end());
}

int get_end_of_path (int x, int prosli) {
    if (v[x].size() == 1) return x;
    for (auto sus : v[x]) {
        if (sus == prosli) continue;
        return get_end_of_path(sus, x);
    }
    return -1;
}

void solve_path (int idx) {
    int curr = get_end_of_path(idx, -1), prosli = -1;
    vector <int> r;
    while (curr != -1) {
        r.push_back(curr);
        int nxt = -1;
        for (auto sus : v[curr]) {
            if (sus == prosli) continue;
            nxt = sus;
        }
        prosli = curr;
        curr = nxt;
    }
    for (int i = 0; i < r.size(); i++) {
        if (r[i + 1] % 2 == 1) {
            potez(r[i + 1] / 2, r[i] / 2);
            i++;
        } else {
            int k = -1;
            for (int j = i; j < (int)r.size() - 1; j++) {
                if (r[j] % 2 == 1 && r[j + 1] % 2 == 1) {
                    k = j;
                    break;
                }
            }
            if (k != -1) {
                if (prazni.size() == 0) {
                    moze = 0;
                    return;
                }
                int to = *prazni.begin();
                potez(r[k] / 2, to);
                potez(r[k + 1] / 2, to);
                for (int j = k - 1; j >= i; j -= 2) {
                    potez(r[j - 1] / 2, r[j] / 2);
                }
                i = k + 1;
            } else {
                for (int j = (int)r.size() - 1; j > i + 1; j -= 2) {
                    potez(r[j - 1] / 2, r[j] / 2);
                }
                potez(r[i] / 2, r[i + 1] / 2);
                break;
            }
        }
    }
}

void solve_cycle (int idx) {
    if (prazni.size() == 0) {
        moze = 0;
        return;
    }
    int pocetak = idx, curr = idx, prosli = -1;
    vector <int> r;
    while (curr != -1) {
        r.push_back(curr);
        int nxt = -1;
        for (auto sus : v[curr]) {
            if (sus == prosli || sus == pocetak) continue;
            nxt = sus;
        }
        prosli = curr;
        curr = nxt;
    }
    int R = r.size();

    int k = -1;
    for (int i = 0; i < R; i++) {
        if (r[i] % 2 == 1) k = i;
        if (r[i] % 2 == 1 && r[(i + 1) % R] % 2 == 1) {
            k = i;
            break;
        }
    }
    int x = r[k], y = r[(k + 1) % R];
    if (y % 2 == 1) {
        int to = *prazni.begin();
        potez(x / 2, to);
        potez(y / 2, to);
        remove_edge(r[(k - 1 + R) % R], r[k]);
        remove_edge(r[k], r[(k + 1) % R]);
        remove_edge(r[(k + 1) % R], r[(k + 2) % R]);
        solve_path(r[(k + 2) % r.size()]);
    } else {
        int to = *prazni.begin();
        if (x / 2 != y / 2) {
            potez(x / 2, to);
            for (int i = 2; i < R; i += 2) {
                potez(r[(k - i + R) % R] / 2, r[(k - i + 1 + R) % R] / 2);
            }
            potez(y / 2, to);
        } else {
            potez(x / 2, to);
            for (int i = 1; i + 1 < R; i += 2) {
                potez(r[(k + i + 1) % R] / 2, r[(k + i) % R] / 2);
            }
            potez(r[(k - 1 + R) % R] / 2, to);
        }
    }
}

void ispis () {
    if (moze == 0) {
        cout << -1 << '\n';
        return;
    }
    cout << sol.size() << endl;
    for (auto pp : sol) cout << pp.first + 1 << " " << pp.second + 1 << '\n';
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        cin >> b[i] >> t[i];
        if (b[i]) pos[b[i]].push_back(2 * i);
        if (t[i]) pos[t[i]].push_back(2 * i + 1);
        par[2 * i] = 2 * i; siz[2 * i] = 1;
        par[2 * i + 1] = 2 * i + 1; siz[2 * i + 1] = 1;
        if (b[i]) cnt[i]++;
        if (t[i]) cnt[i]++;
        if (cnt[i] == 0) prazni.insert(i);
    }
    build_graph();
    for (int i = 0; i < red.size(); i++) {
        int idx = red[i].second;
        if (!is_cyc[idx]) solve_path(idx); else solve_cycle(idx);
    }
    ispis();
    return 0;
}
