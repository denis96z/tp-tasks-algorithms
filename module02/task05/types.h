#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <cstddef>

typedef uint8_t byte;
const size_t NUM_BYTES = 256;
const size_t NUM_BITS_IN_BYTE = 8;

#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

#endif //TYPES_H
