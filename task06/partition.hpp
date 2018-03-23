#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <algorithm>
#include <cassert>

#define HALF(x) \
    ((x) >> 1)

template <typename T>
T &find_k_stat(T *array, size_t n, size_t k, compare_f(compFunc)) {
    assert(array && compFunc);
    assert(n > 0 && k >= 0 && k < n);

    size_t firstIndex = 0;
    size_t lastIndex = n - 1;

    while (true) {
        size_t p = partition(array, firstIndex, lastIndex, compFunc);
        if (p < k) {
            firstIndex = p + 1;
        }
        else if (p > k) {
            lastIndex = p - 1;
        }
        else {
            return array[p];
        }
    }
}

template <typename T>
size_t partition(T *array, size_t firstIndex, size_t lastIndex, compare_f(compFunc)) {
    assert(array && compFunc);

    size_t pivotIndex = select_pivot(array, firstIndex, lastIndex, compFunc);
    std::swap(array[firstIndex], array[pivotIndex]);

    size_t i = lastIndex, j = lastIndex;
    while(j > firstIndex) {
        while (j > firstIndex && compFunc(array[j], array[firstIndex]) <= 0) {
            --j;
        }
        while (j > firstIndex && compFunc(array[j], array[firstIndex]) > 0) {
            std::swap(array[j], array[i]);
            --j, --i;
        }
    }

    std::swap(array[i], array[firstIndex]);
    return i;
}

template <typename T>
int default_compare(const T &first, const T &second) {
    if (first == second) {
        return 0;
    }
    return first < second ? -1 : 1;
}

template <typename T>
size_t select_pivot(const T *array, size_t firstIndex, size_t lastIndex, compare_f(compFunc)) {
    assert(array && compFunc);

    const size_t midIndex = HALF(firstIndex + lastIndex);

    if (compFunc(array[firstIndex], array[midIndex]) < 0) {
        return compFunc(array[midIndex], array[lastIndex]) < 0 ? midIndex : lastIndex;
    }

    return compFunc(array[firstIndex], array[lastIndex]) < 0 ? firstIndex : lastIndex;
}

#endif //PARTITION_HPP
