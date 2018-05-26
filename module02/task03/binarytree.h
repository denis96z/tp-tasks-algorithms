#ifndef BINTREE_H
#define BINTREE_H

#include "tree.h"
#include <stack>

template <typename T, typename C = Comparator<T>, typename TR = TreeTraversal<T>>
class BinaryTree : public Tree<T, C, TR> {
    public:
        BinaryTree() = default;
        BinaryTree(const BinaryTree &tree) = delete;
        BinaryTree(BinaryTree &&tree) noexcept = default;

        ~BinaryTree() noexcept override;

        BinaryTree& operator =(const BinaryTree &tree) = delete;
        BinaryTree& operator =(BinaryTree &&tree) noexcept = default;

        bool TryInsert(const T &item) override;
        bool TryDelete(const T &item) override;
        bool Has(const T &item) const override;
        bool IsEmpty() const override;

        Tree<T, C, TR>& Insert(const T &item) override;
        Tree<T, C, TR>& Delete(const T &item) override;
        Tree<T, C, TR>& Clear() override;

        const T& FindMin() const override;
        const T& FindMax() const override;

        size_t CountMaxWidth() const override;

        Tree<T, C, TR>& operator<<(const T &item) override;
        Tree<T, C, TR>& operator>>(const T &item) override;

    private:
        struct TreeNode {
            T item{};
            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const T &item) : item(item) {}
        };

        TreeNode *rootNode = nullptr;

        void DeleteNode(TreeNode *&node);
};

template<typename T, typename C, typename TR>
BinaryTree<T, C, TR>::~BinaryTree() noexcept {
    BinaryTree<T, C, TR>::Clear();
}

template<typename T, typename C, typename TR>
bool BinaryTree<T, C, TR>::TryInsert(const T &item) {
    auto curPtr = &rootNode;
    while (*curPtr) {
        auto curNode = *curPtr;

        auto cmp = this->GetComparator().ApplyTo(item, curNode->item);
        if (cmp < 0) {
            curPtr = &curNode->leftNode;
        }
        else if (cmp >= 0) {
            curPtr = &curNode->rightNode;
        }

    }
    *curPtr = new TreeNode(item);
    this->IncNumItems();
    return true;
}

template<typename T, typename C, typename TR>
bool BinaryTree<T, C, TR>::TryDelete(const T &item) {
    auto curPtr = &rootNode;
    while (*curPtr) {
        auto curNode = *curPtr;

        int cmp = this->GetComparator().ApplyTo(item, curNode->item);
        if (cmp == 0) {
            DeleteNode(*curPtr);
            this->DecNumItems();
            return true;
        }

        if (cmp < 0) {
            curPtr = &curNode->leftNode;
        } else {
            curPtr = &curNode->rightNode;
        }
    }
    return false;
}

template<typename T, typename C, typename TR>
bool BinaryTree<T, C, TR>::Has(const T &item) const {
    auto curNode = rootNode;
    while (curNode) {
        int cmp = this->GetComparator().ApplyTo(item, curNode->item);
        if (cmp == 0) {
            return true;
        }
        curNode = (cmp < 0) ? curNode->leftNode : curNode->rightNode;
    }
    return false;
}

template<typename T, typename C, typename TR>
bool BinaryTree<T, C, TR>::IsEmpty() const {
    return rootNode == nullptr;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &BinaryTree<T, C, TR>::Insert(const T &item) {
    TryInsert(item);
    return *this;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &BinaryTree<T, C, TR>::Delete(const T &item) {
    TryDelete(item);
    return *this;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &BinaryTree<T, C, TR>::Clear() {
    if (rootNode) {
        std::queue<TreeNode *> queue;
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
    }
    return *this;
}

template<typename T, typename C, typename TR>
const T &BinaryTree<T, C, TR>::FindMin() const {
    if (rootNode) {
        auto curNode = rootNode;
        while (curNode->leftNode) {
            curNode = curNode->leftNode;
        }
        return curNode->item;
    }
    throw EmptyContainerException();
}

template<typename T, typename C, typename TR>
const T &BinaryTree<T, C, TR>::FindMax() const {
    if (rootNode) {
        auto curNode = rootNode;
        while (curNode->rightNode) {
            curNode = curNode->rightNode;
        }
        return curNode->item;
    }
    throw EmptyContainerException();
}

template<typename T, typename C, typename TR>
size_t BinaryTree<T, C, TR>::CountMaxWidth() const {
    if (rootNode == nullptr) {
        return 0;
    }

    size_t result = 0;

    std::queue<TreeNode*> q;
    q.push(rootNode);

    while (!q.empty())
    {
        size_t count = q.size();
        result = std::max(count, result);

        while (count--) {
            TreeNode *tempNode = q.front();
            q.pop();

            if (tempNode->leftNode != NULL) {
                q.push(tempNode->leftNode);
            }
            if (tempNode->rightNode != NULL) {
                q.push(tempNode->rightNode);
            }
        }
    }

    return result;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &BinaryTree<T, C, TR>::operator<<(const T &item) {
    return Insert(item);
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &BinaryTree<T, C, TR>::operator>>(const T &item) {
    return Delete(item);
}

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::DeleteNode(TreeNode *&node) {
    if(!node->leftNode) {
        auto rightNode = node->rightNode;
        delete node;
        node = rightNode;
    }
    else if(!node->rightNode) {
        auto leftNode = node->leftNode;
        delete node;
        node = leftNode;
    }
    else {
        auto minParent = node;
        auto min = node->rightNode;
        while(min->leftNode) {
            minParent = min;
            min = min->leftNode;
        }

        node->item = min->item;
        (minParent->leftNode == min
         ? minParent->leftNode
         : minParent->rightNode) = min->rightNode;
        delete min;
    }
}

#endif //BINTREE_H
