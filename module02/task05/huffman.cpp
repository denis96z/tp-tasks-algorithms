#include <cstdint>
#include <vector>
#include <algorithm>

#include "types.h"
#include "huffman.h"
#include "huffman_tree.h"

std::vector<byte> read_bytes(IInputStream &stream);
void write_bytes(IOutputStream &stream, const std::vector<byte> &bytes);

std::vector<size_t> count_frequencies(const std::vector<byte> &bytes);
HuffmanTree build_tree(const std::vector<size_t> &frequencies);

void Encode(IInputStream &original, IOutputStream &compressed) {
    auto bytes = read_bytes(original);
    auto frequencies = count_frequencies(bytes);
    auto tree = build_tree(frequencies);


}

void Decode(IInputStream &compressed, IOutputStream &original) {

}

std::vector<byte> read_bytes(IInputStream &stream) {
    std::vector<byte> bytes;
    byte b = 0;
    while (stream.Read(b)) {
        bytes.push_back(b);
    }
    return bytes;
}

void write_bytes(IOutputStream &stream, const std::vector<byte> &bytes) {
    for (auto b : bytes) {
        stream.Write(b);
    }
}

std::vector<size_t> count_frequencies(const std::vector<byte> &bytes) {
    std::vector<size_t> frequencies(NUM_BYTES, 0);
    for (auto b : bytes) {
        ++frequencies[b];
    }
    return frequencies;
}

HuffmanTree build_tree(const std::vector<size_t> &frequencies) {
    typedef std::unique_ptr<HuffmanTreeNode> node_ptr_t;

    std::vector<node_ptr_t> nodes;
    for (size_t i = 0; i < frequencies.size(); ++i) {
        size_t f = frequencies[i];
        if (f > 0) {
            auto newNode = std::make_unique<HuffmanTreeNode>(f,
                    std::make_unique<byte>(i));
            nodes.push_back(newNode);
        }
    }

    while (nodes.size() > 1) {
        std::sort(nodes.begin(), nodes.end(),
                  [](const node_ptr_t &a, const node_ptr_t &b) {
            return a->frequency < b->frequency;
        });

        auto minSum = nodes[0]->frequency + nodes[1]->frequency;
        auto newNode = std::make_unique<HuffmanTreeNode>(minSum);

        newNode->leftNode = std::move(nodes[0]);
        newNode->rightNode = std::move(nodes[1]);

        nodes.erase(nodes.begin());
        nodes[0] = std::move(newNode);
    }

    return HuffmanTree(std::move(nodes[0]));
}