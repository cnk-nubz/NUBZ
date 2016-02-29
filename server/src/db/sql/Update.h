#ifndef DB_SQL__UPDATE__H
#define DB_SQL__UPDATE__H

#include <cassert>

#include <boost/format.hpp>

#include <utils/type_traits.h>

#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... AvailableColumns>
class Update : public Where<Update<AvailableColumns...>, AvailableColumns...> {
    static_assert(utils::same_table<AvailableColumns...>::value,
                  "all columns should be from the same table");

public:
    template <class Column>
    Update &set(const Column &column, const typename Column::value_type::type &newValue) {
        static_assert(::utils::types::find_type<Column, AvailableColumns...>::value,
                      "selected column is not available in this sql query");

        addSet(column, newValue);
        return *this;
    }

    // additional function for non optional value if Column's Value is optional
    template <class Column, class = std::enable_if_t<Column::value_type::is_optional>>
    Update &set(const Column &column, const typename Column::value_type::internal_type &newValue) {
        static_assert(::utils::types::find_type<Column, AvailableColumns...>::value,
                      "selected column is not available in this sql query");

        addSet(column, newValue);
        return *this;
    }

    std::string str() const {
        assert(sets.size() > 0 && "you should call .set() at least once");
        boost::format stmt("UPDATE %1%\nSET %2%\n%3%");
        stmt % utils::getTableName<AvailableColumns...>();
        stmt % utils::asSqlList(sets, ",\n");
        stmt % this->whereStmt();
        return stmt.str();
    }

private:
    template <class Column, class Value>
    void addSet(const Column &col, const Value &newVal) {
        boost::format set("%1%=%2%");
        set % col.name;
        set % utils::sqlVal(newVal);
        sets.push_back(set.str());
    }

    std::vector<std::string> sets;
};
}
}

#endif