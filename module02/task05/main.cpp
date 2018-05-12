#include <iostream>
#include <utility>
#include <vector>

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
        bool Read(byte &value) override {
            if (i++ < 5) {
                value = static_cast<unsigned char &&>((rand() % 10) + 'A');
                return true;
            }
            return false;
        }

    private:
        size_t i = 0;
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

    private:
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

    return 0;
}