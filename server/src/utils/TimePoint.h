#ifndef UTILS__TIME_POINT__H
#define UTILS__TIME_POINT__H

#include <cstdint>

namespace utils {

struct TimePoint {
    std::int32_t h;
    std::int32_t m;
    std::int32_t s;

    inline TimePoint after(std::int32_t secs) const {
        TimePoint res;
        res.h = h;
        res.m = m;
        res.s = s;

        res.s += secs;

        res.m += res.s / 60;
        res.s %= 60;

        res.h += res.m / 60;
        res.m %= 60;
        res.h %= 24;

        return res;
    }
};
}

#endif