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
#include<functional>
#include<random>
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

int integer_pts_on_segment(point p1, point p2) {

    int triangle_width = round(abs(p1.x - p2.x));
    int triangle_height = round(abs(p1.y - p2.y));

    return gcd(triangle_width, triangle_height) + 1;
}

double oriented_triangle_area(point p1, point p2, point p3) {
    return ((p2 - p1) ^ (p3 - p2)) / 2.;
}

double polygon_area(const vector<point>& vertices) {
    double ans = 0;
    for (int i = 2; i < vertices.size(); i++) {
        ans += oriented_triangle_area(vertices[0], vertices[i - 1], vertices[i]);
    }
    return ans;
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


template<typename T>
int findElement(const T& elem, const vector<T>& vec) {
    for (int i = 0; i < vec.size(); i++)
        if (elem == vec[i]) // ...C++ cares about safety about as much as a kid sticking his hand into a power socket, huh
            return i;
    return -1; //was not found
}

template<typename T>
int findElementPair(const vector<T>& vec, function<bool(T, T)> equals) {
    for (int i = 0; i < vec.size() - 1; i++)
        if (equals(vec[i + 1], vec[i]))
            return i;
    return -1; //was not found
}

enum sort_status {INCREASING, DECREASING, STATIC, UNSORTED};

template<typename T>
sort_status isSorted(const vector<T>& vec) {
    bool increasing = true, decreasing = true;

    for (int i = 0; i < vec.size(); i++)
        if (v[i + 1] < v[i])
            increasing = false;
    for (int i = 0; i < vec.size(); i++)
        if (v[i] < v[i + 1])
            decreasing = false;

    if (increasing && decreasing)
        return STATIC;
    else if (increasing)
        return INCREASING;
    else if (decreasing)
        return DECREASING;
    else
        return UNSORTED;
}

template<typename T>
bool isSorted(const vector<T>& vec, function<bool(T, T)> partial_order_comp) {
    bool increasing = true, decreasing = true;

    for (int i = 0; i < vec.size(); i++)
        if (!partial_order_comp(v[i], v[i + 1]))
            increasing = false;
    for (int i = 0; i < vec.size(); i++)
        if (!partial_order_comp(v[i + 1], v[i]))
            decreasing = false;

    if (increasing && decreasing)
        return STATIC;
    else if (increasing)
        return INCREASING;
    else if (decreasing)
        return DECREASING;
    else
        return UNSORTED;
}

template<typename T>
int countElement(T* data, int data_size, const T &elem) {
    int cnt = 0;
    for (T* pointer = data, *data_end = data + data_size; p < data_end; p++)
        if (elem == *p) // ...C++ cares about safety about as much as a kid sticking his hand into a power socket, huh
            ++cnt;
    return cnt;
}

template<typename T>
bool isVecPermutationOfOther(const vector<T> &vec, const vector<T> &other) {
    if(vec.size() != other.size())
        return false;
    vector<bool> foundElems(vec.size(), false);
    for(int i = 0; i < vec.size(); i++) {
        bool elem_found = false;
        for(int j = 0; j < other.size(); j++) {
            if(foundElems[i])
                continue;
            if(vec[i] == other[j]) {
                elem_found = true;
                foundElems[j] = true;
                break;
            }
        }
        if(!elem_found)
            return false;
    }
    return true;
}

int integer_pts_in_polygon(const vector<point> &polygon) {
    int boundary_ints = integer_pts_on_segment(polygon.back(), polygon[0]);
    for(int j = 1; j < polygon.size(); j++)
        boundary_ints += integer_pts_on_segment(polygon[j - 1], polygon[j]) - 1;
    
    int area_twice = round(polygon_area(polygon) * 2);

    return (area_twice - boundary_ints + 2) / 2;
    //pick's theorem
}

bool point_in_rectangle(const point &p, const rectangle &r) {
    return p.x <= r.norm().b.x && p.x >= r.norm().a.x && p.y <= r.norm().b.y && p.y >= r.norm().a.y;
}

double inexact_multiple_rectangles_area_union(const vector<rectangle> &rs) {
    point min_bound = rs[0].a, max_bound = rs[0].a;
    for(int i = 0; i < rs.size(); i++) {
        min_bound.x = min(min_bound.x, min(rs[i].a.x, rs[i].b.x));
        min_bound.y = min(min_bound.y, min(rs[i].a.y, rs[i].b.y));
        max_bound.x = max(max_bound.x, max(rs[i].a.x, rs[i].b.x));
        max_bound.y = max(max_bound.y, max(rs[i].a.y, rs[i].b.y));
    }
    mt19937 engine;
    uniform_real_distribution uni(0., 1.);

    double xlen = max_bound.x - min_bound.x, ylen = max_bound.y - min_bound.y;
    double in_cnt = 0;

    for(int i = 0; i < 100000; i++) {
        point d = {uni(engine) * xlen, uni(engine) * ylen};
        if(point_in_rectangle())
    }
}

int main() {
    //ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
   // freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
#endif
    
}
