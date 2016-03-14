#include "Date.h"

namespace server {
namespace io {
namespace output {

Date::Date(const boost::gregorian::date &date)
    : day(date.day()), month(date.month()), year(date.year()) {
}

communication::Date Date::toThrift() const {
    auto res = communication::Date{};
    res.day = day;
    res.month = month;
    res.year = year;
    return res;
}
}
}
}