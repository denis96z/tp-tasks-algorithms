#include <cstdint>
#include <vector>

#include "types.h"
#include "huffman.h"
#include "huffman_tree.h"

std::vector<byte> read_bytes(IInputStream &original);
std::vector<size_t> count_frequencies(const std::vector<byte> &bytes);
HuffmanTree build_tree(const std::vector<size_t> &frequencies);

void Encode(IInputStream &original, IOutputStream &compressed) {
    auto bytes = read_bytes(original);
    auto frequencies = count_frequencies(bytes);
    auto tree = build_tree(frequencies);
}

void Decode(IInputStream &compressed, IOutputStream &original) {

}

std::vector<byte> read_bytes(IInputStream &original) {
    std::vector<byte> bytes;
    byte b = 0;
    while (original.Read(b)) {
        bytes.push_back(b);
    }
    return bytes;
}

std::vector<size_t> count_frequencies(const std::vector<byte> &bytes) {
    std::vector<size_t> frequencies(NUM_BYTES, 0);
    for (auto b : bytes) {
        ++frequencies[b];
    }
    return frequencies;
}

HuffmanTree build_tree(const std::vector<size_t> &frequencies) {
    std::vector<std::unique_ptr<HuffmanTreeNode>> nodes;
    for (size_t i = 0; i < frequencies.size(); ++i) {
        size_t f = frequencies[i];
        if (f > 0) {
            auto newNode = std::make_unique<HuffmanTreeNode>(f,
                    std::make_unique<byte>(i));
            nodes.push_back(newNode);
        }
    }

    while (nodes.size() > 1) {
        //SORT nodes
        auto minSum = nodes[0]->frequency + nodes[1]->frequency;
        auto newNode = std::make_unique<HuffmanTreeNode>(minSum);

        newNode->leftNode = std::move(nodes[0]);
        newNode->rightNode = std::move(nodes[1]);

        //REMOVE_AT_0
        nodes[0] = std::move(newNode);
    }

    return HuffmanTree(std::move(nodes[0]));
}