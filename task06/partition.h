#ifndef PARTITION_H
#define PARTITION_H

#include <cstddef>

#define compare_f(f) \
    int (*f)(const T &left, const T &right)

template <typename T>
int default_compare(const T &first, const T &second);

template <typename T>
T &find_k_stat(T *array, size_t n, size_t k, compare_f(compFunc) = default_compare);

template <typename T>
size_t partition(T *array, size_t n, compare_f(compFunc) = default_compare);

template <typename T>
size_t select_pivot(const T *array, size_t n, compare_f(compFunc) = default_compare);

#include "partition.hpp"

#endif //PARTITION_H
