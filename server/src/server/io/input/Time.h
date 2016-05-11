#ifndef SERVER_IO_INPUT__TIME2__H
#define SERVER_IO_INPUT__TIME2__H

#include <cstdint>

#include <communication/structs_types.h>

#include <utils/TimePoint.h>

namespace server {
namespace io {
namespace input {

struct Time {
    Time(const communication::Time &thrift);
    ::utils::TimePoint toRepo() const;

    const std::int32_t hour;
    const std::int32_t min;
    const std::int32_t sec;
};
}
}
}

#endif