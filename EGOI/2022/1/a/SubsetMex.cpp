/*
  Task: subsetmex (egoi-2022-subsetmex)
  Author: Hazem Issa
  Comments: full solution o(n)
*/

#include <bits/stdc++.h>
using namespace std;

void solve()
{
    int n;
    scanf("%d",&n);
    vector <long long> f(n);
    for(auto&i : f)
        scanf("%lld",&i);
    long long ans = 1;
    for(int i = n - 1; i >= 0; i--)
        ans += max(ans - f[i] ,0LL);
    printf("%lld\n",ans);
}

int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
        solve();
}
