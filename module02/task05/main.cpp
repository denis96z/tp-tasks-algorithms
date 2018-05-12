#include <fstream>
#include <cassert>
#include "huffman.h"

class FileInputStream : public IInputStream {
    public:
        explicit FileInputStream(const char *fileName) {
            fs.open(fileName, std::ios::binary);
        }

        bool Read(byte &value) override {
            fs >> value;
            return !fs.eof();
        }

        void Close() {
            fs.close();
        }

    private:
        std::ifstream fs;
};

class FileOutputStream : public IOutputStream {
    public:
        explicit FileOutputStream(const char *fileName) {
            fs.open(fileName, std::ios::binary);
        }

        void Write(byte value) override {
            fs << value;
        }

        void Close() {
            fs.close();
        }

    private:
        std::ofstream fs;
};

int main(){
    FileInputStream original("original.txt");
    FileOutputStream compressed1("encoded.txt");
    Encode(original, compressed1);
    compressed1.Close();
    original.Close();

    FileInputStream compressed2("encoded.txt");
    FileOutputStream decoded("decoded.txt");
    Decode(compressed2, decoded);
    compressed2.Close();
    decoded.Close();

    std::ifstream origStream("original.txt");
    std::ifstream decStream("decoded.txt");

    while(!origStream.eof()) {
        byte a, b;
        origStream >> a;
        decStream >> b;
        assert(a == b);
    }

    return 0;
}