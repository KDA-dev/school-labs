#include<iostream>
#include<vector>
#include<iomanip>
#include<map>
#include<cmath>
#include<string>
#include<set>
#include<queue>
#include<algorithm>
using namespace std;
typedef long long ll;
#define mp make_pair
const int INF = 2e9+9;
#define _DEBUGs

struct point {
    double x, y;
};

double seg_len(point p1, point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p2.y - p2.y) * (p2.y - p2.y));
}

double triangle_area(point p1, point p2, point p3) {
    double hp = (seg_len(p1, p2) + seg_len(p2, p3) + seg_len(p1, p3)) / 2;
    return sqrt(hp * (hp - seg_len(p1, p2)) * (hp - seg_len(p2, p3)) * (hp - seg_len(p1, p3)));
}

double ov_area(point seg1a, point seg1b, point seg2a, point seg2b) {
    double ov_width, ov_height;

    if(max(seg1a.x, seg1b.x) > min(seg2a.x, seg2b.x) && min(seg1a.x, seg1b.x) < max(seg2a.x, seg2b.x))
        ov_width = max(seg1a.x, seg1b.x) - min(seg2a.x, seg2b.x);
    else if (max(seg2a.x, seg2b.x) > min(seg1a.x, seg1b.x) && min(seg2a.x, seg2b.x) < max(seg1a.x, seg1b.x))
        ov_width = max(seg2a.x, seg2b.x) - min(seg1a.x, seg1b.x);
    else
        return 0;

    if(max(seg1a.y, seg1b.y) > min(seg2a.y, seg2b.y) && min(seg1a.y, seg1b.y) < max(seg2a.y, seg2b.y))
        ov_height = max(seg1a.y, seg1b.y) - min(seg2a.y, seg2b.y);
    else if (max(seg2a.y, seg2b.y) > min(seg1a.y, seg1b.y) && min(seg2a.y, seg2b.y) < max(seg1a.y, seg1b.y))
        ov_height = max(seg2a.y, seg2b.y) - min(seg1a.y, seg1b.y);
    else
        return 0;

    return ov_width * ov_height;
}

bool are_multiplications_equal(int mul1_factor1, int mul1_factor2, int mul2_factor1, int mul2_factor2) {
    if((mul1_factor1 == 0 || mul1_factor2 == 0) && (mul2_factor1 == 0 || mul2_factor2 == 0))
        return true;
    if(mul1_factor1 == 0 || mul1_factor2 == 0 || mul2_factor1 == 0 || mul2_factor2 == 0)
        return false;
    
    bool equal_remainders = are_multiplications_equal(mul1_factor2, mul1_factor1 % mul2_factor2, mul2_factor2, mul2_factor1 % mul1_factor2);
    return mul1_factor1 / mul2_factor2 == mul2_factor1 / mul1_factor2 && equal_remainders;
}

bool sum_equal_to(int summand1, int summand2, int sum_query) {
    if(summand1 >=0 && summand2 <=0 || summand1 <=0 && summand2 >= 0)
        return summand1 + summand2 == sum_query;
    if(summand1 >=0 && sum_query <=0 || summand1 <=0 && sum_query >= 0)
        return false;
    return sum_query - summand1 == summand2;
}

int main() {
    //ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    cout << are_multiplications_equal(7907 * 7901, 7919, 7907, 7901 * 7919);
}
