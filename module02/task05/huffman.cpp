#include <cstdint>
#include <vector>
#include <algorithm>

#include "types.h"
#include "huffman.h"
#include "huffman_tree.h"
#include "bitcode.h"

std::vector<byte> read_bytes(IInputStream &stream);
void write_bytes(IOutputStream &stream, const std::vector<byte> &bytes);

std::vector<size_t> count_frequencies(const std::vector<byte> &bytes);
HuffmanTree build_tree(const std::vector<size_t> &frequencies);
BitCodesTable create_table(const HuffmanTree &tree);

std::vector<byte> encode_data(const std::vector<byte> &originalBytes,
                              const BitCodesTable &table);
std::vector<byte> encode_size(size_t size);

void Encode(IInputStream &original, IOutputStream &compressed) {
    auto bytes = std::move(read_bytes(original));
    auto frequencies = std::move(count_frequencies(bytes));
    auto tree = std::move(build_tree(frequencies));
    auto table = std::move(create_table(tree));

    table.Save(compressed);

    auto encoded = std::move(encode_data(bytes, table));
    for (auto b : encode_size(encoded.size())) {
        compressed.Write(b);
    }

    for (auto b : encoded) {
        compressed.Write(b);
    }
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
            nodes.push_back(std::move(newNode));
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

BitCodesTable create_table(const HuffmanTree &tree) {
    BitCodesTable table;

    std::vector<bool> bits;
    tree.Traverse([&](HuffmanTree::TraverseAction action,
                      const std::unique_ptr<byte> &dataByte) {
        switch (action) {
            case HuffmanTree::TraverseAction::TURN_LEFT:
                bits.push_back(true);
                break;

            case HuffmanTree::TraverseAction::TURN_RIGHT:
                bits.push_back(false);
                break;

            case HuffmanTree::TraverseAction::TURN_BACK:
                if (dataByte != nullptr) {
                    table.Add(*dataByte, BitCode(bits));
                }
                if (!bits.empty()) {
                    bits.pop_back();
                }
                break;
        }
    });

    return table;
}

std::vector<byte> encode_data(const std::vector<byte> &originalBytes,
                              const BitCodesTable &table) {
    std::vector<byte> encodedBytes;

    byte curByte = 0;
    size_t curIndex = 0;

    for (byte b : originalBytes) {
        const auto &bits = table.GetCode(b).GetBits();
        for (bool bt : bits) {
            curByte |= static_cast<byte>(bt ? 1 : 0) << curIndex;

            if (curIndex == NUM_BYTES - 1) {
                encodedBytes.push_back(curByte);
                curIndex = 0;
            }
            else {
                ++curIndex;
            }
        }
    }

    if (curIndex > 0) {
        encodedBytes.push_back(curByte);
    }

    return encodedBytes;
}

byte get_size_byte(size_t size, size_t index) {
    return static_cast<byte>((size >> index) & static_cast<size_t>(255));
}

std::vector<byte> encode_size(size_t size) {
    std::vector<byte> encodedSize;
    encodedSize.push_back(get_size_byte(size, 0));
    encodedSize.push_back(get_size_byte(size, 1));
    encodedSize.push_back(get_size_byte(size, 2));
    encodedSize.push_back(get_size_byte(size, 3));
    return encodedSize;
}