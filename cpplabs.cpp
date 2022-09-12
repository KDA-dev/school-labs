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
//const int INF = 2e9 + 9;
#define _DEBUGs

struct point {
    double x, y;

    point operator+(const point &other) const {
        return { x + other.x, y + other.y };
    }
    point operator-(const point &other) const {
        return { x - other.x, y - other.y };
    }
    point operator*(const int &other) const {
        return { x * other, y * other };
    }
    const double len() {
        return sqrt(x * x + y * y);
    }
};

struct circle {
    point center;
    double radius;
};

double triangle_area(point p1, point p2, point p3) {
    double seg_len1 = (p1 - p2).len(), seg_len2 = (p2 - p3).len(), seg_len3 = (p1 - p3).len();

    double hp = (seg_len1 + seg_len2 + seg_len3) / 2;
    return sqrt(hp * (hp - seg_len1) * (hp - seg_len2) * (hp - seg_len3));
}

double polygon_perimeter(vector<point> &vertices) {
    double ans = (vertices[0] - vertices[vertices.size()]).len();
    for (int i = 1; i < vertices.size(); i++) {
        ans += (vertices[i - 1] - vertices[i]).len();
    }
    return ans;
}

point bisector(point vertex, point side1, point side2) {
    point vector1 = side1 - vertex;
    point vector2 = side2 - vertex;
    return vector1 * vector2.len() + vector2 * vector1.len();
}

enum class overlap_status {
    INTERSECT, TOUCH, SEPARATE
};

overlap_status circles_overlap(circle a, circle b) {
    double distance_indicator = (a.center - b.center).len() - a.radius - b.radius;
    if (distance_indicator > 0)
        return overlap_status::SEPARATE;
    if (distance_indicator < 0)
        return overlap_status::INTERSECT;
    return overlap_status::TOUCH;
}

double polygon_area(vector<point>& vertices) {
    double ans = 0;
    for (int i = 2; i < vertices.size(); i++) {
        ans += triangle_area(vertices[0], vertices[i - 1], vertices[i]);
    }
    return ans;
}

int main() {
    //ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
   // freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
#endif
    
}
