#ifndef AVLTREE_H
#define AVLTREE_H

#include "tree.h"
#include <cstdint>
#include <cassert>

template <typename T, typename C = Comparator<T>, typename TR = TreeTraversal<T>>
class AVLTree : public Tree<T, C, TR> {
    public:
        AVLTree() = default;
        AVLTree(const AVLTree &tree) = delete;
        AVLTree(AVLTree &&tree) noexcept = default;

        ~AVLTree() override;

        AVLTree<T, C, TR> &operator =(const AVLTree &tree) = delete;
        AVLTree<T, C, TR> &operator =(AVLTree &&tree) noexcept = default;

        bool TryInsert(const T &item) override;
        bool TryDelete(const T &item) override;
        bool Has(const T &item) const override;
        bool IsEmpty() const override;

        Tree<T, C, TR>& Insert(const T &item) override;
        Tree<T, C, TR>& Delete(const T &item) override;
        Tree<T, C, TR>& Clear() override;

        const T& FindMin() const override;
        const T& FindMax() const override;
        const T& FindStat(size_t k) const override;

        size_t CountMaxWidth() const override;
        size_t CountMaxHeight() const override;

        void PreOrderTraverse() override;
        void InOrderTraverse() override;
        void PostOrderTraverse() override;
        void LevelOrderTraverse() override;

        Tree<T, C, TR>& operator <<(const T &item) override;
        Tree<T, C, TR>& operator >>(const T &item) override;

    private:
        struct TreeNode {
            T item{};
            uint8_t height = 1;
            size_t numItems = 1;
            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const T &item) : item(item) {}
        };

        TreeNode *rootNode = nullptr;

        uint8_t GetHeight(TreeNode *node) const;
        void FixHeight(TreeNode *node);

        size_t GetSize(TreeNode *node) const;
        void FixSize(TreeNode *node);

        int16_t CountBalanceFactor(TreeNode *node) const;

        TreeNode* RotateRight(TreeNode *node);
        TreeNode* RotateLeft(TreeNode *node);
        TreeNode* FixBalance(TreeNode *node);

        TreeNode* InsertNode(TreeNode *node, const T &item);
        TreeNode* DeleteNode(TreeNode *node, const T &item);
        void ClearNode(TreeNode *node);
};

template<typename T, typename C, typename TR>
AVLTree<T, C, TR>::~AVLTree() {
    this->Clear();
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::TryInsert(const T &item) {
    rootNode = InsertNode(rootNode, item);
    return true;
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::TryDelete(const T &item) {
    rootNode = DeleteNode(rootNode, item);
    return true;
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::Has(const T &item) const {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::IsEmpty() const {
    return rootNode == nullptr;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &AVLTree<T, C, TR>::Insert(const T &item) {
    TryInsert(item);
    return *this;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &AVLTree<T, C, TR>::Delete(const T &item) {
    TryDelete(item);
    return *this;
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &AVLTree<T, C, TR>::Clear() {
    ClearNode(rootNode);
    this->SetNumItems(0);
}

template<typename T, typename C, typename TR>
const T &AVLTree<T, C, TR>::FindMin() const {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
const T &AVLTree<T, C, TR>::FindMax() const {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
const T &AVLTree<T, C, TR>::FindStat(size_t k) const {
    assert(k < this->GetNumItems());

    auto curNode = rootNode;
    size_t index = GetSize(rootNode->leftNode);

    while (index != k) {
        if (k <= index) {
            curNode = curNode->leftNode;
            index -= 1 + GetSize(curNode->rightNode);
        }
        if (k > index) {
            curNode = curNode->rightNode;
            index += 1 + GetSize(curNode->leftNode);
        }
    }

    return curNode->item;
}

template<typename T, typename C, typename TR>
size_t AVLTree<T, C, TR>::CountMaxWidth() const {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
size_t AVLTree<T, C, TR>::CountMaxHeight() const {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::PreOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::InOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::PostOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::LevelOrderTraverse() {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &AVLTree<T, C, TR>::operator<<(const T &item) {
    return Insert(item);
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &AVLTree<T, C, TR>::operator>>(const T &item) {
    return Delete(item);
}

template<typename T, typename C, typename TR>
uint8_t AVLTree<T, C, TR>::GetHeight(AVLTree::TreeNode *node) const {
    return node ? node->height : static_cast<uint8_t>(0);
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::FixHeight(AVLTree::TreeNode *node) {
    uint8_t hl = GetHeight(node->leftNode);
    uint8_t hr = GetHeight(node->rightNode);
    node->height = (hl > hr ? hl : hr) + static_cast<uint8_t>(1);
}

template<typename T, typename C, typename TR>
size_t AVLTree<T, C, TR>::GetSize(AVLTree::TreeNode *node) const {
    return node ? node->numItems : static_cast<size_t>(0);
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::FixSize(AVLTree::TreeNode *node) {
    size_t sl = GetSize(node->leftNode);
    size_t sr = GetSize(node->rightNode);
    node->numItems = (sl > sr ? sl : sr) + static_cast<size_t>(1);
}

template<typename T, typename C, typename TR>
int16_t AVLTree<T, C, TR>::CountBalanceFactor(AVLTree::TreeNode *node) const {
    return GetHeight(node->rightNode)-GetHeight(node->leftNode);
}

template<typename T, typename C, typename TR>
typename AVLTree<T, C, TR>::TreeNode *AVLTree<T, C, TR>::RotateRight(AVLTree::TreeNode *node) {
    TreeNode *tempNode = node->leftNode;
    node->leftNode = tempNode->rightNode;
    tempNode->rightNode = node;
    FixHeight(node);
    FixHeight(tempNode);
    FixSize(node);
    FixSize(tempNode);
    return tempNode;
}

template<typename T, typename C, typename TR>
typename AVLTree<T, C, TR>::TreeNode *AVLTree<T, C, TR>::RotateLeft(AVLTree::TreeNode *node) {
    TreeNode *tempNode = node->rightNode;
    node->rightNode = tempNode->leftNode;
    tempNode->leftNode = node;
    FixHeight(node);
    FixHeight(tempNode);
    FixSize(node);
    FixSize(tempNode);
    return tempNode;
}

template<typename T, typename C, typename TR>
typename AVLTree<T, C, TR>::TreeNode *AVLTree<T, C, TR>::FixBalance(AVLTree::TreeNode *node) {
    FixHeight(node);
    FixSize(node);
    auto bf = CountBalanceFactor(node);
    if (bf == 2) {
        if (CountBalanceFactor(node->rightNode) < 0) {
            node->rightNode = RotateRight(node->rightNode);
        }
        return RotateLeft(node);
    }
    if(bf == -2)
    {
        if (CountBalanceFactor(node->leftNode) > 0) {
            node->leftNode = RotateLeft(node->leftNode);
        }
        return RotateRight(node);
    }
    return node;
}

template<typename T, typename C, typename TR>
typename AVLTree<T, C, TR>::TreeNode *AVLTree<T, C, TR>::InsertNode(AVLTree::TreeNode *node, const T &item) {
    if (!node) {
        auto newNode = new TreeNode(item);
        this->IncNumItems();
        return newNode;
    }
    auto cmp = this->GetComparator().ApplyTo(item, node->item);
    if (cmp < 0) {
        node->leftNode = InsertNode(node->leftNode, item);
    }
    else {
        node->rightNode = InsertNode(node->rightNode, item);
    }
    return FixBalance(node);
}

template<typename T, typename C, typename TR>
typename AVLTree<T, C, TR>::TreeNode *AVLTree<T, C, TR>::DeleteNode(AVLTree::TreeNode *node, const T &item) {
    if (!node) {
        return nullptr;
    }

    auto cmp = this->GetComparator().ApplyTo(item, node->item);
    if (cmp < 0) {
        node->leftNode = DeleteNode(node->leftNode, item);
    }
    else {
        if (cmp > 0) {
            node->rightNode = DeleteNode(node->rightNode, item);
        }
        else {
            if (node->leftNode == nullptr || node->rightNode == nullptr) {
                auto tempNode = node->leftNode ? node->leftNode : node->rightNode;

                if (tempNode == nullptr) {
                    tempNode = node;
                    node = nullptr;
                }
                else {
                    *node = *tempNode;
                }
                delete tempNode;
                this->DecNumItems();
            }
            else {
                auto tempNode = node->rightNode;
                while (tempNode->leftNode) {
                    tempNode = tempNode->leftNode;
                }
                node->item = tempNode->item;
                node->rightNode = DeleteNode(node->rightNode, tempNode->item);
            }
        }
    }

    if (node == nullptr) {
        return node;
    }

    return FixBalance(node);
}

template<typename T, typename C, typename TR>
void AVLTree<T, C, TR>::ClearNode(AVLTree::TreeNode *node) {
    if (!node) {
        return;
    }
    ClearNode(node->leftNode);
    ClearNode(node->rightNode);
    delete node;
}

#endif //AVLTREE_H
