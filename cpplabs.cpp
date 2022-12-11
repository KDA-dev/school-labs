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
#include<stack>
#include<random>
#include<optional>
#include <windows.h>
using namespace std;
typedef long long ll;
#define mp make_pair
//const int INF = 2e9 + 9;

template <typename K, typename D>
class bst
{
    struct node {
        K key;
        D data;
        node* left = nullptr, * right = nullptr;
    } *root = nullptr;
    size_t count = 0;
    node** _find(const K& k) const;
    size_t _height_rec(node* subroot) const;
public:
    bst() {};
    ~bst();
    std::optional<D> find(const K& k) const;
    bool insert(const K& k, const D& d);
    bool remove(const K& k);
    std::optional<K> findnext(const K& k) const;
    std::vector<std::pair<K, D>> dump() const;
    size_t size() const { return count; }
    size_t height() const;
    size_t width() const;
    size_t height_rec() { return root ? _height_rec(root) - 1 : 0; }
    void visit(std::function<void(K, D)> worker);
};

template<typename K, typename D>
typename bst<K, D>::node** bst<K, D>::_find(const K& k) const
{
    node** p = (node**)&root;
    while (*p && (*p)->key != k)
        p = (*p)->key < k ? &((*p)->right) : &((*p)->left);
    return p;
}

template<typename K, typename D>
size_t bst<K, D>::_height_rec(node* subroot) const
{
    return subroot ? 1 + max(_height_rec(subroot->left), _height_rec(subroot->right)) : 0;
}

template<typename K, typename D>
bst<K, D>::~bst()
{
    std::stack<node*> s;
    node* cur = root;
    while (s.size() || cur)
    {
        if (cur)
        {
            s.push(cur);
            cur = cur->left;
        }
        else
        {
            cur = s.top()->right;
            delete s.top();
            s.pop();
        }
    }
}

template<typename K, typename D>
std::optional<D> bst<K, D>::find(const K& k) const
{
    node** p = _find(k);
    return (*p) ? std::optional<D>((*p)->data) : std::nullopt;
}

template<typename K, typename D>
bool bst<K, D>::insert(const K& k, const D& d)
{
    node** p = _find(k);
    if (*p)
    {
        (*p)->data = d;
        return false;
    }
    else
    {
        *p = new node{ k,d };
        ++count;
        return true;
    }
}

template<typename K, typename D>
bool bst<K, D>::remove(const K& k)
{
    auto p = _find(k);
    if (*p == nullptr)
        return false;
    if ((*p)->left == nullptr || (*p)->right == nullptr)
    {
        auto n = *p;
        *p = (*p)->left ? (*p)->left : (*p)->right;
        delete n;
    }
    else
    {
        node** cur = &((*p)->right);
        while ((*cur)->left)
            cur = &((*cur)->left);
        node* n = *cur;
        (*p)->key = n->key;
        (*p)->data = n->data;
        *cur = n->right;
        delete n;
    }
    --count;
    return true;
}

template<typename K, typename D>
std::optional<K> bst<K, D>::findnext(const K& k)const
{
    node** p = (node**)&root, ** ll = nullptr;
    while (*p && (*p)->key != k)
        p = (*p)->key < k ? &((*p)->right) : (ll = p, &((*p)->left));

    //if (*p == nullptr)
    //    return std::nullopt;
    if ((*p) == nullptr || (*p)->right == nullptr)
        return ll ? std::optional<K>((*ll)->key) : std::nullopt;
    else
    {
        node* cur = (*p)->right;
        while (cur->left)
            cur = cur->left;
        return cur->key;
    }
}

template<typename K, typename D>
std::vector<std::pair<K, D>> bst<K, D>::dump() const
{
    std::vector<std::pair<K, D>> result;
    result.reserve(this->size());

    std::stack<node*> s;
    node* cur = root;
    while (s.size() || cur)
    {
        if (cur)
        {
            s.push(cur);
            cur = cur->left;
        }
        else
        {
            cur = s.top()->right;
            //delete s.top();
            result.push_back({ s.top()->key,s.top()->data });
            s.pop();
        }
    }
    return result;
}

template<typename K, typename D>
size_t bst<K, D>::height() const
{
    std::stack<std::pair<node*, size_t>> s;
    node* cur = root;
    size_t cur_height = 0, max_height = 0;
    while (s.size() || cur)
    {
        if (cur)
        {
            s.push({ cur,cur_height });
            cur = cur->left;
            ++cur_height;
        }
        else
        {
            max_height = max(max_height, s.top().second);
            cur = s.top().first->right;
            cur_height = s.top().second + 1;
            //delete s.top();
            s.pop();
        }
    }
    return max_height;
}

template<typename K, typename D>
size_t bst<K, D>::width() const
{
    std::stack<std::pair<node*, int>> s;
    node* cur = root;
    int cur_balance = 0, max_balance = 0, min_balance = 0;
    while (s.size() || cur)
    {
        if (cur)
        {
            s.push({ cur,cur_balance });
            cur = cur->left;
            --cur_balance;
        }
        else
        {
            max_balance = max(max_balance, s.top().second);
            min_balance = min(min_balance, s.top().second);
            cur = s.top().first->right;
            cur_balance = s.top().second + 1;
            s.pop();
        }
    }
    return max_balance - max_balance;
}

template <typename K, typename D>
void bst<K, D>::visit(std::function<void(K, D)> worker) {
    stack<node*> s;
    node* cur = root;
    while (s.size() || cur)
    {
        if (cur)
        {
            s.push(cur);
            cur = cur->left;
        }
        else
        {
            worker(s.top()->K, s.top()->D);
            cur = s.top().first->right;
            s.pop();
        }
    }
}

class matrix
{
    std::vector<std::vector<double>> m;
public:
    matrix() {};
    matrix(size_t rows, size_t cols) { m.assign(rows, std::vector<double>(cols)); }
    matrix(size_t n) { m.assign(n, std::vector<double>(n)); }
    matrix(const std::vector<double>& diag);
    static matrix unit_matrix(size_t n);
    const std::vector<double>& operator[](size_t row) const;
    std::vector<double>& operator[](size_t row);
    size_t rows() const { return m.size(); }
    size_t cols() const { return m.size() ? m[0].size() : 0; }
    size_t size() const { return m.size(); }
    matrix operator+(const matrix& arg2) const;
    matrix operator-(const matrix& arg2) const;
    matrix operator*(const matrix& arg2) const;
    matrix operator^(int n) const;
    matrix transpose() const;
    matrix inverse() const;
    double det() const;
    void generate(std::function<double(int, int)> gen);
    friend ostream& operator<<(ostream& o, const matrix& M);
};

double matrix::det() const
{
    if (cols() != rows())
        throw std::string("Row's count and column's count are distinct");
    matrix M{ *this };
    double result = 1;
    size_t n = M.size();
    for (int i = 0; i < n; ++i)
    {
        /*if (abs(M[i][i]) <= 1e-8) {
            swap(M[i], *find_if(M.m.begin() + i, M.m.end(), [&i](const vector<double>& v) -> bool { return abs(v[i]) > 1e-8; }));
        }*/
        double mii = M[i][i];
        std::transform(M[i].begin() + i, M[i].end(), M[i].begin() + i,
            [&mii](auto el) {return el / mii; });
        for (int j = i + 1; j < n; ++j)
        {
            double mji = M[j][i];
            std::transform(M[i].begin() + i, M[i].end(), M[j].begin() + i,
                M[j].begin() + i, [&mji](auto a, auto b) {return b - mji * a; });
        }
        result *= mii;
    }
    return result;
}

matrix matrix::inverse() const
{
    if (cols() != rows())
        throw std::string("Row's count and column's count are distinct");
    matrix M{ *this };
    size_t n = M.size();
    for (int i = 0; i < n; i++) {
        M[i].resize(n * 2, 0);
        M[i][n + i] = 1;
    }
    int zz = 0;
    for (int i = 0; i < n; ++i)
    {
        if (abs(M[i][i]) <= 1e-8) {
            swap(M[i], *find_if(M.m.begin() + i, M.m.end(), [&i](const vector<double>& v) -> bool { return abs(v[i]) > 1e-8; }));
        }
        double mii = M[i][i];
        std::transform(M[i].begin(), M[i].end(), M[i].begin(),
            [&mii](auto el) {return el / mii; });
        for (int j = 0; j < n; ++j)
        {
            if (j == i)
                continue;
            double mji = M[j][i];
            std::transform(M[i].begin(), M[i].end(), M[j].begin(),
                M[j].begin(), [&mji](auto a, auto b) {return b - mji * a; });
        }
    }
    int z = 0;
    for (int i = 0; i < n; i++) {
        move(M[i].begin() + n, M[i].end(), M[i].begin());
        M[i].resize(n);
    }
    return M;
}

void matrix::generate(std::function<double(int, int)> gen)
{
    for (size_t i = 0; i < rows(); i++)
    {
        for (size_t j = 0; j < cols(); j++)
        {
            m[i][j] = gen(i, j);
        }
    }
}

matrix::matrix(const std::vector<double>& diag)
{
    m.assign(diag.size(), std::vector<double>(diag.size()));
    for (size_t i = 0; i < diag.size(); i++)
    {
        m[i][i] = diag[i];
    }
}

matrix matrix::unit_matrix(size_t n)
{
    return matrix(std::vector<double>(n, 1));
}

const std::vector<double>& matrix::operator[](size_t row) const
{
    return m[row];
}

std::vector<double>& matrix::operator[](size_t row)
{
    return m[row];
}

ostream& operator<<(ostream& o, const matrix& M) {
    for (auto& v : M.m) {
        for (auto& a : v)
            o << a << ' ';
        o << '\n';
    }
    return o;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    matrix M(3);
    M.generate([](int r, int c) -> double {return r + c + 1; });
    cout << M.det();
}
