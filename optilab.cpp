#include <iostream>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<functional>
using namespace std;

struct Point {
    vector<double> coord;
    double selfdot() const {
        return *this * *this;
    }
    double len() const {
        return sqrt(selfdot());
    }
    Point operator+(const Point& other) const {
        vector<double> _coord;
        for (int i = 0; i < coord.size(); i++)
            _coord.push_back(coord[i] + other.coord[i]);
        return { _coord };
    }
    Point operator-(const Point& other) const {
        vector<double> _coord;
        for (int i = 0; i < coord.size(); i++)
            _coord.push_back(coord[i] - other.coord[i]);
        return { _coord };
    }
    Point operator*(double other) const {
        vector<double> _coord;
        for (int i = 0; i < coord.size(); i++)
            _coord.push_back(coord[i] * other);
        return { _coord };
    }
    double operator*(const Point& other) const {
        double res = 0;
        for (int i = 0; i < coord.size(); i++)
            res += coord[i] * other.coord[i];
        return res;
    }
    Point operator*(const vector<Point>& matrix) const {
        vector<double> _coord;
        for (int i = 0; i < coord.size(); i++)
            _coord.push_back(*this * matrix[i]);
        return { _coord };
    }
};
typedef vector<Point> matrix;

struct Fn {
    double operator() (const Point& x) const {
        return exp(x.coord[0] * x.coord[0] - 2 * x.coord[0]) + x.coord[0] * x.coord[0] 
            + x.coord[1] * x.coord[1] + exp(x.coord[1] * x.coord[1] + 3 * x.coord[1]);
    }
    Point grad(const Point& x) const {
        double x1 = exp(x.coord[0] * x.coord[0] - 2 * x.coord[0]) * (2 * x.coord[0] - 2) + 2 * x.coord[0];
        double x2 = 2 * x.coord[1] + exp(x.coord[1] * x.coord[1] + 3 * x.coord[1]) * (2 * x.coord[1] + 3);
        return { {x1, x2} };
    }
    matrix H_rev(const Point& x) const {
        matrix res(2);
        double ex = exp(x.coord[0] * x.coord[0] - 2 * x.coord[0]);
        double ey = exp(x.coord[1] * x.coord[1] + 3 * x.coord[1]);
        res[0] = { {ex * (2 * x.coord[0] - 2) * (2 * x.coord[0] - 2) + 2 * ex + 2, 0} };
        res[1] = { {0, 2 + ey * (2 * x.coord[1] + 3) * (2 * x.coord[1] + 3) + 2 * ey} };

        double det = res[0].coord[0] * res[1].coord[1];
        for (auto& a : res)
            a = a * (1. / det);
        swap(res[0].coord[0], res[1].coord[1]);
        return res;
    }
};

matrix matmult(const matrix& a, const matrix& b) {
    matrix res(a.size(), { vector<double>(b[0].coord.size(), 0) });
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b[0].coord.size(); j++)
            for (int k = 0; k < b.size(); k++)
                res[i].coord[j] += a[i].coord[k] * b[k].coord[j];
    return res;
}

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

Point new_ton(const Point& start, const Fn& f) {
    Point xk = start;
    int k = 0;
    while (f.grad(xk).len() > eps) {
        matrix Hrev = f.H_rev(xk);

        double ak = dichotomy(0, maxalph,
            [&xk, &f, &Hrev](double t) {
                return f(xk - f.grad(xk) * Hrev * t);
            });
        xk = xk - f.grad(xk) * Hrev * ak;
        k++;
    }
    cout << k << '\n';
    return xk;
}

int main()
{
    Fn f10;
    auto ans = new_ton({ {0, -5} }, f10);
    cout << ans.coord[0] << ' ' << ans.coord[1] << '\n';
    cout << f10(ans);
}
