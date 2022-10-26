#include <bits/stdc++.h>
#include "socialengineering.h"
using namespace std;
const int N = 2e5 + 5;
vector <int> edge[N];
vector <int> _edge[N];
vector <vector<int>> components;
vector <int> temp;

set <pair<int,int>> used;
bool usedNeighbour[N];
bool isNeighbour[N];
int component[N];
int depth[N];
int vis[N];
int parent[N];
int lca[N];
int match[N];
int nComponenets = 0;
int nNeighbours = 0;

void dfs(int node,int back) {
    if (vis[node] == 1)
        return;
    if (back != 0) {
        edge[back].push_back(node);
        edge[node].push_back(back);
    }
    if (isNeighbour[node])
        nNeighbours++;
    vis[node] = 1;
    temp.push_back(node);
    for (auto next : _edge[node])
        dfs(next,node);
}

int pair_node(int node,int back) {
    int unmatched = -1;
    parent[node] = back;
    for (auto next : edge[node]) {
        if (next == back)
            continue;
        depth[next] = depth[node] + 1;
        int unmatched2 = pair_node(next,node);
        if (unmatched2 != -1) {
            if (unmatched == -1)
                unmatched = unmatched2;
            else {
                lca[unmatched] = node;
                lca[unmatched2] = node;
                match[unmatched] = unmatched2;
                match[unmatched2] = unmatched;
                unmatched = -1;
            }
        }
    }
    if (isNeighbour[node]) {
        if (unmatched == -1)
            unmatched = node;
        else {
            int unmatched2 = node;
            lca[unmatched] = node;
            lca[unmatched2] = node;
            match[unmatched] = unmatched2;
            match[unmatched2] = unmatched;
            unmatched = -1;
        }
    }
    return unmatched;
}

void SocialEngineering(int n, int m, vector<pair<int,int>> edges) {
    int count = 0;
	for (auto temp : edges) {
		// edge[temp.first].push_back(temp.second);
		// edge[temp.second].push_back(temp.first);
		if (temp.first == 1) {
			isNeighbour[temp.second] = true;
            count++;
			continue;
		}
		if (temp.second == 1) {
			isNeighbour[temp.first] = true;
            count++;
			continue;
		}
		_edge[temp.first].push_back(temp.second);
		_edge[temp.second].push_back(temp.first);

	}
	
    if (count % 2 == 1)
		return;

	for (int i = 2 ; i <= n ; i++) {
		if (vis[i] == 0) {
			temp.clear();
            nNeighbours = 0;
            dfs(i,0);
            sort(temp.begin(),temp.end());
            components.push_back(temp);
            if (nNeighbours % 2 == 1)
                return;
		}
	}

    for (int i = 0 ; i < components.size() ; i++) {
        pair_node(components[i][0],-1);
    }

    while(true) {
        int x = GetMove();
        if (x == 0)
            return;
        int y = match[x];
        int par = lca[x];
        while(x != par) {
            MakeMove(parent[x]);
            x = parent[x];
        }
        vector <int> nodes;
        while(y != par) {
            nodes.push_back(y);           
            y = parent[y];
        }
        reverse(nodes.begin(),nodes.end());
        for (int next : nodes)
            MakeMove(next);
        MakeMove(1);
    }

}