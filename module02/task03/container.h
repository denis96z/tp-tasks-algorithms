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

template <typename T, typename C = Comparator<T>>
class Container {
    public:
        Container() = default;
        explicit Container(size_t numItems);
        Container(const Container &container) = default;
        Container(Container &&container) noexcept = default;

        virtual ~Container() noexcept = default;

        Container& operator =(const Container &container) = default;
        Container& operator =(Container &&container) noexcept = default;

        size_t GetNumItems() const;

    protected:
        void SetNumItems(size_t numItems);
        const C& GetComparator() const;

    private:
        size_t numItems{0};
        C comparator{};
};

class EmptyContainerException : public std::exception {
    public:
        const char *what() const noexcept override {
            return "Container is empty.";
        }
};

class NotImplementedException : public std::exception {
    public:
        const char *what() const noexcept override {
            return "Not implemented yet.";
        }
};

template<typename T, typename C>
Container<T, C>::Container(size_t numItems) {
    SetNumItems(numItems);
}


template<typename T, typename C>
size_t Container<T, C>::GetNumItems() const {
    return numItems;
}

template<typename T, typename C>
void Container<T, C>::SetNumItems(size_t numItems) {
    this->numItems = numItems;
}

template<typename T, typename C>
const C &Container<T, C>::GetComparator() const {
    return comparator;
}

#endif //CONTAINER_H
