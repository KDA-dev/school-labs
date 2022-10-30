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
    T& prev();
};

template<typename T>
T& stackm<T>::prev() {
    return data[count - 2];
}

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
    if (count == capacity) {
        capacity *= 2;
        T* new_data = new T[capacity];
        for (size_t i = 0; i < count; i++)
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
    while (count != 0)
        pop();
}

template<typename T>
void stackc<T>::push(const T& elem) {
    topnode = new node{ elem, topnode };
    ++count;
}

template<typename T>
void stackc<T>::push(T&& elem) {
    topnode = new node{ elem, topnode };
    ++count;
}

template<typename T>
void stackc<T>::pop() {
    node* new_node = topnode->next;
    delete topnode;
    topnode = new_node;
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

bool is_bracket_sequence_balanced(const string& seq) {
    stackm<char> st;
    string op_br = "{[(<", cl_br = "}])>";
    for (int i = 0; i < seq.size(); i++) {
        auto idx = op_br.find(seq[i]);
        if (idx != string::npos) {
            st.push(seq[i]);
        }
        else {
            idx = cl_br.find(seq[i]);
            if (st.size() != 0 && op_br[idx] == st.top())
                st.pop();
            else
                return false;
        }
    }
    return st.size() == 0;
}

template<typename T>
bool does_vector_contain_another(const vector<T>& vec, const vector<T>& another) {
    stackc<T> st;
    for (int i = another.size() - 1; i >= 0; i--)
        st.push(another[i]);
    for (int i = 0; i < vec.size(); i++) {
        if (st.size() != 0 && vec[i] == st.top())
            st.pop();
    }
    return st.size() == 0;
}

typedef vector<vector<int>> Graph;

// Вершины графа должны быть занумерованы одинаково, иначе NP
bool is_graph_same_as_another(const Graph& g, const Graph& another) {
    if(g.size() != another.size())
        return false;
    for(int i = 0; i < g.size(); i++)
        if(!isVecPermutationOfOther(g[i], another[i]))
            return false;
    return true;
}
bool is_graph_subset_of_another(Graph& g, Graph& another) {
    if(g.size() > another.size())
        return false;
    for(int i = 0; i < g.size(); i++) {
        sort(g[i].begin(),g[i].end());
        sort(another[i].begin(), another[i].end());
        if(!does_vector_contain_another(another[i], g[i]))
            return false;
    }
    return true;
}

int find_only_nonsequence_bracket(const string& seq) {
    stackm<pair<char, int>> st;
    string op_br = "{[(<", cl_br = "}])>";
    int sus1 = -1, sus2 = -1;
    for (int i = 0; i < seq.size(); i++) {
        auto idx = op_br.find(seq[i]);
        if (idx != string::npos) {
            st.push(mp(seq[i], idx));
        }
        else {
            idx = cl_br.find(seq[i]);
            if (st.size() != 0 && op_br[idx] == st.top().first)
                st.pop();
            else if(sus1 == -1) {
                sus1 = st.top().second;
                sus2 = i;
                --i;
            }
            else
                return sus2;
        }
    }
    if(sus1 == -1)
        return st.top().second;
    if(st.size() != 0)
        return sus2;
    return sus1;
}

template<typename T>
void sort_stack(stackm<T>& st) {
    stackm<T> sec, buffer;
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

double oriented_triangle_area(point p1, point p2, point p3) {
    return ((p2 - p1) ^ (p3 - p2)) / 2.;
}

// Задание 1: Эйлеров Путь
vector<int>& find_euler_path(vector<vector<pair<int, int>>>& G, int edge_cnt) {
    //every edge should have a sequential number attached
    stackm<int> st;
    vector<int> ans;
    vector<bool> visited(edge_cnt, false);

    for(int i = 0; i < G.size(); i++) {
        if(G[i].size() % 2 == 1) {
            st.push(i);
            break;
        }
    }
    if(st.size() == 0)
        st.push(0);
    
    while (st.size() != 0) {
        int v = st.top();
        bool found_edge = false;
        for(int i = 0; i < G[v].size(); i++) {
            if(!visited[G[v][i].second]) {
                st.push(i);
                visited[G[v][i].second] = true;
                found_edge = true;
                break;
            }
        }
        if(!found_edge) {
            st.pop();
            ans.push_back(v);
        }
    }

    return ans;
}

// Задание 2: Выпуклая оболочка
set<int>& graham(const vector<point>& v) {
    vector<int> idx(v.size());
    for(int i = 0; i < v.size(); i++)
        idx[i] = i;
    for(int i = 1; i < v.size(); i++) {
        if(v[idx[i]].y < v[idx[0]].y || (v[idx[i]].y == v[idx[0]].y && v[idx[i]].x > v[idx[0]].x))
            swap(idx[i], idx[0]);
    }
    point v0 = v[idx[0]];
    sort(idx.begin() + 1, idx.end(), [v0, v](int a, int b) {
        if(((v[a] - v0) ^ (v[b] - v0)) == 0)
            return (v[a] - v0).x < (v[b] - v0).x;
        return ((v[a] - v0) ^ (v[b] - v0)) > 0;
    });
    
    stackm<int> p;
    p.push(idx[0]);
    p.push(idx[1]);
    for(int i = 2; i < v.size(); i++) {
        while(((v[p.top()] - v[p.prev()]) ^ (v[idx[i]] - v[p.top()])) <= 0)
            p.pop();
        p.push(idx[i]);
    }
    set<int> ans;
    while(p.size() != 0) {
        ans.insert(p.top());
        p.pop();
    }
    return ans;
}

int count_rings(vector<point>& v) {
    vector<point> v2;
    int ans = 0;
    while(!v.empty()) {
        v2.clear();
        set<int> discards = graham(v);
        for(int i = 0; i < v.size(); i++)
            if(discards.count(i) == 0)
                v2.push_back(v[i]);
        v = v2;
        ans++;
    }
    return ans;
}

template<typename T, template<typename> typename stack>
void turnaround(stack<T>& target) {
    vector<T> tmp;
    while(target.size() != 0) {
        tmp.push_back(target.top());
        target.pop();
    }
    for(T& a : tmp)
        target.push(a);
}
template<typename T, template<typename> typename stack>
void merge(stack<T>& s1, stack<T>& s2, stack<T>& target) {
    while(s1.size() != 0 && s2.size() != 0) {
        if(s1.top() > s2.top()) {
            target.push(s1.top());
            s1.pop();
        }
        else {
            target.push(s2.top());
            s2.pop();
        }
    }
    while(s1.size() != 0) {
        target.push(s1.top());
        s1.pop();
    }
    while(s2.size() != 0) {
        target.push(s2.top());
        s2.pop();
    }
}
template<typename T, template<typename> typename stack>
void merge_sort(stack<T>& s) {
    int half = s.size() / 2;
    if(half == 0)
        return;

    stack<T> left, right;
    while(s.size() > half) {
        left.push(s.top());
        s.pop();
    }
    while(s.size() > 0) {
        right.push(s.top());
        s.pop();
    }

    merge_sort(left);
    merge_sort(right);
    merge(left, right, s);
    turnaround(s);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    cout << find_only_nonsequence_bracket("([()()}]())") << '\n';
    cout << (does_vector_contain_another<int>({ 1, 2, 3, 4, 5, 6 }, { 1, 3, 5 }) ? "YES\n" : "NO\n");

    stackm<int> st;
    st.push(1);
    st.push(4);
    st.push(5);
    st.push(2);
    merge_sort(st);
    while (st.size() != 0)
    {
        cout << st.top() << ' ';
        st.pop();
    }
    cout << '\n';
}
