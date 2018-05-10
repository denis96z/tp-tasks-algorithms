#ifndef TREE_H
#define TREE_H

#include <memory>
#include "types.h"

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
        HuffmanTree() = delete;
        explicit HuffmanTree(std::unique_ptr<HuffmanTreeNode> &&root) noexcept;
        HuffmanTree(const HuffmanTree &tree) = delete;
        HuffmanTree(HuffmanTree &&tree) noexcept = default;

        ~HuffmanTree() noexcept = default;

        HuffmanTree& operator =(const HuffmanTree &tree) = delete;
        HuffmanTree& operator =(HuffmanTree &&tree) noexcept = default;

    private:
        std::unique_ptr<HuffmanTreeNode> rootNode = nullptr;
};

HuffmanTree::HuffmanTree(std::unique_ptr<HuffmanTreeNode> &&root) noexcept {
    rootNode = std::move(root);
}

#endif //TREE_H
