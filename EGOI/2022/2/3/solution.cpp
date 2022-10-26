#include <bits/stdc++.h>

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<int> root(n);
  vector<int> roots;
  roots.push_back(0);
  vector<int> prefix_joins;
  prefix_joins.push_back(0);
  for (int i = 1; i < n; ++i) {
    int left = -1;
    int right = roots.size();
    int last_got = -1;
    while (right - left > 1) {
      int middle = (left + right) / 2;
      cout << "? " << prefix_joins[middle] << " " << 1 << " " << (i + 1) << "\n";
      cout.flush();
      int got;
      cin >> got;
      if (got == prefix_joins[middle]) {
        right = middle;
      } else {
        left = middle;
        last_got = got;
      }
    }
    if (right < roots.size()) {
      root[i] = roots[right];
    } else {
      root[i] = i;
      roots.push_back(i);
      prefix_joins.push_back(last_got);
    }
  }
  cout << "! " << (n - roots.size()) << "\n";
  for (int i = 0; i < n; ++i) if (i != root[i]) {
    cout << (i + 1) << " " << (root[i] + 1) << "\n";
  }
  return 0;
}