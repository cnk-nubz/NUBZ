#ifndef DB_CMD__INCREMENT_COUNTER__H
#define DB_CMD__INCREMENT_COUNTER__H

#include <db/DatabaseSession.h>
#include <db/db_info.h>

namespace db {
namespace cmd {

class IncrementCounter {
public:
    static IncrementCounter exhibitVersion();
    static IncrementCounter mapVersion();
    static IncrementCounter reportId();

    IncrementCounter(db::info::counters::element_type elementType);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getNewVal() const;

private:
    const db::info::counters::element_type elementType;
    std::int32_t newVal;
};
}
}

#endif