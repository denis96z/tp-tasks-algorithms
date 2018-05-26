#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "container.h"

#include <vector>
#include <memory>

#include <cstddef>
#include <cassert>

template <typename T>
class Hash {
    public:
        Hash() = default;
        Hash(const Hash &hash) = default;
        Hash(Hash &&hash) noexcept = default;

        virtual ~Hash() = default;

        Hash& operator =(const Hash &hash) = default;
        Hash& operator =(Hash &&hash) noexcept = default;

        virtual size_t Get(const T &item, size_t bufSize) const = 0;
};

template <typename T, T DEL, typename C = Comparator<T>, typename H = Hash<T>>
class HashTable {
    public:
        HashTable() = default;
        HashTable(const HashTable &hashTable) = delete;
        HashTable(HashTable &&hashTable) noexcept = default;

        ~HashTable() noexcept = default;

        HashTable& operator =(const HashTable &hashTable) = delete;
        HashTable& operator =(HashTable &&hashTable) noexcept = default;

        bool TryAdd(const T &item);
        bool TryDelete(const T &item);
        bool Has(const T &item);

        HashTable& Add(const T &item);
        HashTable& Remove(const T &item);

        HashTable& operator <<(const T &item);
        HashTable& operator >>(const T &item);

    private:
        static const size_t MIN_BUFFER_SIZE = 8;

        using buffer_node_t = std::unique_ptr<T>;
        using buffer_t = std::vector<buffer_node_t>;

        size_t numItems = 0;
        buffer_t buffer{};

        H hash{};
        C comparator{};

        bool ShouldIncBuffer() const;
        bool ShouldDecBuffer() const;

        void IncBuffer();
        void DecBuffer();
        void ResizeBuffer(size_t newSize);
};

template<typename T, T DEL, typename C, typename H>
bool HashTable<T, DEL, C, H>::TryAdd(const T &item) {
    size_t index = hash.Get(item, buffer.size());
    assert(index >= 0 && index < buffer.size());

    int64_t delIndex = -1;

    for (size_t i = 1; i <= buffer.size(); ++i) {
        switch (buffer[index]) {
            case nullptr:
                buffer[index] = std::make_unique<T>(item);
                ++numItems;
                if (ShouldIncBuffer()) {
                    IncBuffer();
                }
                return true;

            case DEL:
                if (delIndex == -1) {
                    delIndex = index;
                    index = (index + i) % buffer.size();
                }
                break;

            default:
                if (comparator.ApplyTo(*(buffer[index]), item) == 0) {
                    return false;
                }
                index = (index + i) % buffer.size();
                break;
        }
    }

    if (delIndex != -1) {
        buffer[delIndex] = std::make_unique<T>(item);
        ++numItems;
        if (ShouldIncBuffer()) {
            IncBuffer();
        }
        return true;
    }

    return false;
}

template<typename T, T DEL, typename C, typename H>
bool HashTable<T, DEL, C, H>::TryDelete(const T &item) {
    return false;
}

template<typename T, T DEL, typename C, typename H>
bool HashTable<T, DEL, C, H>::Has(const T &item) {
    return false;
}

template<typename T, T DEL, typename C, typename H>
HashTable<T, DEL, C, H> &HashTable<T, DEL, C, H>::Add(const T &item) {
    TryAdd(item);
    return *this;
}

template<typename T, T DEL, typename C, typename H>
HashTable<T, DEL, C, H> &HashTable<T, DEL, C, H>::Remove(const T &item) {
    TryDelete(item);
    return *this;
}

template<typename T, T DEL, typename C, typename H>
HashTable<T, DEL, C, H> &HashTable<T, DEL, C, H>::operator<<(const T &item) {
    return Add(item);
}

template<typename T, T DEL, typename C, typename H>
HashTable<T, DEL, C, H> &HashTable<T, DEL, C, H>::operator>>(const T &item) {
    return Remove(item);
}

template<typename T, T DEL, typename C, typename H>
bool HashTable<T, DEL, C, H>::ShouldIncBuffer() const {
    return false;
}

template<typename T, T DEL, typename C, typename H>
bool HashTable<T, DEL, C, H>::ShouldDecBuffer() const {
    return false;
}

template<typename T, T DEL, typename C, typename H>
void HashTable<T, DEL, C, H>::IncBuffer() {

}

template<typename T, T DEL, typename C, typename H>
void HashTable<T, DEL, C, H>::DecBuffer() {

}

template<typename T, T DEL, typename C, typename H>
void HashTable<T, DEL, C, H>::ResizeBuffer(size_t newSize) {

}

/*template <typename T, typename C, typename H>

template<typename T, typename C, typename H>
bool HashTable<T, C, H>::TryAdd(const T &item) {
    size_t index = hash.Get(item, buffer.size());
    assert(index >= 0 && index < buffer.size());

    int64_t delIndex = -1;

    for (size_t i = 1; i <= buffer.size(); ++i) {
        switch (buffer[index].first) {
            case BufferNodeState::EMPTY:
                buffer[index].first = BufferNodeState::ACTIVE;
                buffer[index].second = item;
                ++numItems;
                if (ShouldIncBuffer()) {
                    IncBuffer();
                }
                return true;

            case BufferNodeState::DELETED:
                delIndex = index;
                index = (index + i) % buffer.size();
                break;

            case BufferNodeState::ACTIVE:
                if (comparator.ApplyTo(buffer[index].second, item) == 0) {
                    return false;
                }
                index = (index + i) % buffer.size();
                break;
        }
    }

    if (delIndex != -1) {
        buffer[delIndex].first = BufferNodeState::ACTIVE;
        buffer[delIndex].second = item;
        ++numItems;
        if (ShouldIncBuffer()) {
            IncBuffer();
        }
        return true;
    }

    return false;
}

template<typename T, typename C, typename H>
bool HashTable<T, C, H>::TryDelete(const T &item) {
    size_t index = hash.Get(item, buffer.size());
    assert(index >= 0 && index < buffer.size());
    for (size_t i = 1; i <= buffer.size(); ++i) {
        switch (buffer[index].first) {
            case BufferNodeState::EMPTY:
                return false;

            case BufferNodeState::DELETED:
                index = (index + i) % buffer.size();
                break;

            case BufferNodeState::ACTIVE:
                if (comparator.ApplyTo(buffer[index].second, item) != 0) {
                    index = (index + i) % buffer.size();
                    continue;
                }
                buffer[index].first = BufferNodeState::DELETED;
                --numItems;
                if (ShouldDecBuffer()) {
                    DecBuffer();
                }
                return true;
        }
    }
    return false;
}

template<typename T, typename C, typename H>
bool HashTable<T, C, H>::Has(const T &item) {
    size_t index = hash.Get(item, buffer.size());
    assert(index >= 0 && index < buffer.size());

    for (size_t i = 1; i <= buffer.size(); ++i) {
        switch (buffer[index].first) {
            case BufferNodeState::EMPTY:
                return false;

            case BufferNodeState::DELETED:
                index = (index + i) % buffer.size();
                break;

            case BufferNodeState::ACTIVE:
                if (comparator.ApplyTo(buffer[index].second, item) == 0) {
                    return true;
                }
                index = (index + i) % buffer.size();
                break;
        }
    }
    return false;
}

template<typename T, typename C, typename H>
HashTable<T, C, H> &HashTable<T, C, H>::operator<<(const T &item) {
    return Add(item);
}

template<typename T, typename C, typename H>
HashTable<T, C, H> &HashTable<T, C, H>::operator>>(const T &item) {
    return Remove(item);
}

template <typename T, typename C, typename H>
bool HashTable<T, C, H>::ShouldIncBuffer() const {
    return (double)numItems / buffer.size() > 0.75;
}

template <typename T, typename C, typename H>
bool HashTable<T, C, H>::ShouldDecBuffer() const {
    return (double)numItems / buffer.size() < 0.25;
}

template <typename T, typename C, typename H>
void HashTable<T, C, H>::IncBuffer() {
    ResizeBuffer(buffer.size() * 2);
}

template <typename T, typename C, typename H>
void HashTable<T, C, H>::DecBuffer() {
    if (buffer.size() > MIN_BUFFER_SIZE) {
        ResizeBuffer(buffer.size() / 2);
    }
}

template<typename T, typename C, typename H>
void HashTable<T, C, H>::ResizeBuffer(size_t newSize) {
    auto newBuf = buffer_t(newSize,
            std::make_pair(BufferNodeState::EMPTY, T()));

    for (auto node : buffer) {
        if (node.first != BufferNodeState::ACTIVE) {
            continue;
        }

        size_t index = hash.Get(node.second, newSize);
        assert(index >= 0 && index < newSize);

        bool wasPlaced = false;

        for (size_t i = 1; i <= newSize && !wasPlaced; ++i) {
            switch (newBuf[index].first) {
                case BufferNodeState::EMPTY:
                    newBuf[index].first = BufferNodeState::ACTIVE;
                    newBuf[index].second = node.second;
                    wasPlaced = true;
                    break;

                case BufferNodeState::ACTIVE:
                    index = (index + i) % newSize;
                    break;
            }
        }
    }

    buffer = std::move(newBuf);
}*/

#endif //HASHTABLE_H