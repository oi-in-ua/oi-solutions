#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    vector <int64_t> f(n);
    for(auto&i : f)
        scanf("%lld",&i);
    int64_t ans = 1;
    for(int i = n - 1; i >= 0; i--)
        ans += max(ans - f[i] ,0LL);
    printf("%lld\n",ans);
}
