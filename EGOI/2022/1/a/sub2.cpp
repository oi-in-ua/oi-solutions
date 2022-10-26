#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    vector <int64_t> f(n);
    for(auto&i : f)
        scanf("%lld",&i);
    f.push_back(0);
    int64_t ans = 0;
    while(f[n] == 0){
        int i = 0;
        while(f[i] > 0)
            f[i++]--;
        f[i]++;
        ans++;
    }
    printf("%lld\n",ans);
}
