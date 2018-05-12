#ifndef TREE_H
#define TREE_H

#include "types.h"

#include <memory>
#include <cassert>
#include <functional>
#include <utility>

struct HuffmanTreeNode {
    size_t frequency = 0;
    std::unique_ptr<byte> dataByte = nullptr;

    std::unique_ptr<HuffmanTreeNode> leftNode = nullptr;
    std::unique_ptr<HuffmanTreeNode> rightNode = nullptr;

    explicit HuffmanTreeNode(size_t freq, std::unique_ptr<byte> &&b = nullptr) noexcept :
            frequency(freq), dataByte(std::move(b)) {}
};

class HuffmanTree {
    public:
        typedef std::unique_ptr<HuffmanTreeNode> node_ptr_t;

        HuffmanTree() = delete;
        explicit HuffmanTree(node_ptr_t &&root) noexcept;
        HuffmanTree(const HuffmanTree &tree) = delete;
        HuffmanTree(HuffmanTree &&tree) noexcept = default;

        ~HuffmanTree() noexcept = default;

        HuffmanTree& operator =(const HuffmanTree &tree) = delete;
        HuffmanTree& operator =(HuffmanTree &&tree) noexcept = default;

        enum class TraverseAction {
            TURN_LEFT, TURN_RIGHT, TURN_BACK, STOP
        };

        using traverse_function_t = std::function<void(TraverseAction action,
                const std::unique_ptr<byte> &dataByte)>;
        void Traverse(const traverse_function_t &tFunc) const;

    private:
        node_ptr_t rootNode = nullptr;

        void Traverse(const node_ptr_t &curNode, const traverse_function_t &tFunc) const;
};

HuffmanTree::HuffmanTree(std::unique_ptr<HuffmanTreeNode> &&root) noexcept {
    rootNode = std::move(root);
}

void HuffmanTree::Traverse(const HuffmanTree::traverse_function_t &tFunc) const {
    assert(rootNode != nullptr);
    Traverse(rootNode, tFunc);
}

void HuffmanTree::Traverse(const HuffmanTree::node_ptr_t &curNode,
                           const HuffmanTree::traverse_function_t &tFunc) const {
    if (curNode->leftNode == nullptr && curNode->rightNode == nullptr) {
        assert(curNode->dataByte != nullptr);
        tFunc(TraverseAction::STOP, curNode->dataByte);
    }

    if (curNode->leftNode != nullptr) {
        tFunc(TraverseAction::TURN_LEFT, nullptr);
        Traverse(curNode->leftNode, tFunc);
    }

    if (curNode->rightNode != nullptr) {
        tFunc(TraverseAction::TURN_RIGHT, nullptr);
        Traverse(curNode->rightNode, tFunc);
    }

    tFunc(TraverseAction::TURN_BACK, nullptr);
}

#endif //TREE_H
