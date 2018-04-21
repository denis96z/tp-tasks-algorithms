#ifndef HEAPTREE_H
#define HEAPTREE_H

#include "tree.h"
#include <utility>

template <typename T_FIRST, typename T_SECOND>
class PairComparator : public Comparator<std::pair<T_FIRST, T_SECOND>> {
    public:
        PairComparator() = default;
        PairComparator(const PairComparator &comparator) = default;
        PairComparator(PairComparator &&comparator) noexcept = default;

        ~PairComparator() override = default;

        PairComparator& operator =(const PairComparator &comparator) = default;
        PairComparator& operator =(PairComparator &&comparator) noexcept = default;

        virtual int ApplyToFirst(const T_FIRST &left, const T_FIRST &right) const = 0;
        virtual int ApplyToSecond(const T_SECOND &left, const T_SECOND &right) const = 0;
};

template <typename T, typename P,
          typename C = PairComparator<T, P>,
          typename TR = TreeTraversal<T>>
class HeapTree : public Tree<std::pair<T, P>, C, TR> {
    public:
        HeapTree() = default;
        HeapTree(const HeapTree &tree) = delete;
        HeapTree(HeapTree &&tree) noexcept = default;

        ~HeapTree() noexcept override;

        HeapTree& operator =(const HeapTree &tree) = delete;
        HeapTree& operator =(HeapTree &&tree) noexcept = default;

        bool TryInsert(const std::pair<T, P> &item) override;
        bool TryDelete(const std::pair<T, P> &item) override;
        bool Has(const std::pair<T, P> &item) const override;
        bool IsEmpty() const override;

        Tree<std::pair<T, P>, C, TR> &Insert(const std::pair<T, P> &item) override;
        Tree<std::pair<T, P>, C, TR> &Delete(const std::pair<T, P> &item) override;
        Tree<std::pair<T, P>, C, TR> &Clear() override;

        const std::pair<T, P> &FindMin() const override;
        const std::pair<T, P> &FindMax() const override;

        size_t CountMaxWidth() const override;
        size_t CountMaxHeight() const override;

        void PreOrderTraverse() override;
        void InOrderTraverse() override;
        void PostOrderTraverse() override;
        void LevelOrderTraverse() override;

        Tree<std::pair<T, P>, C, TR> &operator<<(const std::pair<T, P> &item) override;
        Tree<std::pair<T, P>, C, TR> &operator>>(const std::pair<T, P> &item) override;

    private:
        struct TreeNode {
            std::pair<T, P> item{};
            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const std::pair<T, P> &item) : item(item) {}
        };

        TreeNode *rootNode = nullptr;

        void Split(TreeNode *curNode, const T &key,
                   TreeNode *&leftNode, TreeNode *&rightNode);
        TreeNode* Merge(TreeNode *leftNode, TreeNode *rightNode);

        size_t CountWidth(const TreeNode *node, size_t level) const;
        size_t CountHeight(const TreeNode *node) const;
};

template <typename T_FIRST, typename T_SECOND>
std::pair<T_FIRST, T_SECOND> make_pair_first(const T_FIRST &first) {
    return std::make_pair(first, T_SECOND());
};

template <typename T_FIRST, typename T_SECOND>
std::pair<T_FIRST, T_SECOND> make_pair_second(const T_SECOND &second) {
    return std::make_pair(T_FIRST(), second);
};

template<typename T, typename P, typename C, typename TR>
HeapTree<T, P, C, TR>::~HeapTree() noexcept {
    HeapTree<T, P, C, TR>::Clear();
}

template<typename T, typename P, typename C, typename TR>
bool HeapTree<T, P, C, TR>::TryInsert(const std::pair<T, P> &item) {
    auto curPtr = &rootNode;
    while (*curPtr) {
        auto curNode = *curPtr;

        auto cmpPriority = this->GetComparator()
                .ApplyToSecond(curNode->item.second, item.second);
        if (cmpPriority < 0) {
            break;
        }

        auto cmpKeys = this->GetComparator()
                .ApplyToFirst(curNode->item.first, item.first);
        curPtr = &((cmpKeys <= 0) ? curNode->rightNode : curNode->leftNode);
    }

    TreeNode *leftNode = nullptr, *rightNode = nullptr;
    Split(*curPtr, item.first, leftNode, rightNode);

    auto newNode = new TreeNode(item);
    newNode->leftNode = leftNode;
    newNode->rightNode = rightNode;
    *curPtr = newNode;
    this->IncNumItems();

    return true;
}

template<typename T, typename P, typename C, typename TR>
bool HeapTree<T, P, C, TR>::TryDelete(const std::pair<T, P> &item) {
    auto curPtr = &rootNode;
    while (*curPtr) {
        auto curNode = *curPtr;

        auto cmpKeys = this->GetComparator()
                .ApplyToFirst(curNode->item.first, item.first);
        if (cmpKeys == 0) {
            auto tempNode = Merge(curNode->leftNode, curNode->rightNode);
            delete curNode;
            *curPtr = tempNode;
            this->DecNumItems();
            return true;
        }

        curPtr = &((cmpKeys < 0) ? curNode->rightNode : curNode->leftNode);
    }

    return false;
}

template<typename T, typename P, typename C, typename TR>
bool HeapTree<T, P, C, TR>::Has(const std::pair<T, P> &item) const {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
bool HeapTree<T, P, C, TR>::IsEmpty() const {
    return rootNode == nullptr;
}

template<typename T, typename P, typename C, typename TR>
Tree<std::pair<T, P>, C, TR> &HeapTree<T, P, C, TR>::Insert(const std::pair<T, P> &item) {
    TryInsert(item);
    return *this;
}

template<typename T, typename P, typename C, typename TR>
Tree<std::pair<T, P>, C, TR> &HeapTree<T, P, C, TR>::Delete(const std::pair<T, P> &item) {
    TryDelete(item);
    return *this;
}

template<typename T, typename P, typename C, typename TR>
Tree<std::pair<T, P>, C, TR> &HeapTree<T, P, C, TR>::Clear() {
    while (rootNode) {
        Delete(rootNode->item);
    }
    return *this;
}

template<typename T, typename P, typename C, typename TR>
const std::pair<T, P> &HeapTree<T, P, C, TR>::FindMin() const {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
const std::pair<T, P> &HeapTree<T, P, C, TR>::FindMax() const {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
size_t HeapTree<T, P, C, TR>::CountMaxWidth() const {
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

template<typename T, typename P, typename C, typename TR>
size_t HeapTree<T, P, C, TR>::CountMaxHeight() const {
    return CountHeight(rootNode);
}

template<typename T, typename P, typename C, typename TR>
void HeapTree<T, P, C, TR>::PreOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
void HeapTree<T, P, C, TR>::InOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
void HeapTree<T, P, C, TR>::PostOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
void HeapTree<T, P, C, TR>::LevelOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename P, typename C, typename TR>
Tree<std::pair<T, P>, C, TR> &HeapTree<T, P, C, TR>::operator<<(const std::pair<T, P> &item) {
    return Insert(item);
}

template<typename T, typename P, typename C, typename TR>
Tree<std::pair<T, P>, C, TR> &HeapTree<T, P, C, TR>::operator>>(const std::pair<T, P> &item) {
    return Delete(item);
}

template<typename T, typename P, typename C, typename TR>
void HeapTree<T, P, C, TR>::Split(HeapTree::TreeNode *curNode,
                                  const T &key,
                                  HeapTree::TreeNode *&leftNode,
                                  HeapTree::TreeNode *&rightNode) {
    if (!curNode) {
        leftNode = nullptr;
        rightNode = nullptr;
    } else if (this->GetComparator().ApplyToFirst(curNode->item.first, key) <= 0) {
        Split(curNode->rightNode, key, curNode->rightNode, rightNode);
        leftNode = curNode;
    } else {
        Split(curNode->leftNode, key, leftNode, curNode->leftNode);
        rightNode = curNode;
    }
}

template <typename T, typename P, typename C, typename TR>
typename HeapTree<T, P, C, TR>::TreeNode*
HeapTree<T, P, C, TR>::Merge(TreeNode* leftNode, TreeNode* rightNode) {
    if (!leftNode || !rightNode) {
        return !leftNode ? rightNode : leftNode;
    }
    if (this->GetComparator()
            .ApplyToSecond(leftNode->item.second,
                           rightNode->item.second) > 0) {
        leftNode->rightNode = Merge(leftNode->rightNode, rightNode);
        return leftNode;
    }
    else {
        rightNode->leftNode = Merge(leftNode, rightNode->leftNode);
        return rightNode;
    }
}

template<typename T, typename P, typename C, typename TR>
size_t HeapTree<T, P, C, TR>::CountWidth(const HeapTree::TreeNode *node, size_t level) const {
    if (!node) {
        return 0;
    }
    if (level == 1) {
        return 1;
    }
    return CountWidth(node->leftNode, level - 1) +
           CountWidth(node->rightNode, level - 1);
}

template<typename T, typename P, typename C, typename TR>
size_t HeapTree<T, P, C, TR>::CountHeight(const HeapTree::TreeNode *node) const {
    if(!node) {
        return 0;
    }
    size_t hLeft = node->leftNode ? CountHeight(node->leftNode) : 0;
    size_t hRight = node->rightNode ? CountHeight(node->rightNode) : 0;
    return(std::max(hLeft, hRight) + 1);
}

#endif //HEAPTREE_H
