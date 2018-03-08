#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <new>
#include <cassert>
#include <cstdlib>

#define INITIAL_DEQUE_BUFFER_LENGTH 16

#define HALF(value) \
    ((value) >> 1)
#define DOUBLE(value) \
    ((value) + (value))

template <typename T>
void shrmem(T *dest, T *src, size_t n);

template <typename T>
Deque<T>::Deque() : buffer(nullptr), bufferLength(0) {
    ReallocBuffer(INITIAL_DEQUE_BUFFER_LENGTH);
}

template <typename T>
Deque<T>::~Deque() {
    free(buffer);
    buffer = nullptr;
}

template <typename T>
void Deque<T>::PushBack(T item) {
    IncBufferLengthIfNecessary();
    if (lastIndex == bufferLength) lastIndex = 0;
    buffer[lastIndex++] = item;
    ++numItems;
}

template <typename T>
void Deque<T>::PushFront(T item) {
    IncBufferLengthIfNecessary();

    firstIndex == 0 ? firstIndex = bufferLength - 1 : --firstIndex;
    buffer[firstIndex] = item;

    ++numItems;
}

template <typename T>
T Deque<T>::PopFront() {
    assert(numItems);

    T item = buffer[firstIndex++];
    if (firstIndex == bufferLength) firstIndex = 0;

    --numItems;
    DecBufferLengthIfNecessary();

    return item;
}

template <typename T>
T Deque<T>::PopBack() {
    assert(numItems);

    lastIndex == 0 ? lastIndex = bufferLength - 1 : --lastIndex;
    T item = buffer[lastIndex];

    --numItems;
    DecBufferLengthIfNecessary();

    return item;
}

template <typename T>
bool Deque<T>::IsEmpty() const {
    return numItems == 0;
}

template <typename T>
void Deque<T>::Clear() {
    firstIndex = lastIndex = numItems = 0;
    ReallocBuffer(INITIAL_DEQUE_BUFFER_LENGTH);
}

template <typename T>
void Deque<T>::IncBufferLengthIfNecessary() {
    if (numItems == bufferLength) {
        ReallocBuffer(DOUBLE(bufferLength));
        if (firstIndex == lastIndex) {
            firstIndex = bufferLength - (numItems - firstIndex);
            shrmem<T>(buffer + firstIndex, buffer + lastIndex, numItems - lastIndex);
        }
    }
}

template <typename T>
void Deque<T>::DecBufferLengthIfNecessary() {
    //TODO
}

template <typename T>
void Deque<T>::ReallocBuffer(size_t newLength) {
    T *newBuffer = (T*)realloc(buffer, newLength * sizeof(T));
    if (!newBuffer) {
        throw std::bad_alloc();
    }

    buffer = newBuffer;
    bufferLength = newLength;
}

template <typename T>
void shrmem(T *dest, T *src, size_t n) {
    assert(n > 0);
    for (size_t i = n; i > 0; --i) {
        dest[i - 1] = src[i - 1];
    }
}

#endif //DEQUE_HPP
