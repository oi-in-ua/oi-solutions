#include <bits/stdc++.h>

using namespace std;

int n, x;
string s;

pair <int, int> solve() {
  if (s[x++] == '?') return {1, n};

  char c = s[x];
  x += 3;
  auto a = solve();
  x++;
  auto b = solve();
  x++;

  if (c == 'i') {
    return {min(a.first, b.first), a.second + b.second - n - 1};
  }
  return {a.first + b.first, max(a.second, b.second)};
}

int main() {
  ios_base::sync_with_stdio(false);

  cin >> s;
  n = count(s.begin(), s.end(), '?');
  auto tr = solve();
  cout << tr.second - tr.first + 1 << "\n";

  return 0;
}
