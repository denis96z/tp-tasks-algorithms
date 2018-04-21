#include <iostream>
#include "heaptree.h"
#include "binarytree.h"

class IntCmp : public Comparator<int> {
public:
    int ApplyTo(const int &left, const int &right) const override {
        return left - right;
    }
};

class IntWithPriorCmp : public PairComparator<int, int> {
public:
    int ApplyTo(const std::pair<int, int> &left,
                const std::pair<int, int> &right) const override {
        throw NotImplementedException();
    }

    int ApplyToFirst(const int &left, const int &right) const override {
        return left - right;
    }

    int ApplyToSecond(const int &left, const int &right) const override {
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
    try {
        BinaryTree<int, IntCmp, IntTreeTraversal> binaryTree;
        HeapTree<int, int, IntWithPriorCmp, IntTreeTraversal> heapTree;

        size_t n = 0;
        std::cin >> n;

        int key = 0, priority = 0;
        for (size_t i = 0; i < n; ++i) {
            std::cin >> key >> priority;
            heapTree << std::make_pair(key, priority);
            binaryTree << key;
        }

        size_t widthDif = heapTree.CountMaxWidth() -
                          binaryTree.CountMaxWidth();
        std::cout << widthDif;
    }
    catch (std::bad_alloc&) {
        std::cerr << "Failed to allocate memory.";
    }
    return 0;
}