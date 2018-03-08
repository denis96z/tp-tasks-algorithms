#ifndef DEQUE_H
#define DEQUE_H

#include <cstddef>

template <typename T>
class Deque {
    public:
        Deque();
        Deque(const Deque &deque) = delete;
        Deque(Deque &&deque) = delete;

        ~Deque();

        void PushBack(T item);
        void PushFront(T item);

        T PopFront();
        T PopBack();

        bool IsEmpty() const;
        void Clear();

        Deque& operator=(const Deque &deque) = delete;
        Deque& operator=(Deque &&deque) = delete;

    private:
        T *buffer;
        size_t bufferLength;

        size_t numItems = 0;
        size_t firstIndex = 0;
        size_t lastIndex = 0;

        void IncBufferLengthIfNecessary();
        void DecBufferLengthIfNecessary();

        void ReallocBuffer(size_t newLength);
};

#include "deque.hpp"

#endif //DEQUE_H
