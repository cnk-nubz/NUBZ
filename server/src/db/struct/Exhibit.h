#ifndef DB_STRUCT__EXHIBIT__H
#define DB_STRUCT__EXHIBIT__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include "db/struct/MapElementFrame.h"

namespace db {
    struct Exhibit {
        std::int32_t ID;
        std::string name;
        std::int32_t version;
        boost::optional<MapElementFrame> frame;
    };
}

#endif