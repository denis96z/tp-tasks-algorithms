#ifndef HUFFMAN_H
#define HUFFMAN_H

#define interface struct

interface IInputStream {
        // Возвращает false, если поток закончился
        virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
        virtual void Write(byte value) = 0;
};

// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);

// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);


#endif //HUFFMAN_H
