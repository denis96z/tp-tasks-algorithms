#include <iostream>
#include "avltree.h"

class IntCmp : public Comparator<int> {
public:
    int ApplyTo(const int &left, const int &right) const override {
        return left - right;
    }
};

int main() {
    /*try {
        size_t n = 0;
        std::cin >> n;

        AVLTree<int, IntCmp> avlTree;

        int key = 0;
        size_t k = 0;

        for (size_t i = 0; i < n; ++i) {
            std::cin >> key >> k;

            if (key < 0) {
                avlTree >> std::abs(key);
            }
            else {
                avlTree << key;
            }

            int kStat = avlTree.FindStat(k);
            std::cout << kStat << " ";
        }
    }
    catch (std::bad_alloc&) {
        assert(false);
    }*/

    AVLTree<int, IntCmp> tree;
    for (int i = 0; i < 10; ++i) {
        tree.Insert(i);
    }

    for (int i = 0; i < 10; ++i) {
        tree.Delete(i);
    }

    return 0;
}