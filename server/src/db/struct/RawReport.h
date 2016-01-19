#ifndef DB_STRUCT__RAW_REPORT__H
#define DB_STRUCT__RAW_REPORT__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

namespace db {
struct RawReport {
    struct Event {
        boost::optional<std::int32_t> exhibitId;
        std::int32_t durationInSecs;
        std::vector<std::int32_t> actions;
    };

    std::int32_t ID;
    std::vector<Event> history;
};
}

#endif