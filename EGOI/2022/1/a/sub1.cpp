#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    scanf("%d",&n);
    if(n == 1){
        int64_t a;
        scanf("%lld",&a);
        if(a == 0)
            printf("2\n");
        else
            printf("1\n");
    }
    else if(n == 2){
        int64_t a ,b;
        scanf("%lld%lld",&a,&b);
        if(b >= 1){
            if(a == 0)
                printf("2\n");
            else
                printf("1\n");
        }
        else{
            if(a == 0)
                printf("4\n");
            else if(a == 1)
                printf("3\n");
            else if(a >= 2)
                printf("2\n");
        }
    }
}
