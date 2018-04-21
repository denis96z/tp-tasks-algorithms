#include <iostream>
#include "binarytree.h"

class IntCmp : public Comparator<int> {
public:
    int ApplyTo(const int &left, const int &right) const override {
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
    BinaryTree<int, IntCmp, IntTreeTraversal> binTree;
    return 0;
}