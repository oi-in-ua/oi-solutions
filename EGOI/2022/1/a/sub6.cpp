#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    vector <int64_t> f(n);
    for(auto&i : f)
        scanf("%lld",&i);
    f[0] = min(f[0] ,1LL<<n-1);
    printf("%lld\n",(1LL<<n) - f[0]);
}
