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
    vector<pair<K, D>>& dump();
    void _wrapper(ostream& o) const;
    bool is_bst_by_data() const;
    template<typename A, typename B>
    friend ostream& operator<<(ostream& o, const bst<A, B>& _bst);
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

template <typename K, typename D>
vector<pair<K, D>>& bst<K, D>::dump() {
    vector<pair<K, D>> v;
    visit([&v](K k, D d)->void {
        v.push_back({ k, d });
        });
}

template <typename K, typename D>
void bst<K, D>::_wrapper(ostream& o) const
{
    stack<pair<node*, size_t>> s;
    node* cur = root;
    size_t cur_height = 0;
    while (s.size() || cur)
    {
        if (cur)
        {
            for (int i = 0; i < cur_height; i++)
                o << "    ";
            o << cur->key << ", " << cur->data << '\n';
            if(cur->left == nullptr && cur->right != nullptr) {
                for (int i = 0; i < cur_height + 1; i++)
                    o << "    ";
                cout << "nil\n";
            }
            s.push({ cur,cur_height });
            cur = cur->left;
            ++cur_height;
        }
        else
        {
            if(s.top().first->left != nullptr && s.top().first->right == nullptr) {
                for (int i = 0; i < s.top().second + 1; i++)
                    o << "    ";
                cout << "nil\n";
            }
            cur = s.top().first->right;
            cur_height = s.top().second + 1;
            //delete s.top();
            s.pop();
        }
    }
}

template <typename K, typename D>
ostream& operator<<(ostream& o, const bst<K, D>& _bst) {
    _bst._wrapper(o);
    return o;
}

template<typename K, typename D>
bool bst<K, D>::is_bst_by_data() const
{
    stack<node*> s;
    node* cur = root;
    bool is_bst = true;
    while (s.size() || cur)
    {
        if (cur)
        {
            s.push(cur);
            if(cur->left) {
                is_bst &= cur->left->data < cur->data;
            }
            cur = cur->left;
        }
        else
        {
            if(s.top()->right) {
                is_bst &= s.top()->data < s.top()->right->data;
            }
            cur = s.top()->right;
            s.pop();
        }
    }
    return is_bst;
}

typedef vector<vector<int>> graph;

void bfs(const graph& G, vector<int>& dist, vector<int>& parent, int start) {
    dist.assign(G.size(), -1);
    parent.assign(G.size(), -1);
    dist[start] = 0;
    queue<int> s;
    s.push(start);
    while(!s.empty()) {
        int v = s.front();
        s.pop();
        for(int u : G[v])
            if(dist[u] == -1) {
                dist[u] = dist[v] + 1;
                parent[u] = v;
                s.push(u);
            }
    }
}

void find_components(const graph& G, vector<int>& comp) {
    comp.assign(G.size(), -1);
    int cur_comp = 0;
    for(int cur = 0; cur < G.size(); cur++) {
        if(comp[cur] != -1)
            continue;
        comp[cur] = cur_comp++;
        queue<int> s;
        s.push(cur);
        while(!s.empty()) {
            int v = s.front();
            s.pop();
            for(int u : G[v]) {
                if(comp[u] == -1) {
                    comp[u] = comp[v];
                    s.push(u);
                }
            }
        }
    }
}

bool check_if_twoparter(const graph& G) {
    vector<int> partmark(G.size(), -1);
    for(int cur = 0; cur < G.size(); cur++) {
        if(partmark[cur] != -1)
            continue;
        partmark[cur] = 0;
        queue<int> s;
        s.push(cur);
        while(!s.empty()) {
            int v = s.front();
            s.pop();
            for(int u : G[v]) {
                if(partmark[u] == -1) {
                    partmark[u] = (partmark[v] + 1) % 2;
                    s.push(u);
                }
                else if (partmark[u] == partmark[v])
                    return false;
            }
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    bst<string, int> bsi;
    bsi.insert("one", 1);
    cout << (bsi.is_bst_by_data() ? "true" : "false") << '\n';
    bsi.insert("five", 5);
    cout << (bsi.is_bst_by_data() ? "true" : "false") << '\n';
    bsi.insert("eight", 8);
    bsi.insert("three", 3);
    bsi.insert("tao", 2);
    bsi.insert("four", 4);
    cout << bsi << (bsi.is_bst_by_data() ? "true" : "false") << '\n';
}
