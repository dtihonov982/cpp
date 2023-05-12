#define MIN_INF -1'000'000

template <typename InIt, typename OutIt>
void getDeltas(InIt begin, InIt end, OutIt dst) {
    for (auto it = std::next(begin); it != end; ++begin, ++it, ++dst) {
        *dst = *it - *begin;
    }
}


template <typename It>
auto getMaxLeftSubArray(It begin, It end) -> typename It::value_type {
    typename It::value_type maxSum = MIN_INF;
    typename It::value_type currSum {};
    for (; begin != end; ++begin) {
        currSum += *begin;
        if (currSum > maxSum)
            maxSum = currSum;
    }
    return maxSum;
}

template <typename It>
typename It::value_type getMaxCrossSubArray(It begin, It mid, It end) {
    using ValueType = typename It::value_type;
    ValueType leftMax = getMaxLeftSubArray(mid, end);
    ValueType rightMax = getMaxLeftSubArray(std::reverse_iterator(mid), std::reverse_iterator(begin));
    return leftMax + rightMax;
}

template <typename It>
typename It::value_type getMaxSubArray(It begin, It end) {
    using ValueType = typename It::value_type;
    std::size_t sz = std::distance(begin, end);
    if (sz > 1) {
        It mid = begin +  sz / 2;
        ValueType cross = getMaxCrossSubArray(begin, mid, end);
        ValueType left = getMaxSubArray(begin, mid);
        ValueType right = getMaxSubArray(mid, end);
        return std::max(left, std::max(cross, right));
    }
    else {
        return *begin;
    }
}
