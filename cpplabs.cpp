#include<iostream>
#include<vector>
#include<iomanip>
#include<map>
#include<cmath>
#include<string>
#include<set>
#include<queue>
#include<numeric>
#include<algorithm>
using namespace std;
typedef long long ll;
#define mp make_pair
//const int INF = 2e9 + 9;
#define _DEBUG

double eps = 1e-7;

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
    point operator/(const int &other) const {
        return { x / other, y / other };
    }
    double operator*(const point &other) const {
        return x * other.x + y * other.y;
    }
    double operator^(const point &other) const {
        return x * other.y - other.x * y;
    }
    double len() const {
        return sqrt(x * x + y * y);
    }
    point norm() const {
        return {x / len(), y / len()};
    }
};

struct line_segment {
    point a, b;
};

double oriented_triangle_area(point p1, point p2, point p3) {
    return ((p2 - p1) ^ (p3 - p2)) / 2.;
}

bool lines_intersect(line_segment l1, line_segment l2) { // line is defined by a segment on it
    return oriented_triangle_area(l1.a, l1.b, l2.a) * oriented_triangle_area(l1.a, l1.b, l2.b) <= 0
    && oriented_triangle_area(l2.a, l2.b, l1.a) * oriented_triangle_area(l2.a, l2.b, l1.b) <= 0;
}
bool bounding_box_check(line_segment s1, line_segment s2) {
    return max(min(s1.a.x, s1.b.x), min(s2.a.x, s2.b.x)) <= min(max(s1.a.x, s1.b.x), max(s2.a.x, s2.b.x))
        && max(min(s1.a.y, s1.b.y), min(s2.a.y, s2.b.y)) <= min(max(s1.a.y, s1.b.y), max(s2.a.y, s2.b.y));
}

bool line_segments_intersect(line_segment s1, line_segment s2) {
    return lines_intersect(s1, s2) && bounding_box_check(s1, s2);
}
bool segment_ray_intersect(line_segment s, point ray_start, point ray_dir) {
    double max_distance_to_intersection_point = max((s.a - ray_start).len(), (s.b - ray_start).len());
    return line_segments_intersect(s, {ray_start, ray_start + ray_dir.norm() * max_distance_to_intersection_point * 2});
}

bool is_point_in_polygon(point p, const vector<point> &polygon) {
    point ray_dir = {1, 1};
    int intersection_cnt = 0;
    for(int i = 0; i < polygon.size() - 1; i++) {
        double seglen_reduced = (polygon[i + 1] - polygon[i]).len() - eps;
        point open_segment_point = polygon[i] + (polygon[i + 1] - polygon[i]).norm() * seglen_reduced;
        //This essentially removes the segment's second point, preventing cases where
        //the same point is counted twice 
        if(segment_ray_intersect({polygon[i], open_segment_point}, p, ray_dir))
            intersection_cnt++;
    }
    return intersection_cnt % 2 != 0;
}

struct rectangle {
    point a, b;
    rectangle norm() const {
        return {{min(a.x, b.x), min(a.y, b.y)}, {max(a.x, b.x), max(a.y, b.y)}};
    }
    double area() const {
        return (norm().b.x - norm().a.x) * (norm().b.y - norm().a.y);
    }
    rectangle overlap(const rectangle &other) const {
        point anew = {max(norm().a.x, other.norm().a.x), max(norm().a.y, other.norm().a.y)};
        point bnew = {min(norm().b.x, other.norm().b.x), min(norm().b.y, other.norm().b.y)};
        return {anew, bnew};
    }
};

double multiple_rectangles_area_overlap(const vector<rectangle> &rs) {
    rectangle cur = rs[0];
    for(int i = 1; i < rs.size(); i++) {
        cur = cur.overlap(rs[i]);
        if(cur.a.x >= cur.b.x || cur.a.y >= cur.b.y)
            return 0;
    }
    return cur.area();
}
double multiple_rectangles_area_union(const vector<rectangle> &rs) {
    // I'm
    // I'm not writing a scanline for this
    return 0;
}


int main() {
    //ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
   // freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
#endif
    
}
