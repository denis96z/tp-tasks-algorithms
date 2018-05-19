#ifndef AVLTREE_H
#define AVLTREE_H

#include "comparator.h"

#include <queue>
#include <cstdint>
#include <cassert>

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
    size_t GetHeight() const;

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

    TreeNode* RotateRight(TreeNode *node);
    TreeNode* RotateLeft(TreeNode *node);
    TreeNode* FixBalance(TreeNode *node);

    TreeNode* InsertNode(TreeNode *node, const T &key);
    TreeNode* DeleteNode(TreeNode *node, const T &key);
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
    if (rootNode) {
        std::queue<TreeNode *> queue;
        queue.push(rootNode);
        while (!queue.empty()) {
            auto curNode = queue.front();

            if (curNode->leftTree) {
                queue.push(curNode->leftTree);
            }
            if (curNode->rightTree) {
                queue.push(curNode->rightTree);
            }

            delete curNode;
            queue.pop();
        }
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
    return GetSize(rootNode);
}

template<typename T, typename C>
size_t AVLTree<T, C>::GetHeight() const {
    return static_cast<size_t>(GetHeight(rootNode));
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
    assert(k >= 0 && k < GetSize());

    auto curNode = rootNode;
    auto index = GetSize(rootNode->leftTree);

    while (index != k) {
        if (k <= index) {
            curNode = curNode->leftTree;
            index = index - (1 + GetSize(curNode->rightTree));
        }

        if (k > index) {
            curNode = curNode->rightTree;
            index = index + (1 + GetSize(curNode->leftTree));
        }
    }

    return curNode->key;
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
    node->height = std::max(hLeft, hRight) + static_cast<uint8_t>(1);
}

template<typename T, typename C>
size_t AVLTree<T, C>::GetSize(AVLTree::TreeNode *node) const {
    return node ? node->size : static_cast<size_t>(0);
}

template<typename T, typename C>
void AVLTree<T, C>::FixSize(AVLTree::TreeNode *node) {
    auto sLeft = GetSize(node->leftTree);
    auto sRight = GetSize(node->rightTree);
    node->size = sLeft + sRight + 1;
}

template<typename T, typename C>
int16_t AVLTree<T, C>::CountBalanceFactor(AVLTree::TreeNode *node) const {
    return static_cast<int16_t>(GetHeight(node->rightTree)) -
           static_cast<int16_t>(GetHeight(node->leftTree));
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::RotateRight(AVLTree::TreeNode *node) {
    TreeNode* left = node->leftTree;
    node->leftTree = left->rightTree;
    left->rightTree = node;

    FixHeight(node); FixSize(node);
    FixHeight(left); FixSize(left);

    return left;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::RotateLeft(AVLTree::TreeNode *node) {
    TreeNode* right = node->rightTree;
    node->rightTree = right->leftTree;
    right->leftTree = node;

    FixHeight(node); FixSize(node);
    FixHeight(right); FixSize(right);

    return right;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::FixBalance(AVLTree::TreeNode *node) {
    FixHeight(node); FixSize(node);

    if (CountBalanceFactor(node) == -2) {
        if (CountBalanceFactor(node->leftTree) > 0) {
            node->leftTree = RotateLeft(node->leftTree);
        }
        return RotateRight(node);
    }

    if (CountBalanceFactor(node) == 2) {
        if (CountBalanceFactor(node->rightTree) < 0) {
            node->rightTree = RotateRight(node->rightTree);
        }
        return RotateLeft(node);
    }
    return node;
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::InsertNode(AVLTree::TreeNode *node, const T &key) {
    if (!node) {
        return new TreeNode(key);
    }
    if (comparator.ApplyTo(key, node->key) < 0) {
        node->leftTree = InsertNode(node->leftTree, key);
    }
    else {
        node->rightTree = InsertNode(node->rightTree, key);
    }
    return FixBalance(node);
}

template<typename T, typename C>
typename AVLTree<T, C>::TreeNode *AVLTree<T, C>::DeleteNode(AVLTree::TreeNode *node, const T &key) {
    if (!node) {
        return nullptr;
    }

    auto cmpResult = comparator.ApplyTo(key, node->key);
    if (cmpResult == 0) {
        // Удаляем лист.
        if (!node->leftTree && !node->rightTree) {
            delete node;
            return nullptr;
        }

        // Удаляем узел, у которого два поддерева.
        if (node->leftTree && node->rightTree) {
            auto prevNode = &node;
            auto curNode = node->rightTree;

            while (curNode->leftTree) {
                *prevNode = curNode;
                curNode = curNode->leftTree;
            }

            node->key = curNode->key;
            if (curNode->rightTree) {
                auto tempNode = curNode;
                if (*prevNode == node) {
                    (*prevNode)->rightTree = curNode->rightTree;
                }
                else {
                    (*prevNode)->leftTree = curNode->rightTree;
                }
                delete tempNode;
            }
            else {
                delete curNode;
                if (*prevNode == node) {
                    (*prevNode)->rightTree = nullptr;
                }
                else {
                    (*prevNode)->leftTree = nullptr;
                }
            }

            return FixBalance(node);
        }

        // Удаляем узел, у которого одно поддерево.
        auto tempNode = node->leftTree ? node->leftTree : node->rightTree;
        delete node;
        return tempNode;
    }

    return FixBalance(DeleteNode(cmpResult < 0 ? node->leftTree : node->rightTree, key));
}

#endif //AVLTREE_H
