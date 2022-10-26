#include <bits/stdc++.h>
using namespace std;

#define FOR(i, a, b) for (int i = (int)(a); i < (int)(b); ++i)
#define REP(i, n) FOR(i, 0, n)
#define TRACE(x) cerr << #x << " = " << x << endl
#define _ << " _ " <<

const int K = 100010;
const int N = 1000100;

int n, k, q, t;
bool inset[N];
int L2[N];

struct Tree {
  int root;
  vector<int> adj[N];
} t1, t2;

struct CompressedTree {
  static const int LOG = 18;
  static const int SIZE = 2 * K;

  int dep[SIZE];
  int dad[SIZE][LOG];
  int dist[SIZE];
  vector<pair<int, int>> hints[SIZE];

  int comp_to_orig[SIZE];
  unordered_map<int, int> orig_to_comp;

  bool vis[SIZE];

  int Lca(int a, int b) {
    if (dep[a] < dep[b]) swap(a, b);
    int start = L2[dep[a] - dep[b]];
    for (int i = start; i >= 0; --i) {
      if (dep[dad[a][i]] < dep[b]) continue;
      a = dad[a][i];
    }
    if (a == b) return a;
    start = L2[dep[a]];
    for (int i = start; i >= 0; --i) {
      if (dad[a][i] == dad[b][i]) continue;
      a = dad[a][i];
      b = dad[b][i];
    }
    return dad[a][0];
  }

  void AddHint(int a, int b, int d) {
    assert(dep[a] <= dep[b]);
    if (a == b) return;
    hints[a].emplace_back(b, d);
    hints[b].emplace_back(a, -d);
  }

  void Solve(int x = 0) {
    if (vis[x]) return;
    vis[x] = true;
    for (const auto& [y, d] : hints[x]) {
      dist[y] = dist[x] + d;
      Solve(y);
    }
  }

  int Dist(int a_orig, int b_orig) {
    int a = orig_to_comp[a_orig];
    int b = orig_to_comp[b_orig];
    int l = Lca(a, b);
    return dist[a] - dist[l] + dist[b] - dist[l];
  }
} ct1, ct2;

struct Compressor {
  Tree& tree;
  CompressedTree& comp_tree;
  Compressor(Tree& tree, CompressedTree& comp_tree)
      : tree(tree), comp_tree(comp_tree) {}

  vector<int> vec;
  unordered_map<int, vector<int>> adj;

  int Dfs1(int x) {
    vector<int> children;
    for (int y : tree.adj[x]) {
      int r = Dfs1(y);
      if (r != 0) {
        children.push_back(r);
      }
    }
    if (inset[x] || children.size() > 1) {
      vec.push_back(x);
      adj.emplace(x, move(children));
      return x;
    }
    if (children.size() == 1) {
      return children[0];
    }
    return 0;
  }

  void Dfs2(int x, int dad = 0, int d = 0) {
    comp_tree.dep[x] = d;
    comp_tree.dad[x][0] = dad;
    FOR(i, 1, CompressedTree::LOG) {
      comp_tree.dad[x][i] = comp_tree.dad[comp_tree.dad[x][i - 1]][i - 1];
    }
    int x_orig = comp_tree.comp_to_orig[x];
    for (int y_orig : adj[x_orig]) {
      int y = comp_tree.orig_to_comp[y_orig];
      Dfs2(y, x, d + 1);
    }
  }

  void Compress() {
    vec.reserve(2 * k);
    Dfs1(tree.root);
    reverse(vec.begin(), vec.end());
    REP(i, vec.size()) {
      comp_tree.comp_to_orig[i] = vec[i];
      comp_tree.orig_to_comp[vec[i]] = i;
    }
    Dfs2(0);
  }
};

void LoadTree(Tree& tree) {
  REP(i, n) {
    int x;
    cin >> x;
    if (x == -1) {
      tree.root = i + 1;
    } else {
      tree.adj[x].push_back(i + 1);
    }
  }
}

vector<int> GetSet(Tree &tree) {
  vector<int> ret;
  ret.reserve(k);
  queue<int> qq;
  for (qq.push(tree.root); (int)ret.size() < k; qq.pop()) {
    int x = qq.front();
    ret.push_back(x);
    for (int y : tree.adj[x]) {
      qq.push(y);
    }
  }
  return ret;
}

vector<int> GetOrdered(Tree &tree) {
  vector<int> ret;
  ret.reserve(k);
  stack<int> s;
  for (s.push(tree.root); !s.empty();) {
    int x = s.top(); s.pop();
    if (inset[x]) ret.push_back(x);
    for (int y : tree.adj[x]) {
      s.push(y);
    }
  }
  assert((int)ret.size() == k);
  return ret;
}

void PrintSet(const vector<int>& S) {
  REP(i, k) {
    cout << S[i];
    if (i + 1 != k) cout << " ";
  }
  cout << endl;
}

void PrintQueries(const vector<int>& order) {
  REP(i, k - 1) {
    cout << "? " << order[i] << " " << order[i + 1] << "\n";
  }
  cout << "!" << "\n";
  cout.flush();
}

int main(void) {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> n >> k >> q >> t;
  LoadTree(t1);
  LoadTree(t2);

  L2[1] = 0;
  FOR(i, 2, N) L2[i] = L2[i / 2] + 1;

  vector<int> S = GetSet(t1);
  for (int x : S) inset[x] = true;
  PrintSet(S);

  vector<int> order = GetOrdered(t2);
  PrintQueries(order);

  Compressor(t1, ct1).Compress();
  Compressor(t2, ct2).Compress();

  REP(i, k - 1) {
    int a_orig = order[i];
    int b_orig = order[i + 1];
    int a1 = ct1.orig_to_comp[a_orig];
    int b1 = ct1.orig_to_comp[b_orig];
    int a2 = ct2.orig_to_comp[a_orig];
    int b2 = ct2.orig_to_comp[b_orig];
    int l1 = ct1.Lca(a1, b1);
    int l2 = ct2.Lca(a2, b2);
    int d1a, d1b, d2a, d2b;
    cin >> d1a >> d1b >> d2a >> d2b;
    ct1.AddHint(l1, a1, d1a);
    ct1.AddHint(l1, b1, d1b);
    ct2.AddHint(l2, a2, d2a);
    ct2.AddHint(l2, b2, d2b);
  }

  ct1.Solve();
  ct2.Solve();

  vector<pair<int, int>> queries;
  REP(i, t) {
    int a, b;
    cin >> a >> b;
    queries.emplace_back(a, b);
  }

  for (auto [a, b] : queries) {
    cout << ct1.Dist(a, b) << " " << ct2.Dist(a, b) << "\n";
  }
  cout.flush();

  return 0;
}
