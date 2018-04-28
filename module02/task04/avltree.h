#ifndef AVLTREE_H
#define AVLTREE_H

#include "tree.h"
#include <cstdint>

template <typename T, typename C = Comparator<T>, typename TR = TreeTraversal<T>>
class AVLTree : public Tree<T, C, TR> {
    public:
        AVLTree() = default;
        AVLTree(const AVLTree &tree) = delete;
        AVLTree(AVLTree &&tree) noexcept = default;

        ~AVLTree() override = default;

        AVLTree<T, C, TR> &operator =(const Tree &tree) = delete;
        AVLTree<T, C, TR> &operator =(Tree &&tree) override = default;

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

        Tree<T, C, TR>& operator <<(const T &item) override;
        Tree<T, C, TR>& operator >>(const T &item) override;

    private:
        struct TreeNode {
            T item{};
            uint8_t height = 1;
            TreeNode *leftNode = nullptr;
            TreeNode *rightNode = nullptr;

            explicit TreeNode(const T &item) : item(item) {}
        };

        TreeNode *rootNode = nullptr;
};

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::TryInsert(const T &item) {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::TryDelete(const T &item) {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::Has(const T &item) const {
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
bool AVLTree<T, C, TR>::IsEmpty() const {
    throw NotImplementedException();
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
    throw NotImplementedException();
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
    throw NotImplementedException();
}

template<typename T, typename C, typename TR>
Tree<T, C, TR> &AVLTree<T, C, TR>::operator>>(const T &item) {
    throw NotImplementedException();
}

#endif //AVLTREE_H
