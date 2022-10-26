#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    vector <int64_t> f(n);
    for(auto&i : f)
        scanf("%lld",&i);
    int64_t ans = 1LL<<n;
    for(int i = 0; i < n; i++)
        ans -= f[i]<<i;
    printf("%lld\n",ans);
}
