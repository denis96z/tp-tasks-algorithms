#ifndef AVLTREE_H
#define AVLTREE_H

#include "comparator.h"

#include <cstdint>
#include <cassert>
#include <queue>

template <typename T, typename C = Comparator<T>>
class AVLTree {
public:
    AVLTree() = default;
    AVLTree(const AVLTree &tree) = delete;
    AVLTree(AVLTree &&tree) noexcept = default;

    ~AVLTree();

    AVLTree<T, C> &operator =(const AVLTree &tree) = delete;
    AVLTree<T, C> &operator =(AVLTree &&tree) noexcept = default;

    AVLTree<T, C>& Insert(const T &key);
    AVLTree<T, C>& Delete(const T &key);
    AVLTree<T, C>& Clear();

    bool Has(const T &key) const;

    size_t GetSize() const;
    bool IsEmpty() const;

    const T& FindMin() const;
    const T& FindMax() const;
    const T& FindStat(size_t k) const;

    AVLTree<T, C>& operator <<(const T &key);
    AVLTree<T, C>& operator >>(const T &key);

private:
    struct TreeNode {
        T key{};

        uint8_t height = 1;
        size_t size = 1;

        TreeNode *leftTree = nullptr;
        TreeNode *rightTree = nullptr;

        explicit TreeNode(const T &key) : key(key) {}
    };

    TreeNode *rootNode = nullptr;

    C comparator{};

    uint8_t GetHeight(TreeNode *node) const;
    void FixHeight(TreeNode *node);

    size_t GetSize(TreeNode *node) const;
    void FixSize(TreeNode *node);

    int16_t CountBalanceFactor(TreeNode *node) const;

    TreeNode* RotateRight(TreeNode *p);
    TreeNode* RotateLeft(TreeNode *q);
    TreeNode* FixBalance(TreeNode *p);

    TreeNode* InsertNode(TreeNode *p, const T &key);
    TreeNode* DeleteNode(TreeNode *p, const T &key);

    TreeNode* FindMinNode(TreeNode *p);
    TreeNode* DeleteMinNode(TreeNode *p);
};

template<typename T, typename C>
AVLTree<T, C>::~AVLTree() {
    Clear();
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Insert(const T &key) {
    rootNode = InsertNode(rootNode, key);
    return *this;
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Delete(const T &key) {
    rootNode = DeleteNode(rootNode, key);
    return *this;
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Clear() {
    while (rootNode) {
        rootNode = DeleteNode(rootNode, rootNode->key);
    }
    return *this;
}

template<typename T, typename C>
bool AVLTree<T, C>::Has(const T &key) const {
    assert(!IsEmpty());
    auto curNode = rootNode;
    while (curNode) {
        auto cmpResult = comparator.ApplyTo(key, curNode->key);
        if (cmpResult == 0) {
            return true;
        }
        curNode = cmpResult > 0 ? curNode->rightTree : curNode->leftTree;
    }
    return false;
}

template<typename T, typename C>
size_t AVLTree<T, C>::GetSize() const {
    return rootNode ? rootNode->size : 0;
}

template<typename T, typename C>
bool AVLTree<T, C>::IsEmpty() const {
    return !rootNode;
}

template<typename T, typename C>
const T &AVLTree<T, C>::FindMin() const {
    assert(!IsEmpty());
    auto curNode = rootNode;
    while (curNode->leftTree) {
        curNode = curNode->leftTree;
    }
    return curNode->key;
}

template<typename T, typename C>
const T &AVLTree<T, C>::FindMax() const {
    assert(!IsEmpty());
    auto curNode = rootNode;
    while (curNode->rightTree) {
        curNode = curNode->rightTree;
    }
    return curNode->key;
}

template<typename T, typename C>
const T &AVLTree<T, C>::FindStat(size_t k) const {
    assert(!IsEmpty());
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::operator<<(const T &key) {
    return Insert(key);
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::operator>>(const T &key) {
    return Delete(key);
}

template<typename T, typename C>
uint8_t AVLTree<T, C>::GetHeight(AVLTree::TreeNode *node) const {
    return node ? node->height : static_cast<uint8_t>(0);
}

template<typename T, typename C>
void AVLTree<T, C>::FixHeight(AVLTree::TreeNode *node) {
    auto hLeft = GetHeight(node->leftTree);
    auto hRight = GetHeight(node->rightTree);
    node->height = (hLeft > hRight ? hLeft : hRight) + static_cast<uint8_t>(1);
}

template<typename T, typename C>
size_t AVLTree<T, C>::GetSize(AVLTree::TreeNode *node) const {
    return node ? node->size : static_cast<size_t>(0);
}

template<typename T, typename C>
void AVLTree<T, C>::FixSize(AVLTree::TreeNode *node) {

}

template<typename T, typename C>
int16_t AVLTree<T, C>::CountBalanceFactor(AVLTree::TreeNode *node) const {
    return static_cast<int16_t>(GetHeight(node->rightTree)) -
           static_cast<int16_t>(GetHeight(node->leftTree));
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::RotateRight(AVLTree::TreeNode *p) {
    TreeNode *q = p->leftTree;
    p->leftTree = q->rightTree;
    q->rightTree = p;
    FixHeight(p);
    FixHeight(q);
    return q;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::RotateLeft(AVLTree::TreeNode *q) {
    TreeNode* p = q->rightTree;
    q->rightTree = p->leftTree;
    p->leftTree = q;
    FixHeight(q);
    FixHeight(p);
    return p;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::FixBalance(AVLTree::TreeNode *p) {
    FixHeight(p);
    if (CountBalanceFactor(p) == 2)
    {
        if (CountBalanceFactor(p->rightTree) < 0) {
            p->rightTree = RotateRight(p->rightTree);
        }
        return RotateLeft(p);
    }
    if (CountBalanceFactor(p) == -2)
    {
        if (CountBalanceFactor(p->leftTree) > 0) {
            p->leftTree = RotateLeft(p->leftTree);
        }
        return RotateRight(p);
    }
    return p;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::InsertNode(AVLTree::TreeNode *p, const T &key) {
    if (!p) {
        return new TreeNode(key);
    }
    if (comparator.ApplyTo(key, p->key) < 0) {
        p->leftTree = InsertNode(p->leftTree, key);
    }
    else {
        p->rightTree = InsertNode(p->rightTree, key);
    }
    return FixBalance(p);
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::DeleteNode(AVLTree::TreeNode *p, const T &key) {
    if (!p) {
        return nullptr;
    }
    auto cmpResult = comparator.ApplyTo(key, p->key);
    if (cmpResult < 0) {
        p->leftTree = DeleteNode(p->leftTree, key);
    }
    else if (cmpResult > 0) {
        p->rightTree = DeleteNode(p->rightTree, key);
    }
    else {
        TreeNode* q = p->leftTree;
        TreeNode* r = p->rightTree;
        delete p;
        if (!r) {
            return q;
        }
        TreeNode* min = FindMinNode(r);
        min->rightTree = DeleteMinNode(r);
        min->leftTree = q;
        return FixBalance(min);
    }
    return FixBalance(p);
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::FindMinNode(AVLTree::TreeNode *p) {
    return p->leftTree ? FindMinNode(p->leftTree) : p;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::DeleteMinNode(AVLTree::TreeNode *p) {
    if (!p->leftTree) {
        return p->rightTree;
    }
    p->leftTree = DeleteMinNode(p->leftTree);
    return FixBalance(p);
}


#endif //AVLTREE_H
