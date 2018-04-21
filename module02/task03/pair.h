#ifndef PAIR_H
#define PAIR_H

template <typename T_FIRST, typename T_SECOND>
class Pair {
    public:
        static Pair MakeFirst(const T_FIRST &first);
        static Pair MakeSecond(const T_SECOND &second);

        Pair() = default;
        explicit Pair(const T_FIRST &first, const T_SECOND &second);
        Pair(const Pair &pair) = default;
        Pair(Pair &&pair) noexcept = default;

        virtual ~Pair() noexcept = default;

        Pair& operator =(const Pair &pair) = default;
        Pair& operator =(Pair &&pair) noexcept = default;

        const T_FIRST& GetFirst() const;
        Pair& SetFirst(const T_FIRST &first);

        const T_SECOND& GetSecond() const;
        Pair& SetSecond(const T_SECOND &second);

    private:
        T_FIRST first;
        T_SECOND second;
};

template<typename T_FIRST, typename T_SECOND>
Pair Pair<T_FIRST, T_SECOND>::MakeFirst(const T_FIRST &first) {
    return Pair(first, T_SECOND());
}

template<typename T_FIRST, typename T_SECOND>
Pair Pair<T_FIRST, T_SECOND>::MakeSecond(const T_SECOND &second) {
    return Pair(T_FIRST(), second);
}

template<typename T_FIRST, typename T_SECOND>
Pair<T_FIRST, T_SECOND>::Pair(const T_FIRST &first,
                              const T_SECOND &second) :
        first(first), second(second) {
}

template<typename T_FIRST, typename T_SECOND>
const T_FIRST &Pair<T_FIRST, T_SECOND>::GetFirst() const {
    return first;
}

template<typename T_FIRST, typename T_SECOND>
Pair &Pair<T_FIRST, T_SECOND>::SetFirst(const T_FIRST &first) {
    this->first = first;
    return *this;
}

template<typename T_FIRST, typename T_SECOND>
const T_SECOND &Pair<T_FIRST, T_SECOND>::GetSecond() const {
    return second;
}

template<typename T_FIRST, typename T_SECOND>
Pair &Pair<T_FIRST, T_SECOND>::SetSecond(const T_SECOND &second) {
    this->second = second;
    return *this;
}

#endif //PAIR_H
