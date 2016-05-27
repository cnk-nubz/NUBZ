#ifndef DB_SQL__UPDATE__H
#define DB_SQL__UPDATE__H

#include <cassert>

#include <boost/format.hpp>

#include <utils/type_traits.h>

#include "Condition.h"
#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... AvailableFields>
class Update : public Where<Update<AvailableFields...>, AvailableFields...> {
    static_assert(utils::same_table<AvailableFields...>::value,
                  "all fields should be from the same table");

public:
    template <class Column, class = std::enable_if_t<Column::field_type::is_optional>>
    Update &set(Column, const _Null) {
        using Field = typename Column::field_type;
        static_assert(::utils::types::find_type<Field, AvailableFields...>::value,
                      "selected field is not available in this sql query");

        addSet<Column>(typename Field::type{});
        return *this;
    }

    template <class Column>
    Update &set(Column, const typename Column::field_type::type &newValue) {
        using Field = typename Column::field_type;
        static_assert(::utils::types::find_type<Field, AvailableFields...>::value,
                      "selected field is not available in this sql query");

        addSet<Column>(newValue);
        return *this;
    }

    // additional function for non optional value if Field is optional
    template <class Column, class = std::enable_if_t<Column::field_type::is_optional>>
    Update &set(Column, const typename Column::field_type::internal_type &newValue) {
        using Field = typename Column::field_type;
        static_assert(::utils::types::find_type<Field, AvailableFields...>::value,
                      "selected field is not available in this sql query");

        addSet<Column>(newValue);
        return *this;
    }

    std::string str() const {
        assert(sets.size() > 0 && "you should call .set() at least once");
        boost::format stmt("UPDATE %1%\nSET %2%\n%3%");
        stmt % utils::getTableName<AvailableFields...>();
        stmt % utils::asSqlList(sets, ",\n");
        stmt % this->whereStmt();
        return stmt.str();
    }

private:
    template <class Column, class Value>
    void addSet(const Value &newVal) {
        boost::format set("%1%=%2%");
        set % utils::getColumnName<typename Column::field_type>();
        set % translation::sqlVal(newVal);
        sets.push_back(set.str());
    }

    std::vector<std::string> sets;
};
}
}

#endif