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
    int *aArray = nullptr, *bArray = nullptr;

    try {
        size_t n = 0;
        std::cin >> n;

        assert(n > 0);

        aArray = new int[n];
        bArray = new int[n];

        for (size_t i = 0; i < n; ++i) {
            std::cin >> aArray[i];
        }

        for (size_t i = 0; i < n; ++i) {
            std::cin >> bArray[i];
        }

        size_t aIndex = 0, bIndex = 0;
        find_max_indexes(aIndex, bIndex, aArray, bArray, n);

        std::cout << aIndex << " " << bIndex;
    }
    catch (std::bad_alloc) {
        PRINT_ERROR("[out of memory]");
    }
    catch (...) {
        PRINT_ERROR("[error]");
    }

    if (aArray) delete[] aArray;
    if (bArray) delete[] bArray;

    return 0;
}

void find_max_indexes(size_t &i, size_t &j, const int *a, const int *b, size_t n) {
    assert(a && b && n > 0);

    i = 0; j = 0;
    for (size_t k = 1; k < n; ++k) {
        if (a[k] > a[i]) {
            i = k;
        }
        if (b[k] > b[j]) {
            j = k;
        }
    }

    if (i > j) {
        size_t iNew = 0;
        for (size_t k = 1; k <= j; ++k) {
            if (a[k] > a[iNew]) {
                iNew = k;
            }
        }

        size_t jNew = i;
        for (size_t k = i + 1; k < n; ++k) {
            if (b[k] > b[jNew]) {
                jNew = k;
            }
        }

        if ((a[i] + b[jNew]) > (a[iNew] + b[j])) {
            j = jNew;
        }
        else {
            i = iNew;
        }
    }
}