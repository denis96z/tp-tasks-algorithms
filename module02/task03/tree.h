#ifndef TREE_H
#define TREE_H

#include "container.h"

template <typename T>
class TreeTraversal {
    public:
        TreeTraversal() = default;
        TreeTraversal(const TreeTraversal &traversal) = default;
        TreeTraversal(TreeTraversal &&traversal) noexcept = default;

        virtual ~TreeTraversal() = default;

        TreeTraversal& operator =(const TreeTraversal &traversal) = default;
        TreeTraversal& operator =(TreeTraversal &&traversal) noexcept = default;

        virtual void ApplyTo(const T &item) const = 0;
};

template <typename T, typename C = Comparator<T>, typename TR = TreeTraversal<T>>
class Tree : public Container<T, C> {
    public:
        Tree() = default;
        Tree(const Tree &tree) = delete;
        Tree(Tree &&tree) noexcept = default;

        ~Tree() noexcept override = default;

        Tree& operator =(const Tree &tree) = delete;
        Tree& operator =(Tree &&tree) noexcept = default;

        virtual bool TryInsert(const T &item) = 0;
        virtual bool TryDelete(const T &item) = 0;
        virtual bool Has(const T &item) const = 0;
        virtual bool IsEmpty() const = 0;

        virtual Tree& Insert(const T &item) = 0;
        virtual Tree& Delete(const T &item) = 0;
        virtual Tree& Clear() = 0;

        virtual const T& FindMin() const noexcept(false) = 0;
        virtual const T& FindMax() const noexcept(false) = 0;

        virtual size_t CountMaxWidth() const = 0;

        virtual Tree& operator <<(const T &item) = 0;
        virtual Tree& operator >>(const T &item) = 0;

    protected:
        virtual const TR& GetTraversal() const;

    private:
        TR traversal{};
};

template<typename T, typename C, typename TR>
const TR &Tree<T, C, TR>::GetTraversal() const {
    return traversal;
}

#endif //TREE_H
