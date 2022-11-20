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
using namespace std;
typedef long long ll;
#define mp make_pair
//const int INF = 2e9 + 9;
#define _DEBUG

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

class node {  
public:
    node* L, *R;
    int key, data;
    node(int _key, int _data) {
        L = R = nullptr;
        key = _key;
        data = _data;
    }
};

node* find(int key, node* start) {
    while(start != nullptr && start->key != key) {
        if(key < start->key)
            start = start->L;
        else
            start = start->R;
    }
    return start;
}
node* insert(int key, int data, node* start) {
    node* parent = nullptr;
    while(start != nullptr && start->key != key) {
        parent = start;
        if(key < start->key) 
            start = start->L;
        else
            start = start->R;
    }
    if(start != nullptr) {
        start->data = data;
        return start;
    }
    else {
        node* nd = new node(key, data);
        if(key < parent->key)
            parent->L = nd;
        else
            parent->R = nd;
        return nd;
    }
}

template<typename BidirIter, typename ValType>
BidirIter move_to_the_end(BidirIter first, BidirIter last, ValType val) {
    BidirIter ans = last;
    for(; first != last; ++first) {
        if(*first == val) {
            ValType tmp = *last;
            *last = *first;
            *first = tmp;
            ans = first;
        }
    }
    return ans;
}

template <typename BidirIter, typename ValType>
BidirIter move_all_to_the_end(BidirIter first, BidirIter last, const ValType& val) {
    BidirIter ans = last;
    bool ans_found = false;
    for(; first != last; ++first) {
        if(*first == val) {
            ValType tmp = *last;
            *last = *first;
            *first = tmp;
            --last;
            if(!ans_found) {
                ans = first;
                ans_found = true;
            }
        }
    }
    return ans;
}

template <typename BidirIter, typename UnaryPredicate>
BidirIter move_if_to_the_end(BidirIter first, BidirIter last, UnaryPredicate p) {
    BidirIter ans = last;
    bool ans_found = false;
    for(; first != last; ++first) {
        if(p(*first)) {
            swap(first, last);
            --last;
            if(!ans_found) {
                ans = first;
                ans_found = true;
            }
        }
    }
    return ans;
}

int count_nondivisible(const vector<int>& v) {
    return count_if(v.begin(), v.end(), [&v](int t) {
        return none_of(v.begin(), v.end(), [t](int tt) {
            return tt != t && t % tt == 0;
        });
    });
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    
    vector<pair<int, int>> bricks = {{5, 2}, {3, 1}, {2, 4}, {4, 2}, {3, 2}, {4, 2}, {3, 2}};
    cout << find_total_rainfall(bricks) << endl;
}
