/* ------------------------- Задача 5.3 ---------------------------------
 * На числовой прямой окрасили N отрезков. Известны координаты левого и
 * правого концов каждого отрезка (Li и Ri). Найти длину окрашенной части
 * числовой прямой.
 */

#include <iostream>
#include <cassert>

#include "sort.h"
#include "bad_input.h"

#define PRINT_ERROR(msg) \
    std::cout << msg;

typedef struct {
    int x;
    bool isFirst;
} point_t;

int count_total_segment_length(point_t *points, size_t numPoints);

int main() {
    size_t numSeg = 0;
    point_t *points = nullptr;

    try {
        std::cin >> numSeg;
        if (!numSeg) {
            throw bad_input("Number of line segments should be greater than 0.");
        }

	    const auto numPoints = numSeg << 1;
	    points = new point_t[numPoints];

        for (size_t i = 0; i < numPoints; i += 2) {
            std::cin >> points[i].x;
            std::cin >> points[i + 1].x;

            if (points[i + 1].x < points[i].x) {
                throw bad_input("Second points of line segments should have greater coordinates than first ones.");
            }

            points[i].isFirst = true;
            points[i + 1].isFirst = false;
        }

	    const auto segLength = count_total_segment_length(points, numPoints);
        std::cout << segLength;

        delete[] points;
    }
    catch (std::bad_alloc&) {
        PRINT_ERROR("[out of memory]");
    }
    catch (bad_input &exc) {
        PRINT_ERROR(exc.what());
    }
    catch (...) {
        PRINT_ERROR("[error]");
    }

    return 0;
}

int count_total_segment_length(point_t *points, size_t numPoints) {
    assert(points && numPoints);

    merge_sort<point_t>(points, numPoints, [](const point_t &left, const point_t &right) -> int {
        return left.x - right.x;
    });

    // Первая точка обязательно должна быть началом отрезка.
    assert(points[0].isFirst);
    // Количество наложенных друг на друга отрезков.
    size_t numSeg = 1;

	auto segLength = 0;
    for (size_t i = 1; i < numPoints; ++i) {
        if (numSeg) {
            segLength += points[i].x - points[i - 1].x;
        }
        numSeg += points[i].isFirst ? 1 : -1;
    }

    return segLength;
}