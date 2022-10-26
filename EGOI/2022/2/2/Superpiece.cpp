//Superpiece - solution 1

#include <iostream>

using namespace std;

bool rook = false, bishop = false, queen = false, king = false, pawn = false, knight = false;
constexpr int INF = 1e9;

int solve(int a, int b, int c, int d, int max_premoves)
{
    if (rook && (a == c || b == d))
        return 1;
    else if (bishop && (a - b == c - d || a + b == c + d))
        return 1;
    else if (knight && abs((a - c) * (b - d)) == 2)
        return 1;
    else if (king && max(abs(a - c), abs(b - d)) == 1)
        return 1;
    else if (pawn && a + 1 == c && b == d)
        return 1;
    else if (rook)
        return 2;
    // else if (bishop && (a+b+c+d)%2==0) return 2;
    else
    {
        // no rook or queen, if bishop then different colours, also ans != 1
        // we need to move at least once with king, pawn or knight
        int ans = INF;

        // just king
        if (king)
            ans = min(ans, max(abs(a - c), abs(b - d)));

        // just pawn
        if (pawn && a < c && b == d)
            ans = min(ans, c - a);

        // just knight
        if (knight)
        {
            int da = abs(a - c), db = abs(b - d);
            if (da > db)
                swap(da, db);
            int m1 = (da + db) / 3 + (da + db) % 3;
            int m2 = (db + 1) / 2;
            if (((db - 2 * da) % 4) >= 2)
                m2 += 1;
            int m = max(m1, m2);
            if (da + db == 1 || (da == 2 && db == 2))
                m += 2;
            ans = min(ans, m);
        }

        if (max_premoves > 0)
        {
            --max_premoves;
            if (pawn)
                ans = min(ans, 1 + solve(a + 1, b, c, d, max_premoves));
            if (king)
            {
                for (int da = -1; da <= 1; ++da)
                {
                    for (int db = -1; db <= 1; ++db)
                    {
                        if (da != 0 || db != 0)
                        {
                            ans = min(ans, 1 + solve(a + da, b + db, c, d, max_premoves));
                        }
                    }
                }
            }
            if (knight)
            {
                for (int da = -2; da <= 2; ++da)
                {
                    for (int db = -2; db <= 2; ++db)
                    {
                        if (abs(da * db) == 2)
                        {
                            ans = min(ans, 1 + solve(a + da, b + db, c, d, max_premoves));
                        }
                    }
                }
            }
        }

        return ans;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        string S;
        cin >> S;
        for (char s : S)
        {
            switch (s)
            {
            case 'R':
                rook = true;
                break;
            case 'B':
                bishop = true;
                break;
            case 'Q':
                queen = rook = bishop = true;
                break;
            case 'K':
                king = true;
                break;
            case 'P':
                pawn = true;
                break;
            case 'N':
                knight = true;
                break;
            }
        }

        int a, b, c, d;
        cin >> a >> b >> c >> d;
        int ans = solve(a, b, c, d, 1);
        if (ans >= INF)
        {
            cout << -1 << '\n';
        }
        else
        {
            cout << ans << '\n';
        }
    }
}
