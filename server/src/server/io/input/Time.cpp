#include "Time.h"

namespace server {
namespace io {
namespace input {

Time::Time(const communication::Time &thrift)
    : hour(thrift.hour), min(thrift.min), sec(thrift.sec) {
}

::utils::TimePoint Time::toRepo() const {
    auto res = ::utils::TimePoint{};
    res.h = hour;
    res.m = min;
    res.s = sec;
    return res;
}
}
}
}