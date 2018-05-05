#ifndef CONTAINER_H
#define CONTAINER_H

#include <cstddef>
#include <exception>

template <typename T>
class Comparator {
    public:
        Comparator() = default;
        Comparator(const Comparator &comparator) = default;
        Comparator(Comparator &&comparator) noexcept = default;

        virtual ~Comparator() = default;

        Comparator& operator =(const Comparator &comparator) = default;
        Comparator& operator =(Comparator &&comparator) noexcept = default;

        virtual int ApplyTo(const T &left, const T &right) const = 0;
};

#endif //CONTAINER_H
