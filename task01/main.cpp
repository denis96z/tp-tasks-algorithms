/* ------------------------- Задача 1.1 --------------------------------
 * Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
 * Необходимо найти первую пару индексов i0 и j0, i0 <= j0, такую что
 * A[i0] + A[j0] = max{A[i] + B[i], где 0 <= i < n, 0 <= j < n, i <= j}.
 * Время работы - O(n), n <= 100000
 */

#include <iostream>
#include <cassert>

#define PRINT_ERROR(msg) \
    std::cout << msg;

void find_max_indexes(size_t &i, size_t &j, const int *a, const int *b, size_t n);

int main() {
    try {
        size_t n = 0;
        std::cin >> n;

        int *aArray = new int[n];
        int *bArray = new int[n];

        for (size_t i = 0; i < n; ++i) {
            std::cin >> aArray[i];
        }

        for (size_t i = 0; i < n; ++i) {
            std::cin >> bArray[i];
        }

        size_t aIndex = 0, bIndex = 0;
        find_max_indexes(aIndex, bIndex, aArray, bArray, n);

        delete[] aArray;
        delete[] bArray;

        std::cout << aIndex << " " << bIndex;
    }
    catch (std::bad_alloc) {
        PRINT_ERROR("[out of memory]");
    }
    catch (...) {
        PRINT_ERROR("[error]");
    }

    return 0;
}

void find_max_indexes(size_t &i, size_t &j, const int *a, const int *b, size_t n) {
    assert(a && b && n > 0);

    j = 0;
    for (size_t k = 1; k < n; ++k) {
        if (b[k] > b[j]) {
            j = k;
        }
    }

    i = 0;
    for (size_t k = 1; k < n && k <= j; ++k) {
        if (a[k] > a[i]) {
            i = k;
        }
    }
}