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
        if (numTrains <= 0) {
            throw std::bad_exception();
        }

        timetables = new timetable_t[numTrains];

        for (size_t i = 0; i < numTrains; ++i) {
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
        PRINT_ERROR(std::string("[Number of trains is expected to be greater than zero.\n") +
            std::string("Departure time is expected to be greater than arrival time.]"));
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

template <typename T>
class SizedHeap {
    private:
        BinaryHeap<T> heap;
        size_t maxHeapSize = 0;

    public:
        inline void Add(const T &value){
            heap.Add(value);
            maxHeapSize = MAX(maxHeapSize, heap.GetNumItems());
        }

        inline const T &PeekMax() const {
            return heap.PeekMax();
        }

        inline T ExtractMax() {
            return heap.ExtractMax();
        }

        inline size_t GetMaxSize() const {
            return maxHeapSize;
        }
};

size_t count_dead_ends(timetable_t *timetables, size_t numTrains) {
    assert(timetables && numTrains > 0);

    SizedHeap<int> heap;
    heap.Add(-timetables[0].departure);

    for (size_t i = 1; i < numTrains; ++i) {
        if (abs(heap.PeekMax()) < timetables[i].arrival) {
            heap.ExtractMax();
        }
        heap.Add(-timetables[i].departure);
    }

    return heap.GetMaxSize();
}