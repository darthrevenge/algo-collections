#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")

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

#include <iterator>

#define ull unsigned long long
#define ll long long

#define all(vec) vec.begin(), vec.end()
#define pb push_back
#define FOR(i,a,b) for(int i = a; i < b; ++i)
#define printvec(vec) for(auto el: vec) {cout << el << " ";}

constexpr long long INF = 200000000000000001LL;
constexpr int INF32 = 2100000001;
size_t seed42 = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rng(seed42);
std::uniform_int_distribution<int> unidistrib;
int randint() {return unidistrib(rng);}

using namespace std;
ull modulo = 998244353 ;  // 1+7*17*2^23.
//ull modulo = 1000000007 ;
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

pair<int, int> operator+ (const pair<int, int>& lhs, pair<int, int>& rhs) {
    return {lhs.first +rhs.first, lhs.second + rhs.second};
}

template <class T>
ostream& operator << (ostream& lhs, pair<T, T>& rhs) {
    return (lhs << rhs.first<<":" << rhs.second);
}



vector<int> fft(vector<int>& a, bool inv = false) {
    size_t n = a.size();

    vector<int> r(n);
    for (size_t k = 0; k < n; k++) {
        size_t b = 0;
        for (size_t z = 1; z < n; z *= 2) {
            b *= 2;
            if (k&z) ++b;
        }
        r[b] = a[k];
    }
    ull wm;

    for (int m = 2; m <= n; m *= 2) {
        if (!inv)
            wm = poww(5ULL, (119ULL<<23)/m);
        else
            wm = poww(5ULL, (((119ULL<<23)/m) * (modulo-2))%(modulo-1));

        for (int k = 0; k < n; k += m) {
            ull w = 1;
            for (int j = 0; j < m/2; j++) {
                int u = r[k+j];
                int t = (w*r[k+j+m/2])%modulo;
                r[k+j] = (u+t)%modulo32;
                r[k+j+m/2] = (u + modulo32 - t) % modulo32;
                w = (w*wm)%modulo;
            }
        }
    }
    if (inv) {
        ull ninv = poww(n, modulo-2);
        for (int i = 0; i < n; i++)
            r[i] = (ninv*r[i])%modulo;
    }
    return r;
}




int main() {
    #ifdef DARTH
    std::ifstream filestream("input.txt");
    std::cin.rdbuf(filestream.rdbuf());
    #else
    ios::sync_with_stdio(false);
    std::cin.tie(0);
    #endif //DARTH

    vector<ull> facc(200002,1), invfac(200002,1);
    for(ull i = 2; i <= 200001; ++i) {
        facc[i] = (i * facc[i-1]) % modulo;
        invfac[i] = poww(facc[i], modulo-2);
    }


    auto Cnk = [&](int n, int k) {
        if (k<0 || k> n)
            return 0ULL;
        return (((facc[n] * invfac[k]) % modulo) * invfac[n-k])%modulo;
    };

    int n;
    cin >> n;
    vector<int> a(n), b(n);
    FOR(i,0,n) {
        cin >> a[i] >> b[i];
    }


    int maxsz = 1 << 14;
    vector<int> answ (1, 1);
    vector<int> cnk;
    answ.reserve(1<<14);
    cnk.reserve(1<<14);
    //vector<ull> tmp(1<<14);

    answ[0] = 1;
    ull m = 1;

    for(int i = 0; i < n; ++i) {
        int maxj = 2 * m + a[i] - b[i];
        //int maxjj =  2 *m + max(a[i] - b[i], 0);
        int maxjpow2 = 1;
        while (maxjpow2 < maxj)
            maxjpow2<<=1;
        answ.resize(maxjpow2, 0ULL);
        cnk.resize(maxjpow2, 0ULL);

        for (int jplusm = 0; jplusm < maxj; ++jplusm) {
            cnk[jplusm] = Cnk(a[i] + b[i], b[i] + jplusm - m);
            //cout << a[i] + b[i] - j << " a[i] + b[i] - j" <<  k << "=" << cnk[k] << " ";
        }

        fill(cnk.begin()+maxj, cnk.end(), 0);

        answ = fft(answ);
        cnk = fft(cnk);

        for (int j = 0; j < answ.size(); ++j) {
            answ[j] = (answ[j]*1ULL*cnk[j]) % modulo;
        }

        answ = fft(answ, true);
        copy(answ.begin() + m, answ.begin() + maxj, answ.begin());


        m = m + a[i] - b[i];
        fill(answ.begin() + m, answ.end(), 0);
    }


    cout  << (accumulate(answ.begin(), answ.begin()+m, 0ULL)) % modulo;
    //cout << answ[3];
    //printvec(answ);
    return 0;
}

