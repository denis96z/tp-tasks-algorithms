#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <cstddef>

template <typename T>
class BinaryHeap {
    public:
        BinaryHeap();
        explicit BinaryHeap(const T *array, size_t arrayLength);
        BinaryHeap(const BinaryHeap &heap);
        BinaryHeap(BinaryHeap &&heap) noexcept;

        ~BinaryHeap();

        BinaryHeap& operator=(const BinaryHeap &heap);
        BinaryHeap& operator=(BinaryHeap &&heap) noexcept;

        void Add(const T &item);
        void Add(T &&item);

        const T& PeekMax() const;
        T ExtractMax();

        const T& operator[](size_t index) const;

        size_t GetNumItems() const;
        bool IsEmpty() const;

    private:
        static const size_t MIN_BUFFER_LENGTH = 16;

        T *buffer = nullptr;
        size_t bufferLength = 0;
        size_t numItems = 0;

        void CopyFrom(const BinaryHeap &heap);

        void IncBufferIfNecessary();
        void DecBufferIfPossible();

        void SiftUp(size_t index);
        void SiftDown(size_t index);
};

#include "binary_heap.hpp"

#endif //BINARY_HEAP_H
