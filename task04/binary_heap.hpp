#include "binary_heap.h"

#include <cstring>
#include <algorithm>
#include <cassert>

#define QUATER(x) \
    ((x) >> 2)
#define HALF(x) \
    ((x) >> 1)
#define DOUBLE(x) \
    ((x) << 1)

template<typename T>
BinaryHeap<T>::BinaryHeap() : numItems(0) {
    buffer = new T[MIN_BUFFER_LENGTH];
    bufferLength = MIN_BUFFER_LENGTH;
}

template<typename T>
BinaryHeap<T>::BinaryHeap(const T *array, size_t arrayLength) {
    buffer = new T[DOUBLE(arrayLength)];
    bufferLength = DOUBLE(arrayLength);

    memcpy(buffer, array, arrayLength * sizeof(T));
    numItems = arrayLength;

    for (auto i = HALF(numItems) - 1; i >= 0; --i) {
        SiftDown(i);
    }
}

template<typename T>
BinaryHeap<T>::BinaryHeap(const BinaryHeap &heap) {
    CopyFrom(heap);
}

template<typename T>
BinaryHeap<T>::BinaryHeap(BinaryHeap &&heap) noexcept {
    buffer = heap.buffer;
    bufferLength = heap.bufferLength;
    numItems = heap.numItems;
}

template<typename T>
BinaryHeap<T>::~BinaryHeap() {
    delete[] buffer;
}

template<typename T>
BinaryHeap &BinaryHeap<T>::operator=(const BinaryHeap &heap) {
    CopyFrom(heap);
    return *this;
}

template<typename T>
BinaryHeap &BinaryHeap<T>::operator=(BinaryHeap &&heap) noexcept {
    buffer = heap.buffer;
    bufferLength = heap.bufferLength;
    numItems = heap.numItems;
    return *this;
}

template<typename T>
void BinaryHeap<T>::Add(const T &item) {
    IncBufferIfNecessary();
    buffer[numItems++] = item;
    SiftUp(numItems - 1);
}

template<typename T>
void BinaryHeap<T>::Add(T &&item) {
    IncBufferIfNecessary();
    buffer[numItems++] = item;
    SiftUp(numItems - 1);
}

template<typename T>
T &BinaryHeap<T>::ExtractMax() {
    assert(numItems);

    auto result = buffer[0];
    buffer[0] = buffer[(numItems--) - 1];
    if (numItems) {
        SiftDown(0);
    }

    DecBufferIfPossible();
    return result;
}

template<typename T>
const T &BinaryHeap<T>::operator[](size_t index) const {
    assert(index >= 0 && index < numItems);
    return buffer[index];
}

template<typename T>
size_t BinaryHeap<T>::GetNumItems() const {
    return numItems;
}

template<typename T>
bool BinaryHeap<T>::IsEmpty() const {
    return !numItems;
}

template<typename T>
void BinaryHeap<T>::CopyFrom(const BinaryHeap &heap) {
    buffer = new T[heap.bufferLength];
    bufferLength = heap.bufferLength;
    memcpy(buffer, heap.buffer, heap.numItems * sizeof(T));
    numItems = heap.numItems;
}

template<typename T>
void BinaryHeap<T>::IncBufferIfNecessary() {
    if (numItems == bufferLength) {
        auto temp = new T[DOUBLE(bufferLength)];
        memcpy(temp, buffer, numItems * sizeof(T));
        delete[] buffer;
        buffer = temp;
    }
}

template<typename T>
void BinaryHeap<T>::DecBufferIfPossible() {
    if (numItems == QUATER(bufferLength)) {
        auto temp = new T[HALF(bufferLength)];
        memcpy(temp, buffer, numItems * sizeof(T));
        delete[] buffer;
        buffer = temp;
    }
}

template<typename T>
void BinaryHeap<T>::SiftUp(size_t index) {
    assert(index > 0 && index < numItems);
    while (index > 0) {
        auto parentIndex = HALF(index - 1);
        if (buffer[index] <= buffer[parentIndex]) {
            return;
        }

        std::swap(buffer[index], buffer[parentIndex]);
        index = parentIndex;
    }
}

template<typename T>
void BinaryHeap<T>::SiftDown(size_t index) {
    assert(index > 0 && index < numItems);

    auto leftIndex = DOUBLE(index) + 1;
    auto rightIndex = DOUBLE(index) + 2;

    auto largestIndex = index;
    if (leftIndex < numItems && buffer[leftIndex] > buffer[index]) {
        largestIndex = leftIndex;
    }
    if (rightIndex < numItems && buffer[rightIndex] > buffer[largestIndex]) {
        largestIndex = rightIndex;
    }

    if (largestIndex != index) {
        std::swap(buffer[index], buffer[largestIndex]);
        SiftDown(largestIndex);
    }
}