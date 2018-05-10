#include <iostream>
#include <vector>
#include "huffman.h"

class MyOutput : public IOutputStream {
    public:
        void Write(byte value) override {
            std::cout << value << " ";
        }
};

class MyInput : public IInputStream {
    public:
        bool Read(byte &value) override {
            if (i++ < 1000) {
                value = static_cast<unsigned char &&>(rand() % 255);
                return true;
            }
            return false;
        }

    private:
        size_t i = 0;
};

int main() {
    MyInput input;
    MyOutput output;

    Encode(input, output);

    return 0;
}