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
ull modulo = 998244353 ;
//ull modulo = 1000000007 ;

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

//acyclic graph - do not check for grey vertices
void dfs(int u, vector<vector<int>>& graph, vector<int>& grundy, vector<int>& tmp_grundies) {
    if (grundy[u] !=-1)
        return;

    grundy[u] = -2;


    for (int v: graph[u]) {
        dfs(v, graph, grundy, tmp_grundies);
    }

    for (int v: graph[u]) {
        tmp_grundies[grundy[v]] = 1;
    }

    for (int i = 0; i < tmp_grundies.size(); ++i) {
        if (! tmp_grundies[i]) {
            grundy[u] = i;
            break;
        }
    }

    for (int v: graph[u]) {
        tmp_grundies[grundy[v]] = 0;
    }
}

void printA(vector<vector<ull>>& A) {
    int n = 4;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

vector<ull> gauss (vector<vector<ull>> A, vector<ull> b) {
    int nrows = A.size();
    int ncols = A[0].size();   //rows == cols, otherwise my algo is poor

    //direct way
    //printA(A);
    //    cout << "====\n";
    for (int col = 0; col < ncols; ++col) {
        int row = col;
        while (row < nrows && A[row][col] == 0) {
            row++;
        }
        if (row == nrows) {
            /*if (col < 4) {
            printA(A);
            cout << "====\n";
            }*/

            continue;  // All variables are zero - don't need to do anything with this row;
        }
        if (row != col) {
            swap(A[col], A[row]);
            swap(b[col], b[row]);
        }
        row = col;

        ull a_inv = poww(A[row][col], modulo-2);
        for(int i = col; i < ncols; ++i)
            A[row][i]=(A[row][i] * a_inv) % modulo;
        b[row] = (b[row] * a_inv) % modulo;


        for (int j = row + 1; j < nrows; ++j) {
            ull minusK = (modulo - A[j][col]) % modulo;
            A[j][col] = 0;
            for (int i = col + 1; i < ncols; ++i) {
                A[j][i] = (A[j][i] + minusK * A[row][i]) % modulo;
            }
            b[j] = (b[j] + minusK * b[row]) % modulo;
        }
        /*if (col < 4) {
        printA(A);
        cout << "====\n";
        }*/
    }

    //reverse way
    vector<ull> answ(nrows, 0);
    for (int row = nrows-1; row >=0; row--) {
        answ[row] = b[row];
        for (int col = row + 1; col < ncols; ++col) {
            answ[row] = (answ[row] + (modulo - A[row][col]) * answ[col]) % modulo;
        }
    }

    return answ;

}





int main() {
    #ifdef DARTH
    std::ifstream filestream("input.txt");
    std::cin.rdbuf(filestream.rdbuf());
    #else
    ios::sync_with_stdio(false);
    std::cin.tie(0);
    #endif //DARTH
    int k,n,m,t;
    cin >> n >> m;
    vector<vector<int>>graph(n);

    FOR(i,0,m) {
        int u,v;
        cin >> u >> v;
        graph[--u].pb(--v);
    }
    vector<int> grundy(n,-1);
    vector<int> grundies(512);

    FOR(i,0,n) {
        dfs(i, graph, grundy, grundies);
    }
    FOR(i,0,n)
        grundies[grundy[i]]++;


    ull nplus1_inv = poww(n+1, modulo -2);
    //printvec(grundy);
    //printvec(grundies);

    vector<vector<ull>> A(512, vector<ull>(512,0));
    FOR(i,0,512) {
        FOR(j,0,512) {
            A[i][i^j] = (static_cast<ull>(grundies[j]) * nplus1_inv)%modulo;
        }
    }
    FOR(i,0,512) {
        A[i][i] = (A[i][i] + modulo - 1) % modulo;
    }

    vector<ull> b(512, modulo - nplus1_inv);
    b[0] = 0;
    vector<ull> answ = gauss(A,b);

    cout << answ[0] << endl;
    cout << endl;

    //printvec(A[1]);

    return 0;
}

