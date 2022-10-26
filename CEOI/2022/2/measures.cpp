#include <bits/stdc++.h>
using namespace std;

#define TRACE(x) cerr << __LINE__ << ": " << #x << " = " << x << endl
#define _ << " _ " <<

template<class> struct is_container : false_type {};
template<class... Ts> struct is_container<vector<Ts...>> : true_type {};
template<class... Ts> struct is_container<map<Ts...>> : true_type {};
template<class... Ts> struct is_container<unordered_map<Ts...>> : true_type {};
template<class... Ts> struct is_container<set<Ts...>> : true_type {};
template<class... Ts> struct is_container<unordered_set<Ts...>> : true_type {};
template<class... Ts> struct is_container<multiset<Ts...>> : true_type {};
template<class... Ts> struct is_container<unordered_multiset<Ts...>> : true_type {};
template<class T, class = typename enable_if<is_container<T>::value>::type>
ostream& operator<<(ostream &o, T x) {
  int f = 1;
  o << "{";
  for (auto y : x) {
    o << (f ? "" : ", ") << y;
    f = 0;
  }
  return o << "}";
}
template<class T, class U>
ostream& operator<<(ostream &o, pair<T, U> x) {
  return o << "(" << x.first << ", " << x.second << ")";
}

#define fi first
#define se second

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;

const int OFF = 1 << 18;
const ll INF = 1e18;

struct SegTree {
    ll mini[2 * OFF], maxi[2 * OFF], lazy[2 * OFF];

    SegTree() {
        for (int i = 1; i < 2 * OFF; i++) {
            mini[i] = INF;
            maxi[i] = -INF;
        }
    }

    void propagate(int i) {
        mini[2 * i + 0] += lazy[i];
        maxi[2 * i + 0] += lazy[i];
        lazy[2 * i + 0] += lazy[i];
        mini[2 * i + 1] += lazy[i];
        maxi[2 * i + 1] += lazy[i];
        lazy[2 * i + 1] += lazy[i];
        lazy[i] = 0;
    }

    void update(int i) {
        mini[i] = min(mini[2 * i], mini[2 * i + 1]);
        maxi[i] = max(maxi[2 * i], maxi[2 * i + 1]);
    }

    void set(int pos, ll val, int i = 1, int lo = 0, int hi = OFF) {
        if (lo + 1 == hi) {
            mini[i] += val - INF;
            maxi[i] += val + INF;
        } else {
            propagate(i);
            int mid = (lo + hi) / 2;
            if (pos < mid) set(pos, val, 2 * i + 0, lo, mid);
            else set(pos, val, 2 * i + 1, mid, hi);
            update(i);
        }
    }

    void add(int l, int r, ll val, int i = 1, int lo = 0, int hi = OFF) {
        if (l <= lo && hi <= r) {
            mini[i] += val;
            maxi[i] += val;
            lazy[i] += val;
        } else if (r <= lo || hi <= l) return;
        else {
            propagate(i);
            int mid = (lo + hi) / 2;
            add(l, r, val, 2 * i + 0, lo, mid);
            add(l, r, val, 2 * i + 1, mid, hi);
            update(i);
        }
    }

    pll query(int l, int r, int i = 1, int lo = 0, int hi = OFF) {
        if (l <= lo && hi <= r) return {mini[i], maxi[i]};
        else if (r <= lo || hi <= l) return {INF, -INF};
        else {
            propagate(i);
            int mid = (lo + hi) / 2;
            auto p = query(l, r, 2 * i + 0, lo, mid);
            auto q = query(l, r, 2 * i + 1, mid, hi);
            return {min(p.fi, q.fi), max(p.se, q.se)};
        }
    }
} T;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    ll d;
    cin >> n >> m >> d;
    vll x(n + m);
    vector<pll> vec(n + m);
    for (int i = 0; i < n + m; i++) {
        cin >> x[i];
        vec[i] = {x[i], i};
    }
    sort(vec.begin(), vec.end());
    vi idx(n + m);
    for (int i = 0; i < n + m; i++) idx[vec[i].se] = i;

    ll sol = 0;
    for (int i = 0; i < n + m; i++) {
        T.set(idx[i], -x[i]);
        T.add(idx[i], OFF, d);

        sol = max(sol, T.query(idx[i], OFF).se - T.query(0, idx[i]).fi);
        sol = max(sol, T.query(idx[i] + 1, OFF).se - T.query(0, idx[i] + 1).fi);

        if (i >= n) cout << sol / 2 << ((sol & 1) ? ".5" : "") << " ";
    }
    cout << "\n";

    return 0;
}

