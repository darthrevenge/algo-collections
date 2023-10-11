#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include <set>
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

#define ull unsigned long long
#define ll long long

#define all(vec) vec.begin(), vec.end()
#define FOR(i,a,b) for(int i = a; i < b; ++i)

constexpr long long INF = 200000000000000001LL;
constexpr int INF32 = 2100000001;
size_t seed42 = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rng(seed42);
std::uniform_int_distribution<int> unidistrib;
int randint() {return unidistrib(rng);}

using namespace std;
ull modulo = 1000000007;


ull poww(ull x, ull n) {
    if (n == 0)
        return 1;
    ull answ = poww(x, n/2);
    answ = (answ * answ) % modulo;
    if (n%2)
        answ = (answ * x) % modulo;
    return answ;
}

pair<int, int> operator+ (const pair<int, int>& lhs, pair<int, int>& rhs) {
    return {lhs.first +rhs.first, lhs.second + rhs.second};
}


template <class T>
struct SegTree {
    SegTree(const vector<T> & arr): n(arr.size()), tmin(vector<T>(arr.size()*4)), tsum(vector<T>(arr.size()*4)), tmax(vector<T>(arr.size()*4)) {
        build(arr, 1, 0, n-1);
        for (auto el:tmax) {
            //cout << el.first <<"_" << el.second << " ";
        }
        cout << "tree built";
    }
    void build(const vector<T> & arr, int v, int tl, int tr) {
        if (tl == tr) {
            tsum[v] = arr[tl];
            tmin[v] = arr[tl];
            tmax[v] = arr[tl];
        } else {
            int tm = (tl + tr) / 2;
            build (arr, v*2, tl, tm);
            build (arr, v*2+1, tm+1, tr);
            tsum[v] = tsum[v*2] + tsum[v*2+1];
            tmax[v] = max(tmax[v*2], tmax[v*2+1]);
            tmin[v] = min(tmin[v*2], tmin[v*2+1]);
        }
    }

    T sum (int v, int tl, int tr, int l, int r) {
        if (l > r)
            return T();
        if (l == tl && r == tr)
            return tsum[v];
        int tm = (tl + tr) / 2;
        return sum (v*2, tl, tm, l, min(r,tm))	+ sum (v*2+1, tm+1, tr, max(l,tm+1), r);
    }

    T minn (int v, int tl, int tr, int l, int r) {
        if (l > r)
            return {INF32,INF32};//return T();
        if (l == tl && r == tr)
            return tmin[v];
        int tm = (tl + tr) / 2;
        return min(minn (v*2, tl, tm, l, min(r,tm)), minn (v*2+1, tm+1, tr, max(l,tm+1), r));
    }

    T maxx (int v, int tl, int tr, int l, int r) {
        if (l > r)
            return T(-INF32,-INF32);//return T();
        if (l == tl && r == tr)
            return tmax[v];
        int tm = (tl + tr) / 2;
        return max(maxx (v*2, tl, tm, l, min(r,tm)), maxx (v*2+1, tm+1, tr, max(l,tm+1), r));
    }


    void update (int v, int tl, int tr, int pos, T new_val) {
        if (tl == tr) {
            tmin[v] = new_val;
            tmax[v] = new_val;
            tsum[v] = new_val;
        } else {
            int tm = (tl + tr) / 2;
            if (pos <= tm)
                update (v*2, tl, tm, pos, new_val);
            else
                update (v*2+1, tm+1, tr, pos, new_val);
            tsum[v] = tsum[v*2] + tsum[v*2+1];
            tmax[v] = max(tmax[v*2], tmax[v*2+1]);
            tmin[v] = min(tmin[v*2], tmin[v*2+1]);
        }
    }

    vector<T> tsum;
    vector<T> tmax;
    vector<T> tmin;
    int n;
};

bool process (SegTree<pair<int,int>>& seg, int left, int right) {
    if (left > right)
        return true;
    //auto [val, j] = seg.maxx(1, 0, seg.n-1, left, right);
    auto [val, j] = seg.maxx(1, 0, seg.n-1, left, right);
    cout << "val "<< val << " " << j << " j\n";
    if (val <=0)
        return true;
    seg.update(1, 0, seg.n-1, j, make_pair(0, 0));




    auto[inf, i] = seg.minn(1, 0, seg.n-1, left, j-1);
    cout << "inf i " << inf << " " << i << "\n";

    i = -i;
    if (inf == -INF32) {
        seg.update(1, 0, seg.n-1, i, make_pair(0, val));  // answers are {0, val}
    } else {
        return false;
    }

    //return process(seg, left, j-1) && process(seg, j+1, right);
    return process(seg, left, right);
}


int main() {
    #ifdef DARTH
    std::ifstream filestream("input.txt");
    std::cin.rdbuf(filestream.rdbuf());
    #else
    ios::sync_with_stdio(false);
    std::cin.tie(0);
    #endif //DARTH
    int n, t, m;
    char c;
    vector<pair<int,int>> arr;
    cin >> n;
    int cnt = 0;
    for (int i = 0; i < 2*n; ++i) {
        cin >> c;
        if (c == '+')
            cnt++;
        else
            cin >> m;
        if (c == '+') {
            arr.emplace_back(-INF32, -i);   // we need to find rightmost when using min;
        } else {
            arr.emplace_back(m, i);
        }

    }
            for (auto el:arr) {
            //cout << el.first <<" " << el.second << " ";
        }
    cout << "\n arr\n";



    SegTree<pair<int,int>> seg(arr);

    if (process(seg, 0, arr.size()-1)) {
        cout << "YES\n";

    }

    for (int i = arr.size(); i < 2*arr.size(); ++i) {
        auto[x,y] = seg.tmax[i];
        if (x == 0 && y > 0)
            cout << y << " ";

    }

    cout << "\n";
    for (auto el:seg.tmax) {
        //cout << el.first<<"_"<<el.second << " ";
    }


    return 0;
}



