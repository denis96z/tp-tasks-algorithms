#ifndef BITCODE_H
#define BITCODE_H

#include "types.h"

#include <utility>
#include <vector>
#include <memory>
#include <cassert>

struct BitCode {
    std::vector<bool> dataBits{};
    std::unique_ptr<byte> dataByte = nullptr;

    explicit BitCode(std::vector<bool> bits, bool hasByte = false, byte b) :
            dataBits(std::move(bits)), dataByte(hasByte ? nullptr : std::make_unique<byte>(b)) {}

    void Save(IOutputStream &stream) {
        assert(dataByte != nullptr);

        std::vector<byte> blob;
        stream.Write(*dataByte);
        stream.Write(static_cast<byte>(dataBits.size()));

        byte curByte = 0;
        for (size_t i = 0, index = 0; i < dataBits.size(); ++i) {
            curByte |= dataBits[i] << index;

            if (index == (NUM_BITS_IN_BYTE - 1)) {
                stream.Write(curByte);
                index = 0;
            }
            else {
                ++index;
            }
        }
    }

    void Load(IInputStream &stream) {
        byte curByte = 0;
        assert(stream.Read(curByte));
        dataByte = std::make_unique<byte>(curByte);

        assert(stream.Read(curByte) && curByte > 0);
        dataBits = std::move(std::vector(static_cast<size_t>(curByte, 0)));
        for (size_t i = 0, index = 0; i < dataBits.size(); ++i) {
            if (index == 0) {
                assert(stream.Read(curByte));
            }

            dataBits.push_back(curByte & static_cast<byte>(1));

            if (index == (NUM_BITS_IN_BYTE - 1)) {
                index = 0;
            }
            else {
                index += 1;
                curByte >>= 1;
            }
        }
    }
};

#endif //BITCODE_H
