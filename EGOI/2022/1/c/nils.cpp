#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const int MAXN = 200001;
int n,m;
vector<vi> C(MAXN, vi());
bool friend1[MAXN] = {0};

bool mark[MAXN] = {0};

vector<vi> match_path(MAXN, vi());

int dfs(int i){
    vi unmatched;
    mark[i] = 1;
    if(friend1[i])unmatched.push_back(i);
    trav(y, C[i]){
        if(!mark[y]){
            int j = dfs(y);
            if(j != -1){
                match_path[j].push_back(i);
                unmatched.push_back(j);
            }
        }
    }

    while(sz(unmatched) >= 2){
        int a = unmatched.back();
        unmatched.pop_back();
        int b = unmatched.back();
        unmatched.pop_back();

        // match unmatched pairs together, and merge their paths
        // There are probably much nicer ways of writing this :)
        for(int c1 = sz(match_path[b])-2; c1 >= 0; c1--){
            match_path[a].push_back(match_path[b][c1]);
        }
        if(sz(match_path[a]) == 0 || match_path[a].back() != b)match_path[a].push_back(b);

        match_path[b].clear();
        for(int c1 = sz(match_path[a])-2; c1 >= 0; c1--){
            match_path[b].push_back(match_path[a][c1]);
        }
        if(sz(match_path[b]) == 0 || match_path[b].back() != a)match_path[b].push_back(a);
    }
    if(sz(unmatched) == 0)return -1;
    return unmatched[0];
}

int turn = 0;

void make_move(int i){
    cout << i+1 << "\n";
    fflush(stdout);
    turn = i;
}

bool get_move(){
    int a;
    cin >> a;
    if(a == 0)return 1;
    turn = a-1;
    return 0;
}

int main() {
    cin >> n >> m;
    rep(c1,0,m){
        int a,b;
        cin >> a >> b;
        a--;
        b--;
        if(a > b)swap(a,b);
        C[a].push_back(b);
        C[b].push_back(a);
        if(a == 0)friend1[b] = 1;
    }
    mark[0] = 1;

    trav(y, C[0]){
        if(!mark[y]){
            if(dfs(y) != -1){
                cout << "NO\n";
                return 0;
            }
        }
    }

    cout << "YES\n";
    while(!get_move()){
        int j = turn;
        trav(y, match_path[j]){
            make_move(y);
        }
        make_move(0);
    }
    return 0;
}
