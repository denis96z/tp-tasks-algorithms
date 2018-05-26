#include <iostream>
#include "hashtable.h"

class StrCmp : public Comparator<std::string> {
public:
    int ApplyTo(const std::string &left, const std::string &right) const override {
        return left.compare(right);
    }

    bool IsDeleted(const std::string &item) const override {
        return item.empty();
    }
};

class StrHash : public Hash<std::string> {
public:
    size_t Get(const std::string &item, size_t bufSize) const override {
        size_t hash = 0;
        for(size_t i = 0; item[i] != 0; ++i) {
            hash = (hash * 3 + item[i]) % bufSize;
        }
        return hash;
    }
};

int main() {
    HashTable<std::string, StrCmp, StrHash> ht;

    char command = 0;
    std::string key;

    while (std::cin >> command >> key) {
        switch( command ) {
            case '?':
                std::cout << (ht.Has(key) ? "OK" : "FAIL") << std::endl;
                break;

            case '+':
                std::cout << (ht.TryAdd(key) ? "OK" : "FAIL") << std::endl;
                break;

            case '-':
                std::cout << (ht.TryDelete(key) ? "OK" : "FAIL") << std::endl;
                break;

            default:
                assert(false);
        }
    }

    return 0;
}