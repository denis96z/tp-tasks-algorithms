#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <new>
#include <cassert>
#include <cstdlib>
#include <cstring>

#define INITIAL_DEQUE_BUFFER_LENGTH 16

#define HALF(value) \
    ((value) >> 1)
#define DOUBLE(value) \
    ((value) + (value))

template <typename T>
Deque<T>::Deque() : buffer(nullptr) {
    buffer = new T[INITIAL_DEQUE_BUFFER_LENGTH];
    bufferLength = INITIAL_DEQUE_BUFFER_LENGTH;
}

template <typename T>
Deque<T>::~Deque() {
    free(buffer);
    buffer = nullptr;
}

template <typename T>
void Deque<T>::PushBack(T item) {
    IncBufferIfNecessary();
    if (lastIndex == bufferLength) lastIndex = 0;
    buffer[lastIndex++] = item;
    ++numItems;
}

template <typename T>
void Deque<T>::PushFront(T item) {
    IncBufferIfNecessary();

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
    DecBufferIfNecessary();

    return item;
}

template <typename T>
T Deque<T>::PopBack() {
    assert(numItems);

    lastIndex == 0 ? lastIndex = bufferLength - 1 : --lastIndex;
    T item = buffer[lastIndex];

    --numItems;
    DecBufferIfNecessary();

    return item;
}

template <typename T>
bool Deque<T>::IsEmpty() const {
    return numItems == 0;
}

template <typename T>
void Deque<T>::Clear() {
    firstIndex = lastIndex = numItems = 0;
    delete[] buffer;
    buffer = new T[INITIAL_DEQUE_BUFFER_LENGTH];
}

template <typename T>
void Deque<T>::IncBufferIfNecessary() {
    if (numItems == bufferLength) {
        IncBuffer(DOUBLE(bufferLength));
    }
}

template <typename T>
void Deque<T>::DecBufferIfNecessary() {
    //TODO
}

template <typename T>
void Deque<T>::IncBuffer(size_t newLength) {
    auto *newBuffer = new T[newLength];
    if (firstIndex == lastIndex) {
        auto tempIndex = bufferLength - lastIndex;
        memmove(newBuffer, buffer + firstIndex, tempIndex * sizeof(T));
        memmove(newBuffer + tempIndex, buffer, lastIndex * sizeof(T));
        firstIndex = 0;
        lastIndex = numItems;
    }
    else {
        memmove(newBuffer, buffer, bufferLength * sizeof(T));
    }

    delete[] buffer;
    buffer = newBuffer;
    bufferLength = newLength;
}

#endif //DEQUE_HPP
