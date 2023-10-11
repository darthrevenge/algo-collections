#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
//#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <numeric>
#include <cmath>
#include <bitset>
#include <tuple>
#include <memory>
#include <random>
#include <chrono>
#include <sstream>
#include <functional>

#include <iterator>

#define ull unsigned long long
#define ll long long

#define all(vec) vec.begin(), vec.end()
#define pb push_back
#define FOR(i,a,b) for(int i = a; i < b; ++i)
#define printvec(vec) for(auto el: vec) {cout << el << " ";}

constexpr long long INF = 2000000000000000001LL;
constexpr int INF32 = 2100000001;
size_t seed42 = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rng(seed42);
std::uniform_int_distribution<int> unidistrib;
int randint() {return unidistrib(rng);}

using namespace std;
ull modulo = 998244353 ;  // 1+7*17*2^23.
//ull modulo = 998244353 ;
int modulo32 = 998244353;

ull poww(ull x, ull n) {
    if (n == 0)
        return 1;
    ull answ = poww(x, n/2);
    answ = (answ * answ) % modulo;
    if (n%2)
        answ = (answ * x) % modulo;
    return answ;
}

/*int poww(int a, int b) {
    int res = 1;
    for (; b; b >>= 1, a = 1ll * a * a % modulo32)
        if (b & 1)
            res = 1ll * res * a % modulo32;
    return res;
}*/





pair<int, int> operator+ (const pair<int, int>& lhs, pair<int, int>& rhs) {
    return {lhs.first +rhs.first, lhs.second + rhs.second};
}

template <class T>
ostream& operator << (ostream& lhs, pair<T, T>& rhs) {
    return (lhs << rhs.first<<":" << rhs.second);
}


class Fenwick2 {
  public:
    Fenwick2(int n): dataMul(n), dataAdd(n) {}

    void update(int left, int right, ll by) {
        internalUpdate(left, by, -by * (left - 1));
        internalUpdate(right, -by, by * right);
    }

    void internalUpdate(int at, ll mul, ll add) {
        while (at < dataMul.size()) {
            dataMul[at] += mul;
            dataAdd[at] += add;
            at |= (at + 1);
        }
    }

    void update(int at, ll by) {
        while (at < dataMul.size()) {
            dataAdd[at] += by;
            at |= (at + 1);
        }
    }

    ll query(int at) {
        ll mul = 0;
        ll add = 0;
        int start = at;
        while (at >= 0) {
            mul += dataMul[at];
            add += dataAdd[at];
            at = (at & (at + 1)) - 1;
        }
        return mul * start + add;
    }

    vector<ll> dataMul;
    vector<ll> dataAdd;

};



int main() {
    #ifdef DARTH
    std::ifstream filestream("input.txt");
    std::cin.rdbuf(filestream.rdbuf());
    #else
    ios::sync_with_stdio(false);
    std::cin.tie(0);
    #endif //DARTH
    int t, n, k, m, c;
    cin >> n >> k;

    vector<ll> b(n);
    for (auto & el: b)
        cin >> el;


    vector<ll> db(n);
    db[0] = b [0];

    FOR(i,1,n) {
        db[i] = b[i] - b[i-1];
    }

    Fenwick2 ft(n);

    FOR(i,0,n) {
        ft.update(i, db[i]);
    }

    ll sum = 0;

    for (int idx = n-1; idx >= k - 1; --idx) {
        ll curr_el = ft.query(idx);
        if (curr_el > 0) {
            ll by = (curr_el + (k-1))/k;
            ft.update(idx - (k-1), idx, -by);
            sum+=by;
        }
    }

    ll maxnum = 0;

    for(int idx = 0; idx < k - 1; ++ idx) {
        ll curr_el = ft.query(idx);
        if (curr_el > 0) {
            maxnum = max(maxnum, (curr_el + idx / (idx+1)));
        }
    }

    sum += maxnum;

    cout << sum << endl;





    return 0;
}

