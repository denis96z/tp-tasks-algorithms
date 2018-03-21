#include <iostream>
#include "binary_heap.h"

int main() {
    BinaryHeap<int> heap;
    for (int i = 0; i < 50; i++) {
        heap.Add(i);
    }

    while (!heap.IsEmpty()) {
        std::cout << heap.ExtractMax() << " ";
    }
    return 0;
}