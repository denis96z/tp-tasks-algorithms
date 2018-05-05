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

            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

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
};

template<typename T, typename C>
AVLTree<T, C>::~AVLTree() {
    Clear();
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Insert(const T &key) {
    throw "Not implemented";
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Delete(const T &key) {
    throw "Not implemented";
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Clear() {
    if (IsEmpty()) {
        return *this;
    }

    std::queue<TreeNode*> queue;
    queue.push(rootNode);

    while (!queue.empty()) {
        auto curNode = queue.front();
        if (curNode->leftNode) {
            queue.push(curNode->leftNode);
        }
        if (curNode->rightNode) {
            queue.push(curNode->rightNode);
        }
        delete curNode;
        queue.pop();
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
        curNode = cmpResult > 0 ? curNode->rightNode : curNode->leftNode;
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
    while (curNode->leftNode) {
        curNode = curNode->leftNode;
    }
    return curNode->key;
}

template<typename T, typename C>
const T &AVLTree<T, C>::FindMax() const {
    assert(!IsEmpty());
    auto curNode = rootNode;
    while (curNode->rightNode) {
        curNode = curNode->rightNode;
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
    auto hLeft = GetHeight(node->leftNode);
    auto hRight = GetHeight(node->rightNode);
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
    return static_cast<int16_t>(GetHeight(node->rightNode)) -
            static_cast<int16_t>(GetHeight(node->leftNode));
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::RotateRight(AVLTree::TreeNode *p) {
    TreeNode *q = p->leftNode;
    p->leftNode = q->rightNode;
    q->rightNode = p;
    FixHeight(p);
    FixHeight(q);
    return q;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::RotateLeft(AVLTree::TreeNode *q) {
    TreeNode* p = q->rightNode;
    q->rightNode = p->leftNode;
    p->leftNode = q;
    FixHeight(q);
    FixHeight(p);
    return p;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::FixBalance(AVLTree::TreeNode *p) {
    FixHeight(p);
    if (CountBalanceFactor(p) == 2)
    {
        if (CountBalanceFactor(p->rightNode) < 0) {
            p->rightNode = RotateRight(p->rightNode);
        }
        return RotateLeft(p);
    }
    if (CountBalanceFactor(p) == -2)
    {
        if (CountBalanceFactor(p->leftNode) > 0) {
            p->leftNode = RotateLeft(p->leftNode);
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
        p->leftNode = InsertNode(p->leftNode, key);
    }
    else {
        p->rightNode = InsertNode(p->rightNode, key);
    }
    return FixBalance(p);
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::DeleteNode(AVLTree::TreeNode *p, const T &key) {
    return nullptr;
}


#endif //AVLTREE_H
