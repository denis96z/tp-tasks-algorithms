#include <iostream>
#include "heaptree.h"

class IntWithPriorCmp : public PairComparator<int, int> {
public:
    int ApplyTo(const std::pair<int, int> &left,
                const std::pair<int, int> &right) const override {
        throw NotImplementedException();
    }

    int ApplyToFirst(const int &left, const int &right) override {
        return left - right;
    }

    int ApplyToSecond(const int &left, const int &right) override {
        return left - right;
    }
};

class IntTreeTraversal : public TreeTraversal<int> {
public:
    void ApplyTo(const int &item) const override {
        std::cout << item << " ";
    }
};

int main() {
    HeapTree<int, int, IntWithPriorCmp, IntTreeTraversal> heapTree;
    return 0;
}