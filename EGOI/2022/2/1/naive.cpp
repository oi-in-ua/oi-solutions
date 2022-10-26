// In the name of Allah.
// We're nothing and you're everything.
// Ya Ali!

#include <bits/stdc++.h>

using namespace std;
const int MAX_N = 2e5 + 14;
int n, s, a[MAX_N];

int main() {
	scanf("%d%d", &n, &s);
	for (int i = 0; i < n; ++i)
		scanf("%d", &a[i]);
	sort(a, a + n, greater<int>());
	for (int i = 0; i < s; ++i) {
		int m, d;
		scanf("%d%d", &m, &d);
		for (int j = 0; j < d; ++j)
			a[j] -= m;
		sort(a, a + n, greater<int>());
	}
	for (int i = 0; i < n; ++i)
		printf("%d ", a[i]);
	printf("\n");
}
