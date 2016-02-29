#ifndef DB_SQL__DELETE__H
#define DB_SQL__DELETE__H

#include <string>

#include <boost/format.hpp>

#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... AvailableColumns>
class Delete : public Where<Delete<AvailableColumns...>, AvailableColumns...> {
    static_assert(utils::same_table<AvailableColumns...>::value,
                  "all columns should be from the same table");

public:
    std::string str() const {
        boost::format stmt("DELETE FROM %1%\n%2%");
        stmt % utils::getTableName<AvailableColumns...>();
        stmt % this->whereStmt();
        return stmt.str();
    }
};
}
}

#endif