#include <iostream>
#include <vector>
#include <numeric>


using iterator = std::vector<int>::iterator;
using Matrix = std::vector<std::vector<int>>;

void swap4diap(iterator ab, iterator ae, iterator bb, iterator cb, iterator db) {
    for (; ab != ae; ++ab, ++bb, ++cb, ++db) {
        int tmp = *db;
        *db = *cb;
        *cb = *bb;
        *bb = *ab;
        *ab = tmp;
    }
}

struct Row {
    iterator begin() { return begin; }
    iterator end() { return begin + sz; }
    iterator begin;
    int sz;
};

struct RegionIterator {
    RegionIterator(const Matrix& M, int i0, int j0, int sz) {
        pos = M.begin() + i0;
        offset = j0;
        sz = sz;
    }
    Row operator*() {
        return { *pos.begin() + offset, sz };
    }
    RegionIterator& operator++() {
        ++pos;
        return *this;
    }
    Matrix::iterator pos;
    int offset;
    int sz;
};

bool operator!=(const RegionsIterator& lhs, const RegionIterator& rhs) {
    return lhs.pos == rhs.pos;
}

struct Region { 
    Region(const Matrix& M): pos(M) {}
    RegionIterator begin() { return pos; }
    RegionIterator end() { return pos + sz; }
    RegionIterator pos;
};

int main() {
    std::vector<int> A(3);
    std::iota(A.begin(), A.end(), 1);
    std::vector<int> B(3);
    std::iota(B.begin(), B.end(), 4);
    std::vector<int> C(3);
    std::iota(C.begin(), C.end(), 7);
    std::vector<int> D(3);
    std::iota(D.begin(), D.end(), 10);

    swap4diap(A.begin(), A.end(), B.begin(), C.begin(), D.begin());




    return 0;
}
