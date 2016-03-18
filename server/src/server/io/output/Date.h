#ifndef SERVER_IO_OUTPUT__DATE__H
#define SERVER_IO_OUTPUT__DATE__H

#include <cstdint>

#include <boost/date_time/gregorian/gregorian.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace output {

struct Date {
    using thrift_t = communication::Date;

    Date(const boost::gregorian::date &date);

    thrift_t toThrift() const;

    const std::int32_t day;
    const std::int32_t month;
    const std::int32_t year;
};
}
}
}

#endif