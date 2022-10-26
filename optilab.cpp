#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<functional>
using namespace std;

struct Point {
    double x, y;
    double selfdot() const {
        return x * x + y * y;
    }
    double len() const {
        return sqrt(selfdot());
    }
    Point operator+(const Point& other) const {
        return { x + other.x, y + other.y };
    }
    Point operator-(const Point& other) const {
        return { x - other.x, y - other.y };
    }
    Point operator*(double other) const {
        return { x * other, y * other };
    }
};

double f_10(const Point& p) {
    return exp(p.x * p.x - 2 * p.x) + p.x * p.x + p.y * p.y + exp(p.y * p.y + 3 * p.y);
}
Point grad_10(const Point& p) {
    double x1 = exp(p.x * p.x - 2 * p.x) * (2 * p.x - 2) + 2 * p.x;
    double x2 = 2 * p.y + exp(p.y * p.y + 3 * p.y) * (2 * p.y + 3);
    return { x1, x2 };
}

const double eps = 0.01, gamma = 0.5, theta = 0.5, alph = 0.1, dif = 0.002;

double dichotomy(double lb, double rb, function<double(double)> f) {
    while (rb - lb > eps) {
        double mb1 = lb + (rb - lb) / 2 - dif, mb2 = lb + (rb - lb) / 2 + dif;
        if (f(mb1) <= f(mb2))
            rb = mb2;
        else
            lb = mb1;
    }
    return lb + (rb - lb) / 2;
}

Point gradmethod(const Point& start, function<double(Point)> f, function<Point(Point)> grad) {
    Point xk = start;
    int k = 0;
    while (true) {
        Point gxk = grad(xk);
        if (gxk.selfdot() <= eps)
            break;

        double ak = dichotomy(0, alph, 
            [xk, f, grad](double t) {
                return f(xk - grad(xk) * t);
            });
        xk = xk - gxk * ak;
        k++;
    }
    cout << k << '\n';
    return xk;
}



int main()
{
    auto ans = gradmethod({ 0, 0 }, f_10, grad_10);
    cout << ans.x << ' ' << ans.y;
}
