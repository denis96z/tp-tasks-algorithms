#include <iostream>
#include "binary_heap.h"

#define PRINT_ERROR(err_msg) \
    std::cerr << (err_msg);

typedef struct {
    int arrival = 0;
    int departure = 0;
} timetable_t;

size_t count_dead_ends(timetable_t *timetables, size_t numTrains);

int main() {
    size_t numTrains = 0;
    timetable_t *timetables = nullptr;

    try {
        std::cin >> numTrains;
        timetables = new timetable_t[numTrains];

        for (auto i = 0; i < numTrains; ++i) {
            std::cin >> timetables[i].arrival >> timetables[i].departure;
            if (timetables[i].departure < timetables[i].arrival) {
                throw std::bad_exception();
            }
        }

        auto numDeadEnds = count_dead_ends(timetables, numTrains);
        std::cout << numDeadEnds;
    }
    catch (std::bad_alloc& badAllocExc) {
        PRINT_ERROR("[Failed to allocate memory.]");
    }
    catch (std::bad_exception& badInputExc) {
        PRINT_ERROR("[Departure time is expected to be greater than arrival time.]");
    }
    catch (...) {
        PRINT_ERROR("[error]");
    }

    if (timetables) {
        delete[] timetables;
    }

    return 0;
}

#define MAX(x,y) \
    (((x) > (y)) ? (x) : (y))

size_t count_dead_ends(timetable_t *timetables, size_t numTrains) {
    assert(timetables && numTrains > 0);

    BinaryHeap<int> heap;
    heap.Add(timetables[0].departure);

    auto maxHeapSize = 0;
    for (auto i = 1; i < numTrains; ++i) {
        maxHeapSize = MAX(maxHeapSize, heap.GetNumItems());
        if (heap.PeekMax() < timetables[i].arrival) {
            heap.ExtractMax();
        }
        heap.Add(timetables[i].departure);
    }

    return maxHeapSize;
}