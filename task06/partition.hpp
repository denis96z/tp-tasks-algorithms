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

    while (true) {
        size_t p = partition(array, n, compFunc);
        if (p < k) {
            size_t d = p + 1;
            array += d;
            n -= d;
            k -= d;
        }
        else if (p > k) {
            n = p;
        }
        else {
            return array[k];
        }
    }
}

template <typename T>
size_t partition(T *array, size_t n, compare_f(compFunc)) {
    assert(array && compFunc && n > 0);

    if (n == 1) {
        return 0;
    }

    size_t pivotIndex = select_pivot(array, n, compFunc);
    std::swap(array[pivotIndex], array[0]);

    size_t i = n, j = n - 1;
    for (/*NOP*/; j > 0; --j) {
        if (compFunc(array[j], array[0]) >= 0) {
            std::swap(array[j], array[--i]);
        }
    }

    std::swap(array[--i], array[0]);
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
size_t select_pivot(const T *array, size_t n, compare_f(compFunc)) {
    assert(array && compFunc && n > 0);

    const size_t firstIndex = 0;
    const size_t midIndex = HALF(n);
    const size_t lastIndex = n - 1;

    if (compFunc(array[firstIndex], array[midIndex]) < 0) {
        return compFunc(array[midIndex], array[lastIndex]) < 0 ? midIndex : lastIndex;
    }

    return compFunc(array[firstIndex], array[lastIndex]) < 0 ? firstIndex : lastIndex;
}

#endif //PARTITION_HPP
