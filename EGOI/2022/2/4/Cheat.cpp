#include <cstdio>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include "cheat.h"

using namespace std;

typedef pair<int, int> Point;
typedef pair<Point, Point> Segment;

const int kD = 2;

Segment Normalize(Segment x) {
    if (x.first > x.second) {
        swap(x.first, x.second);
    }
    return x;
}

Point Rotate(Point x, int d) {
    return make_pair(d - x.second, x.first);
}

Segment RotateAndNormalize(Segment x, int d) {
    return Normalize(make_pair(Rotate(x.first, d), Rotate(x.second, d)));
}

// A segment is valid for writing iff it does not contain another point on the grid.
// That is, if the gcd between the x and y coordinate difference of its endpoints is 1.
bool IsValid(Segment s) {
    int x = abs(s.first.first - s.second.first);
    int y = abs(s.first.second - s.second.second);

    if (x == 0 || y == 0) {
        return (x == 1 || y == 1);
    }
    while (x % y) {
        int r = x % y;
        x = y;
        y = r;
    }
    return y == 1;
}

void PrintSegment(Segment x) {
    printf("%d %d %d %d\n", x.first.first, x.first.second, x.second.first, x.second.second);
}

// For a given grid size d, returns the sets of segments that can be obtained from one another by 90 degree rotations.
vector<vector<Segment>> GetSegmentGroups(int d) {
    vector<vector<Segment>> groups;
    map<Segment, bool> used;
    for(int x1 = 0; x1 <= d; ++x1) {
        for (int y1 = 0; y1 <= d; ++y1) {
            for (int x2 = 0; x2 <= d; ++x2) {
                for (int y2 = 0; y2 <= d; ++y2) {
                    Point p1 = make_pair(x1, y1);
                    Point p2 = make_pair(x2, y2);
                    if (p1 == p2) {
                        continue;
                    }
                    Segment s = Normalize(make_pair(p1, p2));
                    if (used[s] || !IsValid(s)) {
                        continue;
                    }
                    vector<Segment> current;
                    for (int i = 0; i < 4; ++i) {
                        current.push_back(s);
                        used[s] = true;
                        s = RotateAndNormalize(s, d);
                    }
                    groups.push_back(current);
                }
            }
        }
    }

    return groups;
}

const vector<int> ordered_config[3] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}};
const vector<int> unordered_config[4] = {{0, 0, 0, 0}, {0, 1, 0, 1}, {1, 0, 1, 0}, {1, 1, 1, 1}};


void CountColourings(long long dp[], long long u[], int c) {
    // dp[i] - the number of different rotation-isomorphic colourings we can obtain with the first i groups of segments
    // u[i] - the number of ways we can colour the first i groups of segments = 2 ^ (4 * i) = 16 ^ i.
    // dp[i] = 4 * dp[i - 1] + 3 * 2 ^ (4 * (i - 1))
    // 4 * dp[i - 1] -> We colour the current class either 0000, 0101, 1010 or 1111 (4) and we establish the correct orientation from a previous class (dp[i - 1])
    // 3 * u[i-1] - > We colour the current class either 0001, 0011 or 0111 (3) so we can establish the correct orientation in the current class. The segments in the previous classes we can then colour in all possible ways u[i-1]
    dp[0] = 0;
    u[0] = 1;
    for (int i = 1; i <= c; ++i) {
        dp[i] = 4 * dp[i - 1] + 3 * u[i - 1];
        u[i] = 16 * u[i - 1];
    }
}

void ColorGroup(const vector<Segment>& group, const vector<int>& config, vector<Segment>& solution) {
    for(int i = 0; i < 4; ++i) {
        if (config[i]) {
            solution.push_back(group[i]);
        }
    }
}

void ColorRemainingGroups(const vector<vector<Segment>>& groups, int position, int n, vector<Segment>& solution) {
    for (int i = 0; i <= position; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (n % 2) {
                 solution.push_back(groups[i][j]);
            }
            n /= 2;
        }
    }
}

// Code number n into a segment set on a dxd grid.
vector<Segment> BuildPattern(int n) {
    int d = 2;
    --n;
    vector<vector<Segment>> groups = GetSegmentGroups(d);
    int c = groups.size();
    long long dp[c + 2], u[c + 2];
    CountColourings(dp, u, c);
    vector<Segment> solution;
    for (int i = c - 1; i >= 0; --i) {
        int ordered = 0;
        while (n >= u[i] && ordered < 3) {
            ++ordered;
            n -= u[i];
        }
        if (ordered < 3) {
            ColorGroup(groups[i], ordered_config[ordered], solution);
            ColorRemainingGroups(groups, i-1, n, solution);
            break;
        }
        int unordered = 0;
        while (n >= dp[i] && unordered < 4) {
            ++unordered;
            n -= dp[i];
        }
        ColorGroup(groups[i], unordered_config[unordered], solution);
    }
    return solution;
}

int WhichOrderedConfig(const vector<Segment>& group, const set<Segment>& figure) {
    vector<int> config;
    for (int i = 0; i < 4; ++i) {
        config.push_back(figure.find(group[i]) != figure.end());
    }
    for (int i = 0; i < 3; ++i) {
        if (ordered_config[i] == config) {
            return i;
        }
    }
    return -1;
}

int WhichUnorderedConfig(const vector<Segment>& group, const set<Segment>& figure) {
    vector<int> config;
    for (int i = 0; i < 4; ++i) {
        config.push_back(figure.find(group[i]) != figure.end());
    }
    for (int i = 0; i < 4; ++i) {
        if (unordered_config[i] == config) {
            return i;
        }
    }
    return -1;
}

set<Segment> FindGoodRotation(const vector<vector<Segment>>& groups, int d, vector<Segment>& segments) {
    int best = -1, best_step = -1;
    for (int step = 0; step < 4; ++step) {
        set<Segment> figure;
        for (const Segment& s : segments) {
            figure.insert(s);
        }
        bool good_rotation = false;
        for (int i = groups.size() - 1; i >= 0; --i) {
            if (WhichOrderedConfig(groups[i], figure) != -1) {
                if (i > best) {
                    best_step = step;
                    best = i;
                    break;
                }
            }
        }
        for (int i = 0; i < segments.size(); ++i) {
            segments[i] = RotateAndNormalize(segments[i], d);
        }
    }
    for (int step = 0; step < best_step; ++step) {
        for (int i = 0; i < segments.size(); ++i) {
            segments[i] = RotateAndNormalize(segments[i], d);
        }
    }
    set<Segment> figure;
    for (const Segment& s : segments) {
        figure.insert(s);
    }
    return figure;
}


int AddedSumFromRemainingGroups(const vector<vector<Segment>>& groups, int position, const set<Segment>& figure) {
    int p2 = 1;
    int sum = 0;
    for (int i = 0; i <= position; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (figure.find(groups[i][j]) != figure.end()) {
                 sum += p2;
            }
            p2 *= 2;
        }
    }
    return sum;
}

int GetCardNumber(vector<Segment> segments) {
    int d = 2;
    for (int i = 0; i < segments.size(); ++i) {
        segments[i] = Normalize(segments[i]);
    }
    vector<vector<Segment>> groups = GetSegmentGroups(d);
    int c = groups.size();
    long long dp[c + 2], u[c + 2];
    CountColourings(dp, u, c);
    set<Segment> figure = FindGoodRotation(groups, d, segments);

    int solution = 0;
    for (int i = c - 1; i >= 0; --i) {
        int ordered = WhichOrderedConfig(groups[i], figure);
        if (ordered != -1) {
            solution += ordered * u[i];
            solution += AddedSumFromRemainingGroups(groups, i-1, figure);
            break;
        }
        solution += 3 * u[i];
        int unordered = WhichUnorderedConfig(groups[i], figure);
        solution += unordered * dp[i];
    }
    ++solution;
    return solution;
}
