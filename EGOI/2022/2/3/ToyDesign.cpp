#include <bits/stdc++.h>
#include "toydesign.h"

using namespace std;

void ToyDesign(int n, int max_ops) {
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
      int got = Connected(prefix_joins[middle],1,(i + 1));
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
  vector <pair<int,int>> result;
  for (int i = 0; i < n; ++i) if (i != root[i])
    result.push_back({(i + 1) , (root[i] + 1) });
  DescribeDesign(result);
  return;
}