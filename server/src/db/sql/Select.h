#ifndef DB_SQL__SELECT__H
#define DB_SQL__SELECT__H

#include <string>
#include <tuple>
#include <vector>

#include <boost/format.hpp>

#include <utils/type_traits.h>

#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... Fields>
class Select : public Where<Select<Fields...>, Fields...> {
    static_assert(utils::same_table<Fields...>::value, "all fields should be from the same table");

public:
    using return_type = std::tuple<Fields...>;

    std::string str() const {
        boost::format stmt("SELECT %1%\nFROM %2%\n%3%");
        stmt % utils::asSqlList({utils::getColumnName<Fields>()...});
        stmt % utils::getTableName<Fields...>();
        stmt % this->whereStmt();
        return stmt.str();
    }
};
}
}

#endif