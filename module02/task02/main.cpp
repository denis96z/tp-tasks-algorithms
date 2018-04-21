#include <iostream>
#include "binary_tree.h"

class IntCmp : public Comparator<int> {
    public:
        int ApplyTo(const int &left, const int &right) const override {
            return left - right;
        }
};

class BinTreeTrav : public BinaryTreeTraversal<int> {
    public:
        void ApplyTo(const int &item) const override {
            std::cout << item << " ";
        }
};

int main() {
    try {
        BinaryTree<int, IntCmp, BinTreeTrav> binTree;

        size_t n = 0;
        std::cin >> n;

        int item = 0;
        for (size_t i = 0; i < n; ++i) {
            std::cin >> item;
            binTree << item;
        }

        binTree.PostOrderTraversal();
    }
    catch (std::bad_alloc&) {
        std::cerr << "Failed to allocate memory.";
    }
}