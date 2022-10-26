// Tourists - full solution

#include <bits/stdc++.h>
using namespace std;

// segment tree size
const int S2 = 524288;

int lca[S2][20];
int height[S2];
bool odw[S2];
vector<int> sc[S2];

void dfs(int w, int h, int f = -1)
{
    height[w] = h;
    if (odw[w])
    {
        exit(0);
    }
    odw[w] = 1;
    lca[w][0] = f;
    for (int i = 0; i < 19; i++)
    {
        if (lca[w][i] == -1 || lca[lca[w][i]][i] == -1)
            break;
        lca[w][i + 1] = lca[lca[w][i]][i];
    }
    for (auto i : sc[w])
    {
        if (i == f)
            continue;
        dfs(i, h + 1, w);
    }
}

int distance2(int a, int b)
{
    if (height[b] > height[a])
        swap(a, b);
    int currentCost = 0;
    for (int i = 19; i >= 0; i--)
    {
        if (height[a] - (1 << i) >= height[b])
        {
            currentCost += (1 << i);
            a = lca[a][i];
        }
    }
    if (a == b)
        return currentCost;
    for (int i = 19; i >= 0; i--)
    {
        if (lca[a][i] == lca[b][i])
            continue;
        currentCost += (1 << (i + 1));
        a = lca[a][i];
        b = lca[b][i];
    }
    return currentCost + 2;
}

pair<int, int> lst;
int lstw = 0;

int A,B;

inline int distance(int a, int b)
{
    A++;
    if (lst != make_pair(a, b))
    {
        B++;
        lst = {a, b};
        lstw = distance2(a, b);
    }
    return lstw;
}

// in st[x][0] we keep all opinion changes (S2 from desired solution)
// in st[x][1] we keep current city of all child's (-1 means that there are multiple cities)
long long st[2 * S2][2], S = S2;

long long citiesOpinion[S2];

long long query(int w, int p, int k, int pp)
{
    if (pp > k || pp < p)
        return 0;
    if (pp <= p && pp >= k)
        return st[w][0];
    long long p1 = query(w * 2, p, (p + k) / 2, pp), p2 = query(w * 2 + 1, (p + k) / 2 + 1, k, pp);
    return p1 + p2 + st[w][0];
}

int getCity(int w, int p, int k, int pp)
{
    if (pp > k || pp < p)
        return 0;
    if (st[w][1] != -1)
        return st[w][1];
    return getCity(w * 2, p, (p + k) / 2, pp) + getCity(w * 2 + 1, (p + k) / 2 + 1, k, pp);
}

void changeCity(int w, int p, int k, int pp, int kk, long long c)
{
    if (pp > k || kk < p)
        return;
    if (pp <= p && kk >= k)
    {
        if (st[w][1] != -1)
        {
            st[w][0] += citiesOpinion[st[w][1]];
            st[w][0] -= citiesOpinion[c];
            st[w][0] -= distance(st[w][1], c);
            st[w][1] = c;
            return;
        }
    }
    if (st[w][1] != -1)
        st[w * 2][1] = st[w * 2 + 1][1] = st[w][1];
    changeCity(w * 2, p, (p + k) / 2, pp, kk, c);
    changeCity(w * 2 + 1, (p + k) / 2 + 1, k, pp, kk, c);
    if (st[w * 2][1] != st[w * 2 + 1][1])
        st[w][1] = -1;
    else
        st[w][1] = st[w * 2][1];
}

int main()
{
    ios_base::sync_with_stdio(0);
    int n, m, q;
    cin >> n >> m >> q;
    while (S / 2 > m)
        S /= 2;
    for (int i = 0; i < m; i++)
        cin >> st[i + S][1];
    for (int i = m; i < S; i++)
        st[i + S][1] = -1;
    for (int w = S - 1; w > 0; w--)
    {
        if (st[w * 2][1] != st[w * 2 + 1][1] || min(st[w * 2][1], st[w * 2 + 1][1]) == -1)
            st[w][1] = -1;
        else
            st[w][1] = st[w * 2][1];
    }
    for (int i = 1; i < n; i++)
    {
        int a, b;
        cin >> a >> b;
        sc[a].push_back(b);
        sc[b].push_back(a);
    }
    dfs(1, 0);
    for (int i = 0; i < q; i++)
    {
        string queryType;
        int a, b, c;
        cin >> queryType;
        if (queryType == "e")
        {
            cin >> a >> b;
            citiesOpinion[a] += b;
        }
        else if (queryType == "q")
        {
            cin >> a;
            long long odp = citiesOpinion[getCity(1, 1, S, a)] + query(1, 1, S, a);
            cout << odp << '\n';
        }
        else if (queryType == "t")
        {
            cin >> a >> b >> c;
            changeCity(1, 1, S, a, b, c);
        }
    }
}