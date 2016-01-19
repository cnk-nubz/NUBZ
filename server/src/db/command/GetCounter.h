#ifndef DB_CMD__GET_COUNTER__H
#define DB_CMD__GET_COUNTER__H

#include <db/struct/MapImage.h>
#include <db/DatabaseSession.h>
#include <db/db_info.h>

namespace db {
namespace cmd {

class GetCounter {
public:
    GetCounter(db::info::counters::element_type elementType);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getResult() const;

private:
    const db::info::counters::element_type elementType;
    std::int32_t result;

    std::string createQuery() const;
};
}
}

#endif