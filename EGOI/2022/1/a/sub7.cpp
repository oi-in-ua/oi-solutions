#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    vector <int64_t> f(n);
    for(auto&i : f)
        scanf("%lld",&i);
    int i = 0;
    while(i+1 < n && f[i] == 0)
        i++;
    f[i] = min(f[i] ,1LL<<n-i-1);
    printf("%lld\n",(1LL<<n) - (f[i]<<i));
}
