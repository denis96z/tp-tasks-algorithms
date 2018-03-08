#include <iostream>
#include "deque.h"

int main() {
    size_t commandsCount = 0;
    std::cin >> commandsCount;

    Deque<int> deque;
    for (size_t i = 0; i < commandsCount; ++i) {
        int command = 0;
        std::cin >> command;

        int value = 0;
        std::cin >> value;

        switch (command) {
            case 1:
                deque.PushFront(value);
                break;

            case 3:
                deque.PushBack(value);
                break;

            case 2:
                if (deque.IsEmpty() && value != -1 || !deque.IsEmpty() && deque.PopFront() != value) {
                    std::cout << "NO";
                    return 0;
                }
                break;

            case 4:
                if (deque.IsEmpty() && value != -1 || !deque.IsEmpty() && deque.PopBack() != value) {
                    std::cout << "NO";
                    return 0;
                }
                break;

            default:
                assert(false);
        }
    }

    std::cout << "YES";
    return 0;
}