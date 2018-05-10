#ifndef BITCODE_H
#define BITCODE_H

#include "types.h"

#include <utility>
#include <vector>
#include <memory>
#include <cassert>

class BitCode {
    public:
        BitCode() = delete;
        explicit BitCode(std::vector<bool> bits);
        BitCode(const BitCode &bitCode) = default;
        BitCode(BitCode &&bitCode) noexcept = default;

        ~BitCode() noexcept = default;

        BitCode& operator =(const BitCode &bitCode) = default;
        BitCode& operator =(BitCode &&bitCode) noexcept = default;

        const std::vector<bool>& GetBits() const;

        void Save(IOutputStream &stream);
        void Load(IInputStream &stream);

    private:
        std::vector<bool> dataBits{};
};

BitCode::BitCode(std::vector<bool> bits) :
        dataBits(std::move(bits)) {}

const std::vector<bool> &BitCode::GetBits() const {
    return dataBits;
}

void BitCode::Save(IOutputStream &stream) {
    std::vector<byte> blob;
    stream.Write(static_cast<byte>(dataBits.size()));

    byte curByte = 0;
    for (size_t i = 0, index = 0; i < dataBits.size(); ++i) {
        auto curBit = static_cast<byte>(dataBits[i] ? 1 : 0);
        curByte |= curBit << index;

        if (index == (NUM_BITS_IN_BYTE - 1)) {
            stream.Write(curByte);
            index = 0;
        }
        else {
            ++index;
        }
    }
}

void BitCode::Load(IInputStream &stream) {
    byte curByte = 0;

    assert(stream.Read(curByte) && curByte > 0);
    dataBits = std::move(std::vector(static_cast<size_t>(curByte, 0)));

    for (size_t i = 0, index = 0; i < dataBits.size(); ++i) {
        if (index == 0) {
            assert(stream.Read(curByte));
        }

        dataBits.push_back((curByte & static_cast<byte>(1)) != 0);

        if (index == (NUM_BITS_IN_BYTE - 1)) {
            index = 0;
        }
        else {
            index += 1;
            curByte >>= 1;
        }
    }
}

class BitCodesTable {
    public:
        BitCodesTable();
        BitCodesTable(const BitCodesTable &table) = delete;
        BitCodesTable(BitCodesTable &&table) noexcept = default;

        ~BitCodesTable() noexcept = default;

        BitCodesTable& operator =(const BitCodesTable &table) = delete;
        BitCodesTable& operator =(BitCodesTable &&table) noexcept = default;

        void Add(byte b, BitCode &&bitCode) noexcept;
        bool GetCode(BitCode &bitCode, byte b) const;

        void Save(IOutputStream &stream);
        void Load(IInputStream &stream);

    private:
        std::vector<std::unique_ptr<BitCode>> bitCodes;
        byte numCodes = 0;
};

BitCodesTable::BitCodesTable() : bitCodes(NUM_BYTES, nullptr) {}

void BitCodesTable::Add(byte b, BitCode &&bitCode) {
    assert(b >= 0 && b < NUM_BYTES);
    assert(bitCodes[b] == nullptr);
    bitCodes[b] = std::make_unique<BitCode>(std::move(bitCode));
    ++numCodes;
}

bool BitCodesTable::GetCode(BitCode &bitCode, byte b) const {
    assert(b >= 0 && b < NUM_BYTES);
    bool result = false;

    if (bitCodes[b] != nullptr) {
        bitCode = *(bitCodes[b]);
    }

    return result;
}

void BitCodesTable::Save(IOutputStream &stream) {
    assert(numCodes > 0);
    stream.Write(numCodes);
    for (size_t i = 0, counter = 0; i < NUM_BYTES && counter < numCodes; ++i) {
        if (bitCodes[i] != nullptr) {
            stream.Write(static_cast<byte>(i));
            bitCodes[i]->Save(stream);
            ++counter;
        }
    }
}

void BitCodesTable::Load(IInputStream &stream) {
    byte curByte = 0;

    assert(stream.Read(curByte) && curByte > 0);
    numCodes = curByte;

    bitCodes = std::move(std::vector<std::unique_ptr<BitCode>>(NUM_BYTES, nullptr));
    for (byte counter = 0; counter < numCodes; ++counter) {
        assert(stream.Read(curByte) && curByte > 0);
        bitCodes[curByte]->Load(stream);
    }
}

#endif //BITCODE_H
