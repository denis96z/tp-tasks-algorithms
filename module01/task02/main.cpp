/* -----------------------------Задача 2-4 ----------------------------------
 * Дан отсортированный массив различных целых чисел A[0..n-1] и
 * массив целых чисел B[0..m-1]. Для каждого элемента массива B[i]
 * найти минимальный индекс элемента массива A[k], ближайшего по
 * значению к B[i]. Время работы поиска для каждого элемента B[i]: O(log(k)).
 * n <= 110000, m <= 1000
 */

#include <iostream>
#include <cassert>

#define PRINT_ERROR(msg) \
    std::cout << msg;

void find_nearest_indexes(int *result, const int *a, size_t n, const int *b, size_t m);

int main() {
    int *aArray = nullptr, *bArray = nullptr;

    try {
        size_t n = 0, m = 0;

        std::cin >> n;
        aArray = new int[n];
        for (size_t i = 0; i < n; ++i) {
            std::cin >> aArray[i];
        }

        std::cin >> m;
        bArray = new int[m];
        for (size_t i = 0; i < m; ++i) {
            std::cin >> bArray[i];
        }

        int *nearestIndexes = new int[m];
        find_nearest_indexes(nearestIndexes, aArray, n, bArray, m);

        for (size_t i = 0; i < m; ++i) {
            std::cout << nearestIndexes[i] << " ";
        }

        delete[] nearestIndexes;
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

size_t bin_search(const int *a, size_t n, int element);
size_t find_nearest_index(const int *a, size_t n, int bValue);

void find_nearest_indexes(int *result, const int *a, size_t n, const int *b, size_t m) {
    assert(a && b && n && m);
    for (size_t i = 0; i < m; ++i) {
        result[i] = find_nearest_index(a, n, b[i]);
    }
}

size_t find_nearest_index(const int *a, size_t n, int bValue) {
    if (bValue < a[0]) {
        return 0;
    }
    if (bValue > a[n - 1]) {
        return n - 1;
    }

    // Локализация отрезка от 2^i до 2^(i+1).
    size_t index1 = 0, index2 = 1;
    while (index2 < n && a[index2] < bValue) {
        index1 = index2;
        index2 += index2;
    }

    // Бинарный поиск на найденном отрезке.
    size_t result = index1 + bin_search(a + index1, (n <= index2) ?
        (n - index1) : (index2 - index1 + 1), bValue);

    // Для нахождения минимального из соседних индексов.
    if (result && (bValue <= ((a[result - 1] + a[result]) / 2))) {
        --result;
    }

    return result;
}

size_t bin_search(const int *a, size_t n, int element) {
    assert(a && n);

    size_t first = 0;
    size_t last = n;

    while (first < last) {
        size_t middle = (first + last) / 2;
        if (a[middle] < element) {
            first = middle + 1;
        }
        else {
            last = middle;
        }
    }

    return first;
}