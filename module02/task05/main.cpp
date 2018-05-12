#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

#include "huffman.h"

class Output : public IOutputStream {
    public:
        std::vector<byte> dataVector;

        void Write(byte value) override {
            dataVector.push_back(value);
        }
};

class OriginalInput : public IInputStream {
    public:
        OriginalInput() {
            for (size_t i = 0; i < 10000; ++i) {
                data.push_back(rand() % 255);
            }
        }

        bool Read(byte &value) override {
            static size_t i = 0;
            if (i < data.size()) {
                value = data[i++];
                return true;
            }
            return false;
        }

    public:
        std::vector<byte> data;
};

class CompressedInput : public IInputStream {
    public:
        explicit CompressedInput(std::vector<byte> cmp) :
                bytes(std::move(cmp)) {}

        bool Read(byte &value) override {
            if (index < bytes.size()) {
                value = bytes[index++];
                return true;
            }
            return false;
        }

    public:
        size_t index = 0;
        std::vector<byte> bytes;
};

int main() {
    OriginalInput origInput;
    Output compOutput;

    Encode(origInput, compOutput);

    CompressedInput compInput(compOutput.dataVector);
    Output origOutput;

    Decode(compInput, origOutput);

    assert(origInput.data == origOutput.dataVector);
    return 0;
}
