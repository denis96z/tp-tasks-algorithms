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

            size_t size = 1;
            int8_t balance = 0;

            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const T &key) : key(key) {}
        };

        TreeNode *rootNode = nullptr;

        C comparator{};

        void InsertNode(TreeNode *&p, const T &key, bool &heightChanged);
        void DeleteNode(TreeNode *&p, const T &key, bool &heightChanged);

        void BalanceL(TreeNode *&p, bool &heightChanged);
        void BalanceR(TreeNode *&p, bool &heightChanged);
        void DeleteNode(TreeNode *&q, TreeNode *&r, bool &heightChanged);
};

template<typename T, typename C>
AVLTree<T, C>::~AVLTree() {
    Clear();
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Insert(const T &key) {
    bool heightChanged = false;
    InsertNode(rootNode, key, heightChanged);
    return *this;
}

template<typename T, typename C>
AVLTree<T, C> &AVLTree<T, C>::Delete(const T &key) {
    bool heightChanged = false;
    DeleteNode(rootNode, key, heightChanged);
    return *this;
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
void AVLTree<T, C>::InsertNode(AVLTree::TreeNode *&p, const T &key, bool &heightChanged) {
    if (!p) {
        p = new TreeNode(key);
        heightChanged = true;
        return;
    }

    auto cmpResult = comparator.ApplyTo(p->key, key);
    TreeNode *p1 = nullptr, *p2 = nullptr;

    if (cmpResult > 0) {
        InsertNode(p->leftNode, key, heightChanged);
        if (heightChanged) /* Выросла левая ветвь. */ {
            if (p->balance == 1) {
                p->balance = 0;
                heightChanged = false;
            }
            else if (p->balance == 0) {
                p->balance = static_cast<uint8_t>(-1);
            }
            else /* Необходимо восстановить баланс. */ {
                p1 = p->leftNode;
                if (p1->balance == -1) /* Одиночная LL-ротация. */ {
                    p->leftNode = p1->rightNode; p1->rightNode = p;
                    p->balance = 0; p = p1;
                }
                else /* Двойная LR-ротация. */ {
                    p2 = p1->rightNode;
                    p1->rightNode = p2->leftNode; p2->leftNode = p1;
                    p->leftNode = p2->rightNode; p2->rightNode = p;
                    p->balance = static_cast<int8_t>(p2->balance == -1 ? 1 : 0);
                    p1->balance = static_cast<int8_t>(p2->balance == 1 ? -1 : 0);
                    p = p2;
                }
                p->balance = 0; heightChanged = false;
            }
        }
    }
    else if (cmpResult < 0) {
        InsertNode(p->rightNode, key, heightChanged);
        if (heightChanged) /* Выросла правая ветвь. */ {
            if (p->balance == -1) {
                p->balance = 0; heightChanged = false;
            }
            else if (p->balance == 0) {
                p->balance = 1;
            }
            else /* Необходимо восстановить баланс. */ {
                p1 = p->rightNode;
                if (p1->balance == 1) /* Одиночная RR-ротация. */ {
                    p->rightNode = p1->leftNode; p1->leftNode = p;
                    p->balance = 0; p = p1;
                }
                else /* Двойная RL-ротация. */ {
                    p2 = p1->leftNode;
                    p1->leftNode = p2->rightNode; p2->rightNode = p1;
                    p->rightNode = p2->leftNode; p2->leftNode = p;
                    p->balance = static_cast<int8_t>(p2->balance == 1 ? -1 : 0);
                    p1->balance = static_cast<int8_t>(p2->balance == -1 ? 1 : 0);
                    p = p2;
                }
                p->balance = 0; heightChanged = false;
            }
        }
    }
    else {
        ++(p->size);
    }
}

template<typename T, typename C>
void AVLTree<T, C>::DeleteNode(AVLTree::TreeNode *&p, const T &key, bool &heightChanged) {
    if (!p) /* Элемента нет в дереве. */ {
        return;
    }

    auto cmpResult = comparator.ApplyTo(p->key, key);
    TreeNode *p1 = nullptr, *p2 = nullptr;

    if (cmpResult > 0) {
        DeleteNode(p->leftNode, key, heightChanged);
        if (heightChanged) {
            BalanceL(p, heightChanged);
        }
    }
    else if (cmpResult < 0) {
        DeleteNode(p->rightNode, key, heightChanged);
        if (heightChanged) {
            BalanceR(p, heightChanged);
        }
    }
    else {
        auto q = p;
        if (!q->rightNode) {
            p = q->leftNode; heightChanged = true;
        }
        else if (!q->leftNode) {
            p = q->rightNode; heightChanged = true;
        }
        else {
            DeleteNode(q, q->leftNode, heightChanged);
            if (heightChanged) {
                BalanceL(p, heightChanged);
            }
        }
    }
}

template<typename T, typename C>
void AVLTree<T, C>::BalanceL(AVLTree::TreeNode *&p, bool &heightChanged) {
    TreeNode *p1 = nullptr, *p2 = nullptr;
    if (p->balance == -1) {
        p->balance = 0;
    }
    else if (p->balance == 0) {
        p->balance = 1; heightChanged = false;
    }
    else {
        p1 = p->rightNode;
        if (p1->balance >= 0) {
            p->rightNode = p1->leftNode; p1->leftNode = p;
            if (p1->balance == 0) {
                p->balance = 1; p1->balance = -1; heightChanged = false;
            }
            else {
                p->balance = 0; p1->balance = 0;
            }
            p = p1;
        }
        else {
            p2 = p1->leftNode;
            p1->leftNode = p2->rightNode; p2->rightNode = p1;
            p->rightNode = p2->leftNode; p2->leftNode = p;
            p->balance = static_cast<int8_t>(p2->balance == 1 ? -1 : 0);
            p1->balance = static_cast<int8_t>(p2->balance == -1 ? 1 : 0);
        }
    }
}

template<typename T, typename C>
void AVLTree<T, C>::BalanceR(AVLTree::TreeNode *&p, bool &heightChanged) {
    TreeNode *p1 = nullptr, *p2 = nullptr;
    if (p->balance == 1) {
        p->balance = 0;
    }
    else if (p->balance == 0) {
        p->balance = -1; heightChanged = false;
    }
    else {
        p1 = p->leftNode;
        if (p1->balance <= 0) {
            p->leftNode = p1->rightNode; p1->rightNode = p;
            if (p1->balance == 0) {
                p->balance = -1; p1->balance = 1; heightChanged = false;
            }
            else {
                p->balance = 0; p1->balance = 0;
            }
            p = p1;
        }
        else {
            p2 = p1->rightNode;
            p1->rightNode = p2->leftNode; p2->leftNode = p1;
            p->leftNode = p2->rightNode; p2->rightNode = p;
            p->balance = static_cast<int8_t>(p2->balance == -1 ? 1 : 0);
            p1->balance = static_cast<int8_t>(p2->balance == 1 ? -1 : 0);
            p = p2; p2->balance = 0;
        }
    }
}

template<typename T, typename C>
void AVLTree<T, C>::DeleteNode(AVLTree::TreeNode *&q, AVLTree::TreeNode *&r, bool &heightChanged) {
    if (r->rightNode) {
        DeleteNode(q, r->rightNode, heightChanged);
        if (heightChanged) {
            BalanceR(r, heightChanged);
        }
        else {
            q->key = r->key; q->size = r->size; q = r;
            delete r; r = r->leftNode; heightChanged = true;
        }
    }
}


#endif //AVLTREE_H
