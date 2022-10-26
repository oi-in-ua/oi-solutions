#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    vector <int64_t> f(n) ,q(n+1);
    for(auto&i : f)
        scanf("%lld",&i);
    q[n] = 1;
    for(int i = n - 1; i >= 0; i--){
        q[i] = accumulate(q.begin() ,q.end() ,0LL);
        q[i] = max(q[i] - f[i] ,0LL);
    }
    printf("%lld\n",accumulate(q.begin() ,q.end() ,0LL));
}
