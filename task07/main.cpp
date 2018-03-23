#include <iostream>
#include <cassert>
#include <cstring>

#define PRINT_ERROR(err_msg) \
    std::cerr << (err_msg);

template <typename T>
void lsd_sort(T *array, size_t n);

int main() {
    uint64_t *numbers = nullptr;

    try {
        size_t n = 0;

        std::cin >> n;
        if (n <= 0) {
            throw std::bad_exception();
        }

        numbers = new uint64_t[n];
        for (size_t i = 0; i < n; ++i) {
            std::cin >> numbers[i];
        }

        lsd_sort(numbers, n);
        for (size_t i = 0; i < n; ++i) {
            std::cout << numbers[i] << " ";
        }
    }
    catch (std::bad_alloc &badAllocExc) {
        PRINT_ERROR("[Failed to allocate memory.]");
    }
    catch (std::bad_exception &badInputExc) {
        PRINT_ERROR(std::string("[Input: N > 0]"));
    }
    catch (...) {
        PRINT_ERROR("[error]");
    }

    if (numbers) {
        delete[] numbers;
    }

    return 0;
}

#define BITS_IN_BYTE 256

template <typename T>
inline uint8_t get_byte(const T &x, uint8_t k) {
    return (uint8_t)(x >> (k << 3));
}

template <typename T>
void lsd_sort(T *array, size_t n) {
    assert(array && n > 0);

    size_t numBits = sizeof(uint8_t) * BITS_IN_BYTE;

    auto count = new size_t[numBits];
    auto tempArray = new T[n];

    for (uint8_t k = 0; k < sizeof(T); ++k) {
        memset(count, 0, numBits * sizeof(size_t));

        for (size_t i = 0; i < n; ++i) {
            auto b = get_byte(array[i], k);
            ++count[b];
        }

        for (size_t i = 1; i < numBits; ++i) {
            count[i] += count[i - 1];
        }

        size_t lastIndex = n - 1;
        for (size_t i = 0; i < n; ++i) {
            auto b = get_byte(array[lastIndex - i], k);
            --count[b];
            tempArray[count[b]] = array[lastIndex - i];
        }

        memcpy(array, tempArray, n * sizeof(T));
    }

    delete[] tempArray;
    delete[] count;
}