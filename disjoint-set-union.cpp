#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
// allows elements of template class T thus allows custom features?
// template <class T>
class DSU {
 public:
  DSU(size_t N): // data(N, T()),
                   parent(N),
                   rank(N, 0) { }

  size_t findRoot (size_t x) {
      if (x == parent[x]) {
        return x;
      }
      return p[x] = findRoot (size_t x);
  }
  
  // False if already united
  bool unite (size_t x, size_t y) {
    if ((x = findRoot(x)) == (y = findRoot(y))) {
      return false;
    }
    
    if (rank[y] < rank[x]) {
      std::swap(x,y);
    }

    p[x] = y;
    rank[y] += (rank[y] == rank[x]);
    return true;
  }

 private:
  std::vector<size_t> parent;
  std::vector<size_t> rank;
};






int main() {

  return 0;
}
