#include<bits/stdc++.h>

using namespace std;

typedef long long llint;
typedef pair <int, int> pi;

const int MAXN = 200005;
const int MAXQ = 1000005;

int n, q, siz, ofs = 1, flg;
int p[MAXN], nxt[MAXN], sol[MAXQ];
int t[MAXN * 8];
set < pair <int, pi> > st;
vector < pair <int, pi> > v;
vector <pi> queries[MAXN];

void compute_next () {
    vector <int> st;
    st.push_back(n + 1);
    p[n + 1] = 1e9;
    for (int i = n; i >= 1; i--) {
        while (p[i] > p[st.back()]) st.pop_back();
        nxt[i] = st.back();
        st.push_back(i);
    }
}

int get_idx (int a, int b) {
    return lower_bound(v.begin(), v.end(), make_pair(p[a], make_pair(a, b))) - v.begin();
}

void tour_init () {
    sort(v.begin(), v.end());
    while (ofs < v.size()) ofs *= 2;
}

void update (int i, int tip) {
    t[i + ofs] = v[i].second.second - v[i].second.first + 1;
    if (tip == -1) t[i + ofs] = 0;
    i = (i + ofs) / 2;
    while (i > 0) {
        t[i] = t[2 * i] + t[2 * i + 1];
        i /= 2;
    }
}

int upit (int x, int val, int lo = 0, int hi = ofs - 1) {
    if (lo == hi) return p[v[lo].second.first + val - 1];
    if (val <= t[2 * x]) return upit(2 * x, val, lo, (lo + hi) / 2);
    return upit(2 * x + 1, val - t[2 * x], (lo + hi) / 2 + 1, hi);
}

void ubaci (int a, int b) {
    st.insert({p[a], {a, b}});
    siz += b - a + 1;
    if (flg == 0) v.push_back({p[a], {a, b}});
    if (flg == 1) update(get_idx(a, b), 1);
}

void podijeli (int a, int b) {
    while (a <= b) {
        int lim = min(nxt[a] - 1, b);
        ubaci(a, lim);
        a = nxt[a];
    }
}

void remove_from_back () {
    while (1) {
        int a = (st.rbegin()->second).first;
        int b = (st.rbegin()->second).second;
        if (siz - (b - a + 1) < n / 2) break;
        auto it = st.end(); it--;
        st.erase(it);
        siz -= b - a + 1;
    }
}

void update_mid () {
    int a = (st.rbegin()->second).first;
    int b = (st.rbegin()->second).second;
    st.erase({p[a], {a, b}});
    siz -= b - a + 1;
    if (flg) update(get_idx(a, b), -1);
    int ofs = n / 2 - siz;
    ubaci(a, a + ofs - 1);
    podijeli(a + ofs, b);
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
    }
    for (int i = 1; i <= q; i++) {
        int t, pos;
        cin >> t >> pos;
        t = min(t, n);
        queries[t].push_back({i, pos});
    }
    compute_next();
    podijeli(1, n);
    while (1) {
        remove_from_back();
        if (siz == n / 2) break;
        update_mid();
    }
    tour_init();

    siz = 0; flg = 1;
    st.clear();
    podijeli(1, n);
    for (int t = 0; t <= n; t++) {
        for (auto pp : queries[t]) {
            int i = pp.first, pos = pp.second;
            sol[i] = upit(1, pos);
        }
        if (siz > n / 2) {
            remove_from_back();
            update_mid();
        }
    }
    for (int i = 1; i <= q; i++) {
        cout << sol[i] << '\n';
    }
    return 0;
}
