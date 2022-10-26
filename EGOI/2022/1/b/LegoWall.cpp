#include <bits/stdc++.h>
using namespace std;

#define SORT(v) sort((v).begin(), (v).end())
#define RSORT(v) sort((v).rbegin(), (v).rend())
#define REVERSE(v) reverse((v).begin(), (v).end())
#define MAX(v) (*max_element((v).begin(), (v).end()))
#define MIN(v) (*min_element((v).begin(), (v).end()))
#define pb push_back
#define FOR(i, n) for (int i = 0; i < (n); i++)
typedef long long ll;
typedef unsigned long long ull;

int mod = 1e9 + 7;
// int mod = 998244353;

inline void sum_self(int &a, int b)
{
    a += b;
    if (a >= mod)
        a -= mod;
    if (a < 0)
        a += mod;
}
inline int sum(int a, int b)
{
    a += b;
    if (a >= mod)
        a -= mod;
    if (a < 0)
        a += mod;
    return a;
}
inline void mul_self(int &a, int b) { a = 1ll * a * b % mod; }
inline int mul(int a, int b)
{
    a = 1ll * a * b % mod;
    return a;
}
inline int exp(ll b, ll p)
{
    if (p < 0)
        return 0;
    b %= mod;
    int res = 1;
    int mul = b;
    while (p)
    {
        if (p & 1)
            mul_self(res, mul);
        p >>= 1;
        mul_self(mul, mul);
    }
    return res;
}
inline int invexp(ll b) { return exp(b, mod - 2); } // 1/i modulo mod

const int N = 2e5 + 5;

void solve()
{
    int w, h;
    cin >> w >> h;
    if (w > 1000)
    {
        vector<int> f(2 * h, 1), invf(2 * h);
        FOR(i, 2 * h - 1)
        f[i + 1] = mul(f[i], i + 1);
        invf[2 * h - 1] = invexp(f[2 * h - 1]);
        for (int i = 2 * h - 2; i >= 0; i--)
            invf[i] = mul(invf[i + 1], i + 1);
        vector<vector<int>> c(h + 1, vector<int>(h / 2 + 1, -1));
        auto C = [&](int a, int b)
        {
            b = min(b, a - b);
            if (c[a][b] != -1)
                return c[a][b];
            return c[a][b] = mul(f[a], mul(invf[b], invf[a - b]));
        };
        vector<int> dp(h + 1);
        dp[0] = 1;
        w--;
        FOR(j, w)
        {
            vector<int> ndp(h + 1);
            for (int i = 1; i <= h; i++)
            {
                // previous at most h-i
                for (int prev = j > 0; prev <= h - i; prev++)
                {
                    sum_self(ndp[i], mul(dp[prev], C(h - prev, i)));
                }
            }
            dp.swap(ndp);
        }
        int ans = 0;
        FOR(i, h)
        sum_self(ans, dp[i]);
        if (w == 1)
            sum_self(ans, dp[h]);
        cout << ans << "\n";
        return;
    }

    /*
        dp1[w][h] = no empty col
        dp2[w][h] = at least 1 col empty

        dp1[i] = all[i]^h - dp2[i]
        dp2[i] = dp2[i-j] * dp1[j-1]
        all[i] = all[i-1] + all[i-2] -> dp1 + dp2 (fill unconditionally)
    */
    vector<int> dp1(w);
    vector<int> dp2(w);
    dp1[0] = dp2[0] = 1;
    vector<int> all(w, 1);
    for (int i = 1; i < w; i++)
        all[i] = sum(all[i - 1], i - 2 >= 0 ? all[i - 2] : 1);
    for (int i = 1; i < w; i++)
        all[i] = exp(all[i], h);
    for (int i = 1; i < w; i++)
    {
        dp2[i] = all[i - 1]; // col i is empty
        // cols j+1 to i are non-empty empty but col j is empty
        for (int j = 1; j < i; j++)
        {
            sum_self(dp2[i], mul(all[j - 1], dp1[i - j]));
        }
        dp1[i] = sum(all[i], -dp2[i]);
    }
    cout << dp1[w - 1] << "\n";
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }
}