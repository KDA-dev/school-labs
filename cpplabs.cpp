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

template<typename T>
class stackm {
private:
    T* data;
    size_t count, capacity;
    void resize_if_needed();
public:
    stackm();
    ~stackm();
    void push(const T& elem);
    void push(T&& elem);
    void pop();
    T& top();
    const T& top() const;
    size_t size() const;
};

template<typename T>
stackm<T>::stackm() {
    data = new T[1];
    count = 0;
    capacity = 1;
}

template<typename T>
stackm<T>::~stackm() {
    delete[] data;
}

template<typename T>
void stackm<T>::resize_if_needed() {
    if(count == capacity) {
        capacity *= 2;
        T* new_data = new T[capacity];
        for(size_t i = 0; i < count; i++)
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
    }
}

template<typename T>
void stackm<T>::push(const T& elem) {
    resize_if_needed();
    data[count++] = elem;
}

template<typename T>
void stackm<T>::push(T&& elem) {
    resize_if_needed();
    data[count++] = elem;
}

template<typename T>
void stackm<T>::pop() {
    --count;
}

template<typename T>
T& stackm<T>::top() {
    return data[count - 1];
}

template<typename T>
const T& stackm<T>::top() const {
    return data[count - 1];
}

template<typename T>
size_t stackm<T>::size() const {
    return count;
}

template<typename T>
class stackc {
private:
    struct node {
        T data;
        node* next;
    } *topnode;
    size_t count;
public:
    stackc();
    ~stackc();
    void push(const T& elem);
    void push(T&& elem);
    void pop();
    T& top();
    const T& top() const;
    size_t size() const;
};

template<typename T>
stackc<T>::stackc() {
    topnode = nullptr;
    count = 0;
}

template<typename T>
stackc<T>::~stackc() {
    delete topnode;
}

template<typename T>
void stackc<T>::push(const T& elem) {
    node *new_node = new node;
    new_node->data = elem;
    new_node->next = topnode;
    topnode = new_node;
    ++count;
}

template<typename T>
void stackc<T>::push(T&& elem) {
    node *new_node = new node;
    new_node->data = elem;
    new_node->next = top;
    topnode = new_node;
    ++count;
}

template<typename T>
void stackc<T>::pop() {
    topnode = topnode->next;
    --count;
}

template<typename T>
T& stackc<T>::top() {
    return topnode->data;
}

template<typename T>
const T& stackc<T>::top() const {
    return topnode->data;
}

template<typename T>
size_t stackc<T>::size() const {
    return count;
}

bool is_bracket_sequence_balanced(const string& seq) {
    stackm<char> st;
    map<char, char> ctrpart = {{ '(', ')'}, { '[', ']'}, { '{', '}'}};
    for(int i = 0; i < seq.size(); i++) {
        if(ctrpart.count(seq[i]) != 0) {
            st.push(seq[i]);
            int z = 0;
        }
        else {
            char c = st.top();
            if(ctrpart[st.top()] == seq[i])
                st.pop();
            else
                return false;
            int z = 0;
        }
    }
    return st.size() == 0;
}

template<typename T>
bool does_vector_contain_another(const vector<T>& vec, const vector<T>& another) {
    stackc<T> st;
    for(int i = another.size() - 1; i >=0; i--)
        st.push(another[i]);
    for(int i = 0; i < vec.size(); i++) {
        if(st.size() != 0 && vec[i] == st.top())
            st.pop();
    }
    return st.size() == 0;
}

vector<int> find_euler_path(vector<vector<int>>& G) {
    stackm<int> st;
    vector<int> ans;

    st.push(0);
    while(st.size() != 0) {
        int v = st.top();
        if(G[v].size() == 0) {
            ans.push_back(v);
            st.pop();
        }
        else {
            int w = G[v].back();
            G[v].pop_back();
            for(int i = 0; i < G[w].size(); i++) {
                if(G[w][i] == v) {
                    swap(G[w][i], G[w].back());
                    G[w].pop_back();
                    break;
                }
            }
            st.push(w);
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    
    cout << (is_bracket_sequence_balanced("([()()]())") ? "YES\n" : "NO\n");
    cout << (does_vector_contain_another<int>({1, 2, 3, 4, 5, 6}, {1, 3, 5}) ? "YES\n" : "NO\n");
}
