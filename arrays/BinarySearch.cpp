#include <iostream>
#include <vector>
#include <iterator>

using iterator = std::vector<int>::iterator;

#if 0
iterator search(iterator begin, iterator end, int target) {
    if (input.empty()) {
        return input.end();
    }
    while (begin < end) {
        iterator middle = begin + (end - begin) / 2;
        if (*middle == target) {
            return middle;
        }
        else if (*middle < target) {
            begin = ++middle;
        }
        else {
            end = middle;
        }
    }
    return input.end();
}
#endif

iterator upper(iterator begin, iterator end, int target) {
    iterator lbound{begin};
    iterator ubound{end};
    while (lbound < ubound) {
        iterator middle = lbound + (ubound - lbound) / 2;
        iterator next = middle + 1;
        if (*middle == target && (next == end || *next != target )) {
            return middle;
        }
        else if (*middle > target) {
            ubound = middle;
        }
        else {
            lbound = ++middle;
        }
    }
    return end;
}

iterator lower(iterator begin, iterator end, int target) {
    iterator lbound{begin};
    iterator ubound{end};
    while (lbound < ubound) {
        iterator middle = lbound + (ubound - lbound) / 2;
        iterator prev = middle - 1;
        if (*middle == target && (middle == begin || *prev != target )) {
            return middle;
        }
        else if (*middle < target) {
            lbound = ++middle;
        }
        else {
            ubound = middle;
        }
    }
    return end;
}

std::vector<int> searchRange(std::vector<int>& nums, int target) {
    iterator begin = nums.begin();
    iterator end = nums.end();
    iterator up = upper(begin, end, target);
    if (up == end) 
        return {-1, -1};
    iterator low = lower(begin, up, target);
    auto upindex = std::distance(begin, up);
    if (low == end) 
        return {upindex, upindex};
    auto lowindex = std::distance(begin, low);
    return {lowindex, upindex};
}

int main() {
    std::istream_iterator<int> c{std::cin}, end;
    std::vector<int> vec{c, end};
    std::cout << "_____________\n";
    int x;
    std::cin >> x;
    std::vector<int> res = searchRange(vec, x);
    std::cout << res[0] << ", " << res[1] << std::endl;
    //search(vec.begin(), vec.end(), x);
    return 0;
}
       

    
