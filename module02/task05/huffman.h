#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "types.h"

// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);

// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);

#endif //HUFFMAN_H
