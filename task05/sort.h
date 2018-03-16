#ifndef SORT_H
#define SORT_H

#include <cstddef>
#include <cassert>
#include <cstring>

template <typename T>
int default_compare(const T &first, const T &second) {
    return first < second;
}

template <typename T>
void merge(T *result, T *first, size_t firstLength, T *second, size_t secondLength,
        int (*compare_f)(const T &left, const T &right)) {
    assert(result && first && second && firstLength && secondLength);

    size_t firstIndex = 0;
    size_t secondIndex = 0;

    size_t resultLength = firstLength + secondLength;
    for (size_t i = 0; i < resultLength; ++i) {
        if (firstIndex == firstLength) {
            result[i] = second[secondIndex++];
        }
        else if (secondIndex == secondLength) {
            result[i] = first[firstIndex++];
        }
        else if (compare_f(first[firstIndex], second[secondIndex])) {
            result[i] = first[firstIndex++];
        }
        else {
            result[i] = second[secondIndex++];
        }
    }
}

template <typename T>
void merge_sort(T *array, size_t arrayLength,
        int (*compare_f)(const T &left, const T &right)) {
    if (arrayLength <= 1) {
        return;
    }

    size_t firstLength = arrayLength / 2;
    size_t secondLength = arrayLength - firstLength;

    merge_sort(array, firstLength, compare_f);
    merge_sort(array + firstLength, secondLength, compare_f);

    T *temp = new T[arrayLength];
    merge<T>(temp, array, firstLength, array + firstLength, secondLength, compare_f);
    memcpy(array, temp, sizeof(T) * arrayLength);
    delete[] temp;
}


#endif //SORT_H
