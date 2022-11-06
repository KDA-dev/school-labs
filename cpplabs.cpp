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
    T& ptop();
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
T& stackm<T>::ptop() {
    return data[--count];
}

template<typename T>
const T& stackm<T>::top() const {
    return data[count - 1];
}

template<typename T>
size_t stackm<T>::size() const {
    return count;
}

enum class term_type { var, cnst, opbr, clbr, op, fun };
struct term
{
    string name;
    term_type type;
};
 
map<string, int> priority;
map<string, double> vars;
map<string, function<double(stackm<double>& in)>> funs;
 
void build_postfixcalc_prereqs()
{
    priority["+"] = 5;
    priority["-"] = 5;
    priority["u-"] = 20;
    priority["*"] = 10;
    priority["/"] = 10;
    priority["^"] = 15;
    priority["!"] = 15;

    vars["abc"] = 1;
    vars["x"] = 2;
    vars["y"] = 3;
    vars["z"] = 4;
    vars["n"] = 5;
    vars["a"] = 6;
    vars["b"] = 7;

    funs.emplace("ln", [](stackm<double>& in) -> double {
        return log(in.ptop());
    });
    funs.emplace("sin", [](stackm<double>& in) -> double {
        return sin(in.ptop());
    });
    funs.emplace("!", [](stackm<double>& in) -> double  {
        double n = in.ptop(), ans = 1;
        int z = 0;
        while(n > 1) {
            ans *= n;
            n--;
        }
        return ans;
    });
    funs.emplace("+", [](stackm<double>& in) -> double {
        double b = in.ptop(), a = in.ptop();
        return a + b;
    });
    funs.emplace("-", [](stackm<double>& in) -> double {
        double b = in.ptop(), a = in.ptop();
        return a - b;
    });
    funs.emplace("*", [](stackm<double>& in) -> double {
        double b = in.ptop(), a = in.ptop();
        return a * b;
    });
    funs.emplace("/", [](stackm<double>& in) -> double {
        double b = in.ptop(), a = in.ptop();
        return a + b;
    });
    funs.emplace("^", [](stackm<double>& in) -> double {
        double b = in.ptop(), a = in.ptop();
        return pow(a, b);
    });
    funs.emplace("u-", [](stackm<double>& in) -> double {
        return -in.ptop();
    });
}
 
const string delimiters = "~!@#$%^&*()+-/ ";
 
bool is_delimiter(char ch)
{
    return delimiters.find(ch) != string::npos;
}
 
vector<term> create_terms_from_expression(const string& expr)
{
    vector<term> res;
    int i = 0;
    while (i < expr.size())
    {
        if (expr[i] == ' ') { ++i; continue; }
        string name = string(1, expr[i]);
        if(!is_delimiter(expr[i]))
            while (i + 1 < expr.size() && !is_delimiter(expr[i + 1]))
                name += expr[++i];
        ++i;
        res.push_back({ name,term_type::op });
    }
    for (int i = 0; i < res.size(); ++i)
    {
        char ch = res[i].name[0];
        if (isdigit(ch))
            res[i].type = term_type::cnst;
        else if (ch=='(')
            res[i].type = term_type::opbr;
        else if (ch == ')')
            res[i].type = term_type::clbr;
        else if (ch == '-') {
            if(i == 0 || res[i - 1].type == term_type::opbr || res[i - 1].type == term_type::op)
                res[i].name = "u-";
        }
        else if (isalpha(ch))
            if(i+1<res.size()&&res[i+1].name=="(")
                res[i].type = term_type::fun;
            else
                res[i].type = term_type::var;
    }
    return res;
}
 
vector<term> create_postfix_from_expression(const vector<term>& terms)
{
    vector<term> res;
    stackm<term> s;
    for (const auto& t : terms)
    {
        switch (t.type)
        {
        case term_type::var:
        case term_type::cnst: 
            res.push_back(t); 
            break;
        case term_type::opbr: 
        case term_type::fun: 
            s.push(t); 
            break;
        case term_type::clbr:
            while (s.top().type != term_type::opbr)
            {
                res.push_back(s.top());
                s.pop();
            }
            s.pop();
            break;
        case term_type::op:
            while (s.size() && s.top().type != term_type::opbr)
                if (s.top().type == term_type::fun ||
                    priority[s.top().name] >= priority[t.name])
                {
                    res.push_back(s.top());
                    s.pop();
                }
                else
                    break;
            s.push(t);
        }
    }
    while (s.size())
    {
        res.push_back(s.top());
        s.pop();
    }
    return res;
}

double calculate_postfix(const vector<term>& terms) {
    stackm<double> calc;
    for(auto t : terms) {
        int z = 0;
        switch(t.type) {
            case term_type::var:
                calc.push(vars[t.name]);
                break;
            case term_type::cnst:
                calc.push(atof(t.name.c_str()));
                break;
            case term_type::op:
            case term_type::fun:
                double res = funs[t.name](calc);
                calc.push(res);
                break;
        }
    }
    return calc.top();
}
#define _DEBUG

typedef vector<vector<int>> Graph;
vector<int> to_proofer(Graph& g) {
    vector<int> ans;
    int k = g.size();
    while(k > 2) {
        int i = 0;
        while(g[i].size() != 1)
            ++i;
        int p = g[i][0];
        ans.push_back(p);
        *find(g[p].begin(), g[p].end(), i) = g[p].back();
        g[p].pop_back();
        g[i].pop_back();
        k--;
    }
    return ans;
}

Graph from_proofer(const vector<int>& proofer_code) {
    int n = proofer_code.size() + 2;
    Graph ans(n);
    vector<bool> used(n, false);
    for(int i = 0; i < proofer_code.size(); i++) {
        int j = 0;
        while(used[j] || find(proofer_code.begin() + i, proofer_code.end(), j) != proofer_code.end())
            j++;
        ans[proofer_code[i]].push_back(j);
        ans[j].push_back(proofer_code[i]);
        used[j] = true;
    }
    int x = 0, y = 0;
    while(used[x])
        x++;
    used[x] = true;
    while(used[y])
        y++;
    ans[x].push_back(y);
    ans[y].push_back(x);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    
    build_postfixcalc_prereqs();
    string expr("3.15*abc-sin(x-y/(z-a/b))^ln(n!-(-x+2.5))");
    string e2 = "-(n!)";
    cout << calculate_postfix(create_postfix_from_expression(create_terms_from_expression(expr))) << endl;
    cout << endl;

    vector<int> pr = {6, 6, 2, 2, 2};
    Graph ans = from_proofer(pr);
    for(int i = 0; i < ans.size(); i++) {
        cout << i << ": ";
        for(int j : ans[i])
            cout << j << ' ';
        cout << endl;
    }
    cout << endl;

    Graph tree = {
        {1, 4},
        {0, 2, 3},
        {1},
        {1},
        {0, 5, 6},
        {4},
        {4}
    };

    Graph t2 = from_proofer(to_proofer(tree));
    for(int i = 0; i < t2.size(); i++) {
        cout << i << ": ";
        for(int j : t2[i])
            cout << j << ' ';
        cout << endl;
    }
}
