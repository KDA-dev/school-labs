#include <iostream>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<functional>
using namespace std;

struct Point {
    double x, y, z;
    double selfdot() const {
        return *this * *this;
    }
    double len() const {
        return sqrt(selfdot());
    }
    Point operator+(const Point& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }
    Point operator-(const Point& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }
    Point operator*(double other) const {
        return { x * other, y * other, z * other };
    }
    double operator*(const Point& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    Point operator*(const vector<Point>& matrix) const {
        vector<double> coord;
        for (int i = 0; i < 3; i++)
            coord.push_back(*this * matrix[i]);
        return { coord[0], coord[1], coord[2] };
    }
};

struct sqrFn {
    vector<Point> A; // matrix
    Point b;
    double c;
    double operator() (const Point& x) const {
        return x * A * x * 0.5 + x * b + c;
    }
    Point grad(const Point& x) const {
        return x * A + b;
    }
};

const double maxalph = 1000, eps = 0.01, dif = 0.002;

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

Point FletcherReeves(const Point& start, const sqrFn& f) {
    Point xk = start, xprev = { 1, 1, 1 }, dk = { 0, 0, 0 };
    int k = 0;
    while (f.grad(xk).len() > eps) {
        double wprev = f.grad(xk).selfdot() / f.grad(xprev).selfdot();
        dk = f.grad(xk) + dk * wprev;

        double ak = dichotomy(0, maxalph,
            [&xk, &f, &dk](double t) {
                return f(xk - dk * t);
            });
        xprev = xk;
        xk = xk - dk * ak;
        k++;
    }
    cout << k << '\n';
    return xk;
}

int main()
{
    sqrFn f10 = {
        {
            {2, 1.5, 1},
            {1.5, 2, 1},
            {1, 1, 1}
        },
        {1, 2, 3},
        1
    };
    auto ans = FletcherReeves({ 1, 1, 1 }, f10);
    cout << ans.x << ' ' << ans.y << ' ' << ans.z << '\n';
    cout << f10(ans);
}
