#ifndef BITCODE_H
#define BITCODE_H

#include <utility>
#include <vector>
#include <memory>
#include "types.h"

struct BitCode {
    std::vector<bool> dataBits{};
    std::unique_ptr<byte> dataByte = nullptr;

    explicit BitCode(std::vector<bool> bits, bool hasByte = false, byte b) :
            dataBits(std::move(bits)), dataByte(hasByte ? nullptr : std::make_unique<byte>(b)) {
    }
};

#endif //BITCODE_H
