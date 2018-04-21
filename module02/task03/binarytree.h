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
        size_t CountMaxHeight() const override;

        void PreOrderTraverse() override;
        void InOrderTraverse() override;
        void PostOrderTraverse() override;
        void LevelOrderTraverse() override;

        Tree<T, C, TR>& operator<<(const T &item) override;
        Tree<T, C, TR>& operator>>(const T &item) override;

    private:
        struct TreeNode {
            T item{};
            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const T &item) : item(item) {}
        };

        enum class TraversalOrder {
            PRE_ORDER, IN_ORDER, POST_ORDER, LEVEL_ORDER
        };

        TreeNode *rootNode = nullptr;

        void DeleteNode(TreeNode *&node);
        void Traverse(const TreeNode *node, TraversalOrder traverseOrder) const;

        size_t CountWidth(const TreeNode *node, size_t level) const;
        size_t CountHeight(const TreeNode *node) const;
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
    IncNumItems();
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
            DecNumItems();
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
    while (rootNode) {
        DeleteNode(rootNode);
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
    size_t maxWidth = 0;
    size_t h = CountHeight(rootNode);
    for (size_t i = 1; i < h; ++i)
    {
        auto width = CountWidth(rootNode, i);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }
    return maxWidth;
}

template<typename T, typename C, typename TR>
size_t BinaryTree<T, C, TR>::CountMaxHeight() const {
    return CountHeight(rootNode);
}

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::PreOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::InOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::PostOrderTraverse() {
    Traverse(rootNode, TraversalOrder::IN_ORDER);
}

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::LevelOrderTraverse() {
    throw NotImplementedException();
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

template<typename T, typename C, typename TR>
void BinaryTree<T, C, TR>::Traverse(const TreeNode *node,
                                    TraversalOrder traverseOrder) const {
    enum State {
        START, LEFT, RIGHT, PARENT
    };

    State state = START;
    std::stack<const TreeNode*> stack;

    do {
        switch (state) {
            case START:
                state = LEFT;
                break;

            case LEFT:
                if (traverseOrder == TraversalOrder::PRE_ORDER) {
                    this->GetTraversal().ApplyTo(node->item);
                }
                if (node->leftNode) {
                    stack.push(node);
                    node = node->leftNode;
                    state = LEFT;
                } else {
                    state = RIGHT;
                }
                break;

            case RIGHT:
                if (traverseOrder == TraversalOrder::POST_ORDER) {
                    this->GetTraversal().ApplyTo(node->item);
                }
                if (node->rightNode) {
                    stack.push(node);
                    node = node->rightNode;
                    state = LEFT;
                } else {
                    state = PARENT;
                }
                break;

            case PARENT:
                if (traverseOrder == TraversalOrder::IN_ORDER) {
                    this->GetTraversal().ApplyTo(node->item);
                }
                if (stack.empty()) {
                    state = START;
                } else if (stack.top()->leftNode == node) {
                    node = stack.top(); stack.pop();
                    state = RIGHT;
                } else if (stack.top()->rightNode == node) {
                    node = stack.top(); stack.pop();
                    state = PARENT;
                }
                break;
        }
    } while (state != START);
}

template<typename T, typename C, typename TR>
size_t BinaryTree<T, C, TR>::CountWidth(const BinaryTree::TreeNode *node,
                                        size_t level) const {
    if (!node) {
        return 0;
    }
    if (level == 1) {
        return 1;
    }
    return CountWidth(node->leftNode, level - 1) +
           CountWidth(node->rightNode, level - 1);
}

template<typename T, typename C, typename TR>
size_t BinaryTree<T, C, TR>::CountHeight(const BinaryTree::TreeNode *node) const {
    if(!node) {
        return 0;
    }
    size_t hLeft = node->leftNode ? CountHeight(node->leftNode) : 0;
    size_t hRight = node->rightNode ? CountHeight(node->rightNode) : 0;
    return(std::max(hLeft, hRight) + 1);
}

#endif //BINTREE_H
