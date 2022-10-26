// O(N log^2 N)
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

#define MAXN 300000

vector<int> adj[MAXN];

struct Node {
  int index;
  int size;
  Node* left;
  Node* right;
  Node* parent;
  Node* heavy;

  Node* Heavy() { return heavy; }
  Node* Light() { return left == heavy ? right : left; }
} nodes[MAXN], *node_alloc = nodes;

Node* BuildTree(int i, int parent) {
  Node* node = node_alloc++;
  node->index = i;
  node->size = 1;
  for (vector<int>::iterator it = adj[i].begin(); it != adj[i].end(); ++it) {
    if (*it == parent) continue;
    if (!node->left) {
      node->left = BuildTree(*it, i);
      node->left->parent = node;
      node->size += node->left->size;
      node->heavy = node->left;
    } else {
      node->right = BuildTree(*it, i);
      node->right->parent = node;
      node->size += node->right->size;
      if (node->right->size > node->left->size) {
        node->heavy = node->right;
      }
    }
  }
  return node;
}

struct Point {
  int index;
  int vertex;
  int x, y;
} p[MAXN];

long long SignedArea(Point const& A, Point const& B, Point const& C) {
  int dx1 = A.x - C.x;
  int dy1 = A.y - C.y;
  int dx2 = B.x - C.x;
  int dy2 = B.y - C.y;
  return (long long)dx1 * dy2 - (long long)dx2 * dy1;
}

struct CmpAngleFromPointOnHull {
  Point const& O;
  CmpAngleFromPointOnHull(Point const& O) : O(O) {}
  bool operator()(Point const& A, Point const& B) {
    return SignedArea(O, A, B) > 0;
  }
};

struct CmpAngleFromSegment {
  Point const& P;
  Point const& Q;
  CmpAngleFromSegment(Point const& P, Point const& Q) : P(P), Q(Q) {}
  bool operator()(Point const& A, Point const& B) {
    bool sgnA = SignedArea(P, Q, A) > 0;
    bool sgnB = SignedArea(P, Q, B) > 0;
    if (sgnA != sgnB) return sgnA;
    return SignedArea(P, A, B) > 0;
  }
};

struct CmpDist {
  Point const& P;
  Point const& Q;
  CmpDist(Point const& P, Point const& Q) : P(P), Q(Q) {}
  bool operator()(Point const& A, Point const& B) {
    return SignedArea(P, Q, A) < SignedArea(P, Q, B);
  }
};

bool CmpIndex(Point const& A, Point const& B) {
  return A.index < B.index;
}

Node* DiveHeavy(Node* A) {
  if (!A) return A;
  while (A->Heavy()) A = A->Heavy();
  return A;
}

int Size(Node* A) {
  return A ? A->size : 0;
}

Node* GetMidPoint(Node* A, Node* B) {
  Node* C = B;
  Node* D = B;
  while (D != A) {
    D = D->parent;
    if (D == A) break;
    C = C->parent;
    D = D->parent;
  }
  return C;
}

void DrawPath(Node* A, Node* B, Point const& pA, Point* p, int n);
void DrawPath(Node* A, Node* B, Point const& pA, Point const& pB, Point* p, int n);

void DrawPath(Node* A, Node* B, Point const& pA, Point* p, int n) {
  if (!B) return;
  CmpAngleFromPointOnHull cmp(pA);
  nth_element(p, p, p + n, cmp);
  Point& pB = p[0];
  pB.vertex = B->index;
  DrawPath(A, B, pA, pB, p + 1, n - 1);
}

void DrawPath(Node* A, Node* B,
              Point const& pA, Point const& pB,
              Point* p, int n) {
  Node* C = GetMidPoint(A, B);
  if (C == B) return;
  CmpAngleFromPointOnHull cmpA(pA);
  CmpAngleFromPointOnHull cmpB(pB);
  CmpDist cmp_dist(pA, pB);
  int heavy_size = Size(C->Heavy()) - Size(B);
  int light_size = Size(C->Light());

  nth_element(p, p + heavy_size, p + n, cmpB);
  Point cand1 = p[heavy_size];

  nth_element(p, p + heavy_size + light_size, p + n, cmpA);
  Point cand2 = p[heavy_size + light_size];

  bool any = false;
  for (int i = 0; i < n; ++i) {
    if (SignedArea(pB, cand1, p[i]) >= 0 &&
        SignedArea(cand2, pA, p[i]) >= 0) {
      if (!any) {
        swap(p[0], p[i]);
        any = true;
      } else {
        if (cmp_dist(p[i], p[0])) {
          swap(p[0], p[i]);
        }
      }
    }
  }
  if (!any) fprintf(stderr, "Fail!\n");
  Point& pC = p[0];
  pC.vertex = C->index;
  CmpAngleFromSegment cmpC(pC, pB);
  ++p;
  --n;

  nth_element(p, p + heavy_size, p + n, cmpC);
  DrawPath(C, B, pC, pB, p, heavy_size);
  p += heavy_size;
  n -= heavy_size;

  nth_element(p, p + light_size, p + n, cmpC);
  DrawPath(C, DiveHeavy(C->Light()), pC, p, light_size);
  p += light_size;
  n -= light_size;

  DrawPath(A, C, pA, pC, p, n);
}

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 1; i < n; ++i) {
    int u, v;
    scanf("%d%d", &u, &v); --u; --v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  int root_index = 0;
  while (adj[root_index].size() > 1) ++root_index;
  Node* root = BuildTree(root_index, -1);

  for (int i = 0; i < n; ++i) {
    p[i].index = i;
    p[i].vertex = -1;
    scanf("%d%d", &p[i].x, &p[i].y);
    if (p[i].y < p[0].y) swap(p[0], p[i]);
  }

  Point& pA = p[0];
  pA.vertex = root->index;
  DrawPath(root, DiveHeavy(root), pA, p + 1, n - 1);

  sort(p, p + n, CmpIndex);
  for (int i = 0; i < n; ++i) {
    printf("%d ", p[i].vertex + 1);
  }
  printf("\n");
  return 0;
}
