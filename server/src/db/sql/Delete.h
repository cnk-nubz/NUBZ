#ifndef DB_SQL__DELETE__H
#define DB_SQL__DELETE__H

#include <string>

#include <boost/format.hpp>

#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... AvailableFields>
class Delete : public Where<Delete<AvailableFields...>, AvailableFields...> {
    static_assert(utils::same_table<AvailableFields...>::value,
                  "all fields should be from the same table");

public:
    std::string str() const {
        boost::format stmt("DELETE FROM %1%\n%2%");
        stmt % utils::getTableName<AvailableFields...>();
        stmt % this->whereStmt();
        return stmt.str();
    }
};
}
}

#endif