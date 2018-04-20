/* ------------------------------ Задача №6 ------------------------------------------
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9]
 * размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать число,
 * которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
 * Напишите нерекурсивный алгоритм.
 * Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
 * Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition
 * реализуйте методом прохода двумя итераторами от конца массива к началу.
 */

#include <iostream>
#include "partition.h"

#define PRINT_ERROR(err_msg) \
    std::cerr << (err_msg);

int main() {
    int *numbers = nullptr;

    try {
        size_t n = 0, k = 0;

        std::cin >> n;
        if (n <= 0) {
            throw std::bad_exception();
        }

        std::cin >> k;
        if (k < 0 || k >= n) {
            throw std::bad_exception();
        }


        numbers = new int[n];
        for (size_t i = 0; i < n; ++i) {
            std::cin >> numbers[i];
        }

        auto k_stat = find_k_stat<int>(numbers, n, k);
        std::cout << k_stat;
    }
    catch (std::bad_alloc& badAllocExc) {
        PRINT_ERROR("[Failed to allocate memory.]");
    }
    catch (std::bad_exception& badInputExc) {
        PRINT_ERROR(std::string("[Input: N > 0; 0 <= K < N]"));
    }
    catch (...) {
        PRINT_ERROR("[error]");
    }

    if (numbers) {
        delete[] numbers;
    }

    return 0;
}