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
#define _DEBUG

template <typename F>
size_t exec_time(F f)
{ 
    auto start=GetTickCount64(); 
    f(); 
    return GetTickCount64()-start; 
}

int find_total_rainfall(const vector<pair<int, int>>& bricks) { // first = h, second = w
    vector<int> dist(bricks.size());
    dist[0] = bricks[0].second;
    for(int i = 1; i < bricks.size(); i++)
        dist[i] = dist[i - 1] + bricks[i].second;
    int ans = 0;
    stack<int> backwall;
    for(int i = 0; i < bricks.size(); i++) {
        while(!backwall.empty() && bricks[backwall.top()].first < bricks[i].first) {
            int t = backwall.top();
            backwall.pop();
            if(!backwall.empty())
                ans += dist[i - 1] - dist[backwall.top()];
        }
        backwall.push(i);
    }
    return ans;
}

template <typename K, typename D>
class bst
{
    struct node {
        K key;
        D data;
        node* left = nullptr, * right = nullptr;
    } *root = nullptr;
    size_t count = 0;
    //возвращает адрес указателя, по которому должен располагаться ключ k
    node** _find(const K& k) const;
public:
    bst() {};
    ~bst() {};
    std::optional<D> find(const K& k) const;
    bool insert(const K& k, const D& d);
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
optional<D> bst<K, D>::find(const K& k) const
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

template<typename T>
void transfer_top(stack<T>& from, stack<T>& to) {
    to.push(from.top());
    from.pop();
}

template<typename T>
void sort_stack(stack<T>& st) {
    stack<T> sec, buffer;
    while (st.size() != 0)
    {
        while (sec.size() != 0 && sec.top() > st.top())
        {
            buffer.push(sec.top());
            sec.pop();
        }
        sec.push(st.top());
        st.pop();
        while (buffer.size() != 0)
        {
            sec.push(buffer.top());
            buffer.pop();
        }
    }
    while (sec.size() != 0)
    {
        st.push(sec.top());
        sec.pop();
    }
}

template<typename T>
void qsort_stack(stack<T>& st) {
    if(st.empty())
        return;
    stack<T> G, L, E;
    transfer_top(st, E);
    while(!st.empty()) {
        if(st.top() > E.top())
            transfer_top(st, G);
        else if(st.top() < E.top())
            transfer_top(st, L);
        else
            transfer_top(st, E);
    }
    qsort_stack(G);
    qsort_stack(L);
    while(!L.empty())
        transfer_top(L, E);
    while(!E.empty())
        transfer_top(E, st);
    while(!G.empty())
        transfer_top(G, E);
    while(!E.empty())
        transfer_top(E, st);
}

random_device rd;
mt19937 mt(rd());

template<typename T>
void fill_with_random(stack<T>& st, int amnt) {
    while(!st.empty())
        st.pop();
    for(int i = 0; i < amnt; i++) {
        st.push(mt());
    }
}

void test_timing() {
    stack<int> st;
    for(int i = 10; i <= 50; i += 10) {
        cout << "Using " << i << " elements:\n";
        fill_with_random(st, i);
        cout << "Boring sort: " << exec_time([&st]{sort_stack(st);}) << '\n';
        fill_with_random(st, i);
        cout << "Boring sort: " << exec_time([&st]{qsort_stack(st);}) << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    
    test_timing();
}
