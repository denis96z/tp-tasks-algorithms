#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <cstddef>
#include <cassert>

#include <stack>
#include <queue>
#include <algorithm>

template <typename T>
class Comparator {
    public:
        Comparator() = default;
        Comparator(const Comparator &comparator) = default;
        Comparator(Comparator &&comparator) noexcept = default;

        virtual ~Comparator() = default;

        Comparator& operator =(const Comparator &comparator) = default;
        Comparator& operator =(Comparator &&comparator) noexcept = default;

        virtual int ApplyTo(const T &left, const T &right) const = 0;
};

template <typename T>
class BinaryTreeTraversal {
    public:
        BinaryTreeTraversal() = default;
        BinaryTreeTraversal(const BinaryTreeTraversal &traversal) = default;
        BinaryTreeTraversal(BinaryTreeTraversal &&traversal) noexcept = default;

        virtual ~BinaryTreeTraversal() = default;

        BinaryTreeTraversal& operator =(const BinaryTreeTraversal &traversal) = default;
        BinaryTreeTraversal& operator =(BinaryTreeTraversal &&traversal) noexcept = default;

        virtual void ApplyTo(const T &item) const = 0;
};

template <typename T, typename C = Comparator<T>, typename TR = BinaryTreeTraversal<T>>
class BinaryTree {
    public:
        BinaryTree() = default;
        BinaryTree(const BinaryTree &tree) = delete;
        BinaryTree(BinaryTree &&tree) noexcept = default;

        ~BinaryTree() noexcept;

        BinaryTree& operator =(const BinaryTree &tree) = delete;
        BinaryTree& operator =(BinaryTree &&tree) noexcept = default;

        BinaryTree& Insert(const T &item);
        BinaryTree& Delete(const T &item);
        BinaryTree& Clear();

        bool TryInsert(const T &item);
        bool TryDelete(const T &item);

        BinaryTree& operator <<(const T &item);
        BinaryTree& operator >>(const T &item);

        void PostOrderTraversal() const;

        const T& FindMin() const;
        const T& FindMax() const;

    private:
        struct TreeNode {
            T item{};
            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const T &item) : item(item) {}
        };

        TreeNode *rootNode = nullptr;

        C comparator{};
        TR traversal{};

        void DeleteNode(TreeNode *&node);
};

class EmptyContainerException : public std::exception {
    public:
        const char *what() const noexcept override {
            return "Container is empty.";
        }
};

class NotImplementedException : public std::exception {
    public:
        const char *what() const noexcept override {
            return "Not implemented yet.";
        }
};

template<typename T, typename C, typename TR>
BinaryTree<T, C, TR>::~BinaryTree() noexcept {
    Clear();
}

template<typename T, typename C, typename TR>
BinaryTree<T, C, TR> &BinaryTree<T, C, TR>::Insert(const T &item) {
    TryInsert(item);
    return *this;
}

template<typename T, typename C, typename TR>
BinaryTree<T, C, TR> &BinaryTree<T, C, TR>::Delete(const T &item) {
    TryDelete(item);
    return *this;
}

template<typename T, typename C, typename TR>
BinaryTree<T, C, TR> &BinaryTree<T, C, TR>::Clear() {
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
BinaryTree<T, C, TR> &BinaryTree<T, C, TR>::operator<<(const T &item) {
    return Insert(item);
}

template<typename T, typename C, typename TR>
BinaryTree<T, C, TR> &BinaryTree<T, C, TR>::operator>>(const T &item) {
    return Delete(item);
}

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::PostOrderTraversal() const {
    if (!rootNode) {
        return;
    }

    std::stack<TreeNode*> stack;
    auto curNode = rootNode;

    while (curNode || !stack.empty()){
        if (!stack.empty()){
            curNode = stack.top();
            stack.pop();

            if (!stack.empty() && curNode->rightNode == stack.top()){
                curNode = stack.top();
                stack.pop();
            }
            else{
                traversal.ApplyTo(curNode->item);
                curNode = nullptr;
            }
        }
        while (curNode) {
            stack.push(curNode);
            if (curNode->rightNode){
                stack.push(curNode->rightNode);
                stack.push(curNode);
            }
            curNode = curNode->leftNode;
        }
    }
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
bool BinaryTree<T, C, TR>::TryInsert(const T &item) {
    auto curPtr = &rootNode;
    while (*curPtr) {
        auto curNode = *curPtr;

        auto cmp = comparator.ApplyTo(item, curNode->item);
        if (cmp < 0) {
            curPtr = &curNode->leftNode;
        }
        else if (cmp >= 0) {
            curPtr = &curNode->rightNode;
        }

    }
    *curPtr = new TreeNode(item);
    return true;
}

template<typename T, typename C, typename TR>
bool BinaryTree<T, C, TR>::TryDelete(const T &item) {
    auto curPtr = &rootNode;
    while (*curPtr) {
        auto curNode = *curPtr;

        int cmp = comparator.ApplyTo(item, curNode->item);
        if (cmp == 0) {
            DeleteNode(*curPtr);
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
void BinaryTree<T, C, TR>::DeleteNode(BinaryTree::TreeNode *&node) {
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

#endif //BINARY_TREE_H
