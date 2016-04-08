#ifndef REPOSITORY__COUNTERS__H
#define REPOSITORY__COUNTERS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

class Counters {
public:
    enum class Type { LastExhibitVersion, LastReportID };

    Counters(db::DatabaseSession &session);

    std::int32_t get(Type type);

    // returns new value
    std::int32_t increment(Type type);

    void set(Type type, std::int32_t newVal);

private:
    db::DatabaseSession &session;
};

using CounterType = Counters::Type;
}

#endif